#include "Enemy.h"
#include "../../World/IWorld.h"
#include "../../Field/Field.h"
#include "../../CollisionDetection/Line/Line.h"
#include "../../AssetsID/Assets.h"


// �G�̃��[�V�����ԍ�
enum {                      
    MotionIdle = 0,	// �A�C�h��
    MotionWalk = 1,	// ����
    MotionDisappear = 10,	// �K�[�h
    MotionDamage = 12,	// �_���[�W
    MotionDown = 14,	// �_�E��
};

// �ړ�����̋���
const float WalkDistance{ 20.0f };
// �ړ��X�s�[�h
const float WalkSpeed{ 0.05f };
// �U������p�x
const float TurnAngle{ 2.5f };

// �����̍���
const float EnemyHeight{ 1.0f };
// �Փ˔���p�̔��a
const float EnemyRadius{ 0.6f };
// �����̃I�t�Z�b�g
const float FootOffset{ 0.1f };
// �d��
const float Gravity{ -0.016f };


// �R���X�g���N�^
Enemy::Enemy(IWorld* world, const GSvector3& position) :
    mesh_{ Mesh_Enemy, Mesh_Enemy, Mesh_Enemy, MotionIdle },
    motion_{ MotionIdle },
    motion_loop_{ true },
    state_{ State::Idle },
    state_timer_{ 0.0f },
    player_{ nullptr } {
    // ���[���h�̐ݒ�
    world_ = world;
    // �^�O���̐ݒ�
    tag_ = "EnemyTag";
    // ���O�̐ݒ�
    name_ = "Enemy";
    // �Փ˔��苅�̐ݒ�
    collider_ = BoundingSphere{ EnemyRadius, GSvector3{0.0f, EnemyHeight, 0.0f} };
    // ���W�̏�����
    transform_.position(position);
    // ���b�V���̕ϊ��s���������
    mesh_.transform(transform_.localToWorldMatrix());

    // �v���[���[����������
    Actor* player = world_->find_actor("Player");
    if (player != nullptr) {
       angle_player_ = player->transform().position();
       angle_player_.y = FootOffset;
       transform().lookAt(angle_player_);
    }
}

// �X�V
void Enemy::update(float delta_time) {
    // �v���[���[����������
    player_ = world_->find_actor("Player");
    // ��Ԃ̍X�V
    update_state(delta_time);
    // �d�͂��X�V
    velocity_.y += Gravity * delta_time;
    // �d�͂�������
    transform_.translate(0.0f, velocity_.y, 0.0f);
    // �t�B�[���h�Ƃ̏Փ˔���
    collide_field();
    // ���[�V������ύX
    mesh_.change_motion(motion_, motion_loop_);
    // ���b�V�����X�V
    mesh_.update(delta_time);
    // ���[���h�s���ݒ�
    mesh_.transform(transform_.localToWorldMatrix());

}

// �`��
void Enemy::draw() const {
    // ���b�V���̕`��
    mesh_.draw();
    // �Փ˔���̃f�o�b�O�\��
    collider().draw();
}

// �Փˏ���
void Enemy::react(Actor& other) {
    // �_���[�W���܂��̓_�E�����̏ꍇ�͉������Ȃ�
    if (state_ == State::Damage || state_ == State::Down || state_ == State::Disappear) return;
    // �v���[���[�̒e�ɏՓ˂���
    if (other.tag() == "PlayerTag" || other.tag() == "PlayerAttackTag") {
       // �c��̗̑͂��Ȃ���΃_�E����ԂɑJ��
       change_state(State::Damage, MotionDamage, false);
       return;
    }
}

