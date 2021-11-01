#include "Player.h"
#include "../../World/IWorld.h"
#include "../../Field/Field.h"
#include "../../CollisionDetection/Line/Line.h"
#include "../../AssetsID/Assets.h"
#include "../AttackCollider/AttackCollider.h"

// モーション番号
enum {
    MotionIdle = 0,     // アイドル
    MotionWalk = 1,     // 前進
    MotionDamage = 14,    // ダメ―ジ
    MotionGetUp = 16,     // 起き上がり
    MotionAttack = 17    // 攻撃
};

// 移動速度
const float WalkSpeed{ 0.25f };
// 自分の高さ
const float PlayerHeight{ 1.0f };
// 衝突判定用の半径
const float PlayerRadius{ 0.5f };
// 足元のオフセット
const float FootOffset{ 0.1f };
// 重力値
const float Gravity{ -0.016f };


// コンストラクタ
Player::Player(IWorld* world, const GSvector3& position) :
    mesh_{ 0, 0, 0, MotionIdle },
    motion_{ MotionIdle },
    motion_loop_{ true },
    state_{ State::Move },
    state_timer_{ 0.0f } {
    // ワールドを設定
    world_ = world;
    // タグ名の設定
    tag_ = "PlayerTag";
    // 名前の設定
    name_ = "Player";
    // 衝突判定球の設定
    collider_ = BoundingSphere{ PlayerRadius, GSvector3{0.0f, PlayerHeight, 0.0f} };
    // 座標の初期化
    transform_.position(position);
    // メッシュの変換行列を初期化
    mesh_.transform(transform_.localToWorldMatrix());
}

// 更新
void Player::update(float delta_time) {
    // 状態の更新
    update_state(delta_time);
    // 重力値を更新
    velocity_.y += Gravity * delta_time;
    // 重力を加える
    transform_.translate(0.0f, velocity_.y, 0.0f);
    // フィールドとの衝突判定
    collide_field();
    // モーションを変更
    mesh_.change_motion(motion_, motion_loop_);
    // メッシュを更新
    mesh_.update(delta_time);
    // 行列を設定
    mesh_.transform(transform_.localToWorldMatrix());

}

// 描画
void Player::draw() const {
    // メッシュを描画
    mesh_.draw();
    // 衝突判定球のデバッグ表示
    collider().draw();
}

// 衝突リアクション
void Player::react(Actor& other) {
    // ダメージ中と起き上がり中は何もしない
    if (state_ == State::Damage || state_ == State::GetUp) return;
    // 敵と衝突したか？
    if (other.tag() == "EnemyTag") {
        // 衝突した場合は、ダメージ状態に変更
        change_state(State::Damage, MotionDamage, false);
        return;
    }

   
}

// 状態の更新
void Player::update_state(float delta_time) {
    // 状態遷移
    switch (state_) {
    case State::Move:   move(delta_time);   break;
    case State::Attack: attack(delta_time); break;
    case State::Damage: damage(delta_time); break;
    case State::GetUp: get_up(delta_time); break;
    }
    // 状態タイマの更新
    state_timer_ += delta_time;
}

// 状態の変更
void Player::change_state(State state, GSuint motion, bool loop) {
    motion_ = motion;
    motion_loop_ = loop;
    state_ = state;
    state_timer_ = 0.0f;
}

// 移動処理
void Player::move(float delta_time) {
    // スペースキーで攻撃
    if (gsGetKeyState(GKEY_SPACE)) {
        // 攻撃判定を生成
        generate_attack_collider();
        change_state(State::Attack, MotionAttack);
        return;
    }
    // 何もしなければアイドル状態
    GSint motion{ MotionIdle };
    // 前後移動するときの速さ
    float forward_speed{ 0.0f };
    // 左右移動するときの速さ
    float side_speed{ 0.0f };
    // WASD移動
    if (gsGetKeyState(GKEY_W)) {
        forward_speed = WalkSpeed;      // 前進
        motion = MotionWalk;
    }
    else if (gsGetKeyState(GKEY_S)) {
        forward_speed = -WalkSpeed;     // 後退
        motion = MotionWalk;
    }
    else if (gsGetKeyState(GKEY_A)) {
        side_speed = WalkSpeed;        // 左移動
        motion = MotionWalk;
    }
    else if (gsGetKeyState(GKEY_D)) {
        side_speed = -WalkSpeed;        // 右移動
        motion = MotionWalk;
    }
    
    // 移動状態にする
    change_state(State::Move, motion);

    // マウスの左右移動で方向を変える
    int mx, my, mz;
    gsGetMouseVelocity(&mx, &my, &mz);
    float yaw = (float)-mx * 0.5f;
    transform_.rotate(0.0f, yaw * delta_time, 0.0f);

    // 平行移動する
    transform_.translate(side_speed * delta_time, 0.0f, forward_speed * delta_time);

}

