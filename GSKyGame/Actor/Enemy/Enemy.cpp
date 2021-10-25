#include "Enemy.h"

// 敵のモーション番号
enum {                      
    MotionIdle = 0,	// アイドル
    MotionWalk = 1,	// 歩き
    MotionGuard = 10,	// ガード
    MotionDamage = 12,	// ダメージ
    MotionDown = 14,	// ダウン
};

// 振り向き判定の距離
const float TurnDistance{ 1.5f };
// 攻撃判定の距離
const float AttackDistance{ 1.2f };
// 移動判定の距離
const float WalkDistance{ 10.0f };
// 移動スピード
const float WalkSpeed{ 0.025f };
// 振り向く角度
const float TurnAngle{ 2.5f };

// コンストラクタ
Enemy::Enemy(IWorld* world, const GSvector3& position, Actor* player) :
    mesh_{ 1, 1, 1, MotionIdle },
    motion_{ MotionIdle },
    motion_loop_{ true },
    state_{ State::Idle },
    state_timer_{ 0.0f },
    player_{ player } {
    // 座標の初期化
    transform_.position(position);
    // ワールド変換行列の初期化
    mesh_.transform(transform_.localToWorldMatrix());
}

// 更新
void Enemy::update(float delta_time) {
    // 状態の更新
    update_state(delta_time);
    // モーションを変更
    mesh_.change_motion(motion_);
    // メッシュを更新
    mesh_.update(delta_time);
    // 行列を設定
    mesh_.transform(transform_.localToWorldMatrix());
}

// 描画
void Enemy::draw() const {
    // メッシュの描画
    mesh_.draw();
}

// 衝突処理
void Enemy::react(Actor& other) {
    // ここに衝突判定の処理があるとする

    // 衝突した場合は、ダメージ状態に変更
    change_state(State::Damage, MotionDamage, false);
}

// 状態の更新
void Enemy::update_state(float delta_time) {
    // 各状態に分岐する
    switch (state_) {
    case State::Idle:   idle(delta_time);   break;
    case State::Walk:   walk(delta_time);   break;
    case State::Damage: damage(delta_time); break;
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
    // ターゲット方向の角度を求める　(少しずつ向きを変えるように角度を制限する）
    float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
    // ターゲット方向を向く
    transform_.rotate(0.0f, angle * delta_time, 0.0f);
    // 前進する（ローカル座標基準）
    transform_.translate(0.0f, 0.0f, WalkSpeed * delta_time);
}

// ダメージ中
void Enemy::damage(float delta_time) {
    if (state_timer_ < mesh_.motion_end_time()) {
        // ダメージモーション中は何もしない
        return;
    }
    else {
        // アイドル中に遷移
        idle(delta_time);
    }
}

// 移動判定
bool Enemy::is_walk() const {
    // 移動距離内かつ前方向と前向き方向のベクトルとターゲット方向のベクトルの角度差が100.0度以下か？
    return (target_distance() <= WalkDistance) && (target_angle() <= 100.0f);
}

// 前向き方向のベクトルとターゲット方向のベクトルの角度差を求める（符号付き）
float Enemy::target_signed_angle() const {
    // ターゲット方向のベクトルを求める
    GSvector3 to_target = player_->transform().position() - transform_.position();
    // 前向き方向のベクトルを取得
    GSvector3 forward = transform_.forward();
    // ベクトルのy成分を無効にする
    forward.y = 0.0f;
    to_target.y = 0.0f;
    // 前向き方向のベクトルとターゲット方向のベクトルの角度差を求める
    return GSvector3::signedAngle(forward, to_target);
}

// 前向き方向のベクトルとターゲット方向のベクトルの角度差を求める（符号なし）
float Enemy::target_angle() const {
    return std::abs(target_signed_angle());
}

// ターゲットとの距離を求める
float Enemy::target_distance() const {
    return (player_->transform().position() - transform_.position()).magnitude();
}