#include "Player.h"

// コンストラクタ
Player::Player(IWorld* world, const GSvector3& position) {
    name_ = "Player";
}

// 更新
void Player::update(float delta_time) {}

// 描画
void Player::draw() const {}

// 衝突リアクション
void Player::react(Actor& other) {}