// 攻撃中
void Player::attack(float delta_time) {
    // 攻撃モーションの終了を待つ
    if (state_timer_ >= mesh_.motion_end_time()) {
        move(delta_time);
    }
}

// ダメージ中
void Player::damage(float delta_time) {
    // ダメージモーションの終了を待つ
    if (state_timer_ >= mesh_.motion_end_time()) {
        move(delta_time);
    }
    // 衝突した場合は、起き上がり状態に変更
    change_state(State::GetUp, MotionGetUp, false);
}

void Player::get_up(float delta_time){
    // 起き上がりモーションの終了を待つ
    if (state_timer_ >= mesh_.motion_end_time() ) {
        move(delta_time);
    }
}

// フィールドとの衝突判定
void Player::collide_field() {
    
    // 壁との衝突判定（球体との判定）
    GSvector3 center; // 衝突後の球体の中心座標
    if (world_->field()->collide(collider(), &center)) {
        // y座標は変更しない
        center.y = transform_.position().y;
        // 補正後の座標に変更する
        transform_.position(center);
    }
    // 地面との衝突判定（線分との交差判定）
    GSvector3 position = transform_.position();
    Line line;
    line.start = position + collider_.center;
    line.end = position + GSvector3{ 0.0f, -FootOffset, 0.0f };
    GSvector3 intersect;  // 地面との交点
    if (world_->field()->collide(line, &intersect)) {
        // 交点の位置からy座標のみ補正する
        position.y = intersect.y;
        // 座標を変更する
        transform_.position(position);
        // 重力を初期化する
        velocity_.y = 0.0f;
    }

    //グラウンド外に出ないようにクランプ
    position = position.clamp(world_->field()->ground_min, world_->field()->ground_max);
    // 座標を変更する
    transform_.position(position);
}

// アクターとの衝突処理
void Player::collide_actor(Actor& other) {
    // ｙ座標を除く座標を求める
    GSvector3 position = transform_.position();
    position.y = 0.0f;
    GSvector3 target = other.transform().position();
    target.y = 0.0f;
    // 相手との距離
    float distance = GSvector3::distance(position, target);
    // 衝突判定球の半径同士を加えた長さを求める
    float length = collider_.radius + other.collider().radius;
    // 衝突判定球の重なっている長さを求める
    float overlap = length - distance;
    // 重なっている部分の半分の距離だけ離れる移動量を求める
    GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
    transform_.translate(v, GStransform::Space::World);
    // フィールドとの衝突判定
    collide_field();
}

// 攻撃判定を生成する
void Player::generate_attack_collider() {
    // 攻撃判定を出現させる場所の距離
    const float AttackColliderDistance{ 1.5f };
    // 攻撃判定の半径
    const float AttackColliderRadius{ 1.0f };
    // 攻撃判定を出す場所の高さ
    const float AttackColliderHeight{ 1.0f };

    // 攻撃判定が有効になるまでの遅延時間
    const float AttackCollideDelay{ 15.0f };
    // 攻撃判定の寿命
    const float AttackCollideLifeSpan{ 5.0f };

    // 衝突判定を出現させる座標を求める（前方の位置）
    GSvector3 position = transform_.position() + transform_.forward() * AttackColliderDistance;
    // 高さの補正（足元からの高さ）
    position.y += AttackColliderHeight;
    // 衝突判定用の球を作成
    BoundingSphere collider{ AttackColliderRadius, position };
    // 衝突判定を出現させる
    world_->add_actor(new AttackCollider{ world_, collider,
        "PlayerAttackTag", "PlayerAttack", tag_, AttackCollideLifeSpan, AttackCollideDelay });
}