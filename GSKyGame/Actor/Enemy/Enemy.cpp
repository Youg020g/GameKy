#include "Enemy.h"
#include "../../World/IWorld.h"
#include "../../Field/Field.h"
#include "../../CollisionDetection/Line/Line.h"
#include "../../AssetsID/Assets.h"


// 敵のモーション番号
enum {                      
    MotionIdle = 0,	// アイドル
    MotionWalk = 1,	// 歩き
    MotionDisappear = 10,	// ガード
    MotionDamage = 12,	// ダメージ
    MotionDown = 14,	// ダウン
};

// 移動判定の距離
const float WalkDistance{ 20.0f };
// 移動スピード
const float WalkSpeed{ 0.05f };
// 振り向く角度
const float TurnAngle{ 2.5f };

// 自分の高さ
const float EnemyHeight{ 1.0f };
// 衝突判定用の半径
const float EnemyRadius{ 0.6f };
// 足元のオフセット
const float FootOffset{ 0.1f };
// 重力
const float Gravity{ -0.016f };


// コンストラクタ
Enemy::Enemy(IWorld* world, const GSvector3& position) :
    mesh_{ Mesh_Enemy, Mesh_Enemy, Mesh_Enemy, MotionIdle },
    motion_{ MotionIdle },
    motion_loop_{ true },
    state_{ State::Idle },
    state_timer_{ 0.0f },
    player_{ nullptr } {
    // ワールドの設定
    world_ = world;
    // タグ名の設定
    tag_ = "EnemyTag";
    // 名前の設定
    name_ = "Enemy";
    // 衝突判定球の設定
    collider_ = BoundingSphere{ EnemyRadius, GSvector3{0.0f, EnemyHeight, 0.0f} };
    // 座標の初期化
    transform_.position(position);
    // メッシュの変換行列を初期化
    mesh_.transform(transform_.localToWorldMatrix());

    // プレーヤーを検索する
    Actor* player = world_->find_actor("Player");
    if (player != nullptr) {
       angle_player_ = player->transform().position();
       angle_player_.y = FootOffset;
       transform().lookAt(angle_player_);
    }
}

// 更新
void Enemy::update(float delta_time) {
    // プレーヤーを検索する
    player_ = world_->find_actor("Player");
    // 状態の更新
    update_state(delta_time);
    // 重力を更新
    velocity_.y += Gravity * delta_time;
    // 重力を加える
    transform_.translate(0.0f, velocity_.y, 0.0f);
    // フィールドとの衝突判定
    collide_field();
    // モーションを変更
    mesh_.change_motion(motion_, motion_loop_);
    // メッシュを更新
    mesh_.update(delta_time);
    // ワールド行列を設定
    mesh_.transform(transform_.localToWorldMatrix());

}

// 描画
void Enemy::draw() const {
    // メッシュの描画
    mesh_.draw();
    // 衝突判定のデバッグ表示
    collider().draw();
}

// 衝突処理
void Enemy::react(Actor& other) {
    // ダメージ中またはダウン中の場合は何もしない
    if (state_ == State::Damage || state_ == State::Down || state_ == State::Disappear) return;
    // プレーヤーの弾に衝突した
    if (other.tag() == "PlayerTag" || other.tag() == "PlayerAttackTag") {
        // ダメージ効果音を再生
        gsPlaySE(Se_EnemeyDamage);
        //攻撃判定に当たっていればスコア追加
        if (other.tag() == "PlayerAttackTag")world_->add_score(1);
       // 残りの体力がなければダウン状態に遷移
       change_state(State::Damage, MotionDamage, false);
       return;
    }
}

// 状態の更新
void Enemy::update_state(float delta_time) {
    // 各状態に分岐する
    switch (state_) {
    case State::Idle:   idle(delta_time);   break;
    case State::Walk:   walk(delta_time);   break;
    case State::Damage: damage(delta_time); break;
    case State::Disappear: disappear(delta_time); break;
    case State::Down:   down(delta_time);   break;
    }

    // 状態タイマの更新
    state_timer_ += delta_time;
}

// 状態の変更
void Enemy::change_state(State state, GSuint motion, bool loop) {
    // モーション番号の更新
    motion_ = motion;
    // モーションのループ指定
    motion_loop_ = loop;
    // 状態の更新
    state_ = state;
    // 状態タイマの初期化
    state_timer_ = 0.0f;
}

// アイドル状態
void Enemy::idle(float delta_time) {
    // 歩くか？
    if (is_walk()) {
        // 歩き状態に遷移
        change_state(State::Walk, MotionWalk);
        return;
    }
    // 何もなければ、アイドル中のまま
    change_state(State::Idle, MotionIdle);
}

// 移動中
void Enemy::walk(float delta_time) {
    // 前進する（ローカル座標基準）
    transform_.translate(0.0f, 0.0f, WalkSpeed * delta_time);
}

// ダメージ中
void Enemy::damage(float delta_time) {
    enable_collider_ = false;
    // ダメージモーション中か？
    if (state_timer_ < mesh_.motion_end_time()) {
        // ノックバックする
        transform_.translate(velocity_ * delta_time, GStransform::Space::World);
        velocity_ -= GSvector3{ velocity_.x, 0.0f, velocity_.z } *0.5f * delta_time;
        return;
    }

    change_state(State::Down, MotionDown, false);
}

void Enemy::down(float delta_time){
    if (state_timer_ >= mesh_.motion_end_time()) {
        // ダウンモーションが終了したら死亡 
        die();
    }
}

void Enemy::disappear(float delta_time){
    enable_collider_ = false;
    if (state_timer_ >= mesh_.motion_end_time()) {
        // 消滅モーションが終了したら死亡 
        die();
    }
}

// 移動判定
bool Enemy::is_walk() const {
    // 移動距離内かつ前方向と前向き方向のベクトルとターゲット方向のベクトルの角度差が100.0度以下か？
    return (target_distance() <= WalkDistance);
}

// ターゲットとの距離を求める
float Enemy::target_distance() const {
    // ターゲットがいなければ最大距離を返す
    if (player_ == nullptr) return FLT_MAX; // float型の最大値
    // ターゲットとの2点間の距離を返す
    return GSvector3::distance(player_->transform().position(), transform_.position());
}

void Enemy::collide_field(){
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
        // 交差した点からy座標のみ補正する
        position.y = intersect.y;
        // 座標を変更する
        transform_.position(position);
        // 重力を初期化する
        velocity_.y = 0.0f;
    }

    if ( state_ != State::Disappear && ( world_->field()->ground_min >= transform_.position() 
        || world_->field()->ground_max <= transform_.position() || transform_.position().z <= -20.0f ||transform_.position().z >= 20.0f)) {
        //遷移
        change_state(State::Disappear, MotionDisappear, false);
    }
}

void Enemy::collide_actor(Actor& other){
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
