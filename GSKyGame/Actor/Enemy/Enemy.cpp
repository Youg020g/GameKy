#include "Enemy.h"

// �G�̃��[�V�����ԍ�
enum {                      
    MotionIdle = 0,	// �A�C�h��
    MotionWalk = 1,	// ����
    MotionGuard = 10,	// �K�[�h
    MotionDamage = 12,	// �_���[�W
    MotionDown = 14,	// �_�E��
};

// �U���������̋���
const float TurnDistance{ 1.5f };
// �U������̋���
const float AttackDistance{ 1.2f };
// �ړ�����̋���
const float WalkDistance{ 10.0f };
// �ړ��X�s�[�h
const float WalkSpeed{ 0.025f };
// �U������p�x
const float TurnAngle{ 2.5f };

// �R���X�g���N�^
Enemy::Enemy(IWorld* world, const GSvector3& position, Actor* player) :
    mesh_{ 1, 1, 1, MotionIdle },
    motion_{ MotionIdle },
    motion_loop_{ true },
    state_{ State::Idle },
    state_timer_{ 0.0f },
    player_{ player } {
    // ���W�̏�����
    transform_.position(position);
    // ���[���h�ϊ��s��̏�����
    mesh_.transform(transform_.localToWorldMatrix());
}

// �X�V
void Enemy::update(float delta_time) {
    // ��Ԃ̍X�V
    update_state(delta_time);
    // ���[�V������ύX
    mesh_.change_motion(motion_);
    // ���b�V�����X�V
    mesh_.update(delta_time);
    // �s���ݒ�
    mesh_.transform(transform_.localToWorldMatrix());
}

// �`��
void Enemy::draw() const {
    // ���b�V���̕`��
    mesh_.draw();
}

// �Փˏ���
void Enemy::react(Actor& other) {
    // �����ɏՓ˔���̏���������Ƃ���

    // �Փ˂����ꍇ�́A�_���[�W��ԂɕύX
    change_state(State::Damage, MotionDamage, false);
}

// ��Ԃ̍X�V
void Enemy::update_state(float delta_time) {
    // �e��Ԃɕ��򂷂�
    switch (state_) {
    case State::Idle:   idle(delta_time);   break;
    case State::Walk:   walk(delta_time);   break;
    case State::Damage: damage(delta_time); break;
    }
    // ��ԃ^�C�}�̍X�V
    state_timer_ += delta_time;
}

// ��Ԃ̕ύX
void Enemy::change_state(State state, GSuint motion, bool loop) {
    // ���[�V�����ԍ��̍X�V
    motion_ = motion;
    // ���[�V�����̃��[�v�w��
    motion_loop_ = loop;
    // ��Ԃ̍X�V
    state_ = state;
    // ��ԃ^�C�}�̏�����
    state_timer_ = 0.0f;
}

// �A�C�h�����
void Enemy::idle(float delta_time) {
   
    // �������H
    if (is_walk()) {
        // ������ԂɑJ��
        change_state(State::Walk, MotionWalk);
        return;
    }
    // �����Ȃ���΁A�A�C�h�����̂܂�
    change_state(State::Idle, MotionIdle);
}

// �ړ���
void Enemy::walk(float delta_time) {
    // �^�[�Q�b�g�����̊p�x�����߂�@(������������ς���悤�Ɋp�x�𐧌�����j
    float angle = CLAMP(target_signed_angle(), -TurnAngle, TurnAngle);
    // �^�[�Q�b�g����������
    transform_.rotate(0.0f, angle * delta_time, 0.0f);
    // �O�i����i���[�J�����W��j
    transform_.translate(0.0f, 0.0f, WalkSpeed * delta_time);
}

// �_���[�W��
void Enemy::damage(float delta_time) {
    if (state_timer_ < mesh_.motion_end_time()) {
        // �_���[�W���[�V�������͉������Ȃ�
        return;
    }
    else {
        // �A�C�h�����ɑJ��
        idle(delta_time);
    }
}

// �ړ�����
bool Enemy::is_walk() const {
    // �ړ����������O�����ƑO���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����100.0�x�ȉ����H
    return (target_distance() <= WalkDistance) && (target_angle() <= 100.0f);
}

// �O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�i�����t���j
float Enemy::target_signed_angle() const {
    // �^�[�Q�b�g�����̃x�N�g�������߂�
    GSvector3 to_target = player_->transform().position() - transform_.position();
    // �O���������̃x�N�g�����擾
    GSvector3 forward = transform_.forward();
    // �x�N�g����y�����𖳌��ɂ���
    forward.y = 0.0f;
    to_target.y = 0.0f;
    // �O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�
    return GSvector3::signedAngle(forward, to_target);
}

// �O���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x�������߂�i�����Ȃ��j
float Enemy::target_angle() const {
    return std::abs(target_signed_angle());
}

// �^�[�Q�b�g�Ƃ̋��������߂�
float Enemy::target_distance() const {
    return (player_->transform().position() - transform_.position()).magnitude();
}