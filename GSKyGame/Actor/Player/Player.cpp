#include "Player.h"

// �R���X�g���N�^
Player::Player(IWorld* world, const GSvector3& position) {
    name_ = "Player";
}

// �X�V
void Player::update(float delta_time) {}

// �`��
void Player::draw() const {}

// �Փ˃��A�N�V����
void Player::react(Actor& other) {}