// ��Ԃ̍X�V
void Enemy::update_state(float delta_time) {
    // �e��Ԃɕ��򂷂�
    switch (state_) {
    case State::Idle:   idle(delta_time);   break;
    case State::Walk:   walk(delta_time);   break;
    case State::Damage: damage(delta_time); break;
    case State::Disappear: disappear(delta_time); break;
    case State::Down:   down(delta_time);   break;
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
    // �O�i����i���[�J�����W��j
    transform_.translate(0.0f, 0.0f, WalkSpeed * delta_time);
}

// �_���[�W��
void Enemy::damage(float delta_time) {
    enable_collider_ = false;
    // �_���[�W���[�V���������H
    if (state_timer_ < mesh_.motion_end_time()) {
        // �m�b�N�o�b�N����
        transform_.translate(velocity_ * delta_time, GStransform::Space::World);
        velocity_ -= GSvector3{ velocity_.x, 0.0f, velocity_.z } *0.5f * delta_time;
        return;
    }

    change_state(State::Down, MotionDown, false);
}

void Enemy::down(float delta_time){
    if (state_timer_ >= mesh_.motion_end_time()) {
        // �_�E�����[�V�������I�������玀�S 
        die();
    }
}

void Enemy::disappear(float delta_time){
    enable_collider_ = false;
    if (state_timer_ >= mesh_.motion_end_time()) {
        // ���Ń��[�V�������I�������玀�S 
        die();
    }
}

// �ړ�����
bool Enemy::is_walk() const {
    // �ړ����������O�����ƑO���������̃x�N�g���ƃ^�[�Q�b�g�����̃x�N�g���̊p�x����100.0�x�ȉ����H
    return (target_distance() <= WalkDistance);
}

// �^�[�Q�b�g�Ƃ̋��������߂�
float Enemy::target_distance() const {
    // �^�[�Q�b�g�����Ȃ���΍ő勗����Ԃ�
    if (player_ == nullptr) return FLT_MAX; // float�^�̍ő�l
    // �^�[�Q�b�g�Ƃ�2�_�Ԃ̋�����Ԃ�
    return GSvector3::distance(player_->transform().position(), transform_.position());
}

void Enemy::collide_field(){
    // �ǂƂ̏Փ˔���i���̂Ƃ̔���j
    GSvector3 center; // �Փˌ�̋��̂̒��S���W
    if (world_->field()->collide(collider(), &center)) {
        // y���W�͕ύX���Ȃ�
        center.y = transform_.position().y;
        // �␳��̍��W�ɕύX����
        transform_.position(center);
    }
    // �n�ʂƂ̏Փ˔���i�����Ƃ̌�������j
    GSvector3 position = transform_.position();
    Line line;
    line.start = position + collider_.center;
    line.end = position + GSvector3{ 0.0f, -FootOffset, 0.0f };
    GSvector3 intersect;  // �n�ʂƂ̌�_
    if (world_->field()->collide(line, &intersect)) {
        // ���������_����y���W�̂ݕ␳����
        position.y = intersect.y;
        // ���W��ύX����
        transform_.position(position);
        // �d�͂�����������
        velocity_.y = 0.0f;
    }

    if ( state_ != State::Disappear && ( world_->field()->ground_min >= transform_.position() 
        || world_->field()->ground_max <= transform_.position() || transform_.position().z >= 20.0f)) {
        //�J��
        change_state(State::Disappear, MotionDisappear, false);
    }
}

void Enemy::collide_actor(Actor& other){
    // �����W���������W�����߂�
    GSvector3 position = transform_.position();
    position.y = 0.0f;
    GSvector3 target = other.transform().position();
    target.y = 0.0f;
    // ����Ƃ̋���
    float distance = GSvector3::distance(position, target);
    // �Փ˔��苅�̔��a���m�����������������߂�
    float length = collider_.radius + other.collider().radius;
    // �Փ˔��苅�̏d�Ȃ��Ă��钷�������߂�
    float overlap = length - distance;
    // �d�Ȃ��Ă��镔���̔����̋������������ړ��ʂ����߂�
    GSvector3 v = (position - target).getNormalized() * overlap * 0.5f;
    transform_.translate(v, GStransform::Space::World);
    // �t�B�[���h�Ƃ̏Փ˔���
    collide_field();

}
