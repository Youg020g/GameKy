#include "Player.h"
#include "../../World/IWorld.h"
#include "../../Field/Field.h"
#include "../../CollisionDetection/Line/Line.h"
#include "../../AssetsID/Assets.h"
#include "../AttackCollider/AttackCollider.h"

// ���[�V�����ԍ�
enum {
    MotionIdle = 0,     // �A�C�h��
    MotionWalk = 1,     // �O�i
    MotionDamage = 14,    // �_���\�W
    MotionGetUp = 16,     // �N���オ��
    MotionAttack = 17    // �U��
};

// �ړ����x
const float WalkSpeed{ 0.25f };
// �����̍���
const float PlayerHeight{ 1.0f };
// �Փ˔���p�̔��a
const float PlayerRadius{ 0.5f };
// �����̃I�t�Z�b�g
const float FootOffset{ 0.1f };
// �d�͒l
const float Gravity{ -0.016f };


// �R���X�g���N�^
Player::Player(IWorld* world, const GSvector3& position) :
    mesh_{ 0, 0, 0, MotionIdle },
    motion_{ MotionIdle },
    motion_loop_{ true },
    state_{ State::Move },
    state_timer_{ 0.0f } {
    // ���[���h��ݒ�
    world_ = world;
    // �^�O���̐ݒ�
    tag_ = "PlayerTag";
    // ���O�̐ݒ�
    name_ = "Player";
    // �Փ˔��苅�̐ݒ�
    collider_ = BoundingSphere{ PlayerRadius, GSvector3{0.0f, PlayerHeight, 0.0f} };
    // ���W�̏�����
    transform_.position(position);
    // ���b�V���̕ϊ��s���������
    mesh_.transform(transform_.localToWorldMatrix());
}

// �X�V
void Player::update(float delta_time) {
    // ��Ԃ̍X�V
    update_state(delta_time);
    // �d�͒l���X�V
    velocity_.y += Gravity * delta_time;
    // �d�͂�������
    transform_.translate(0.0f, velocity_.y, 0.0f);
    // �t�B�[���h�Ƃ̏Փ˔���
    collide_field();
    // ���[�V������ύX
    mesh_.change_motion(motion_, motion_loop_);
    // ���b�V�����X�V
    mesh_.update(delta_time);
    // �s���ݒ�
    mesh_.transform(transform_.localToWorldMatrix());

}

// �`��
void Player::draw() const {
    // ���b�V����`��
    mesh_.draw();
    // �Փ˔��苅�̃f�o�b�O�\��
    collider().draw();
}

// �Փ˃��A�N�V����
void Player::react(Actor& other) {
    // �_���[�W���ƋN���オ�蒆�͉������Ȃ�
    if (state_ == State::Damage || state_ == State::GetUp) return;
    // �G�ƏՓ˂������H
    if (other.tag() == "EnemyTag") {
        // �Փ˂����ꍇ�́A�_���[�W��ԂɕύX
        change_state(State::Damage, MotionDamage, false);
        return;
    }

   
}

// ��Ԃ̍X�V
void Player::update_state(float delta_time) {
    // ��ԑJ��
    switch (state_) {
    case State::Move:   move(delta_time);   break;
    case State::Attack: attack(delta_time); break;
    case State::Damage: damage(delta_time); break;
    case State::GetUp: get_up(delta_time); break;
    }
    // ��ԃ^�C�}�̍X�V
    state_timer_ += delta_time;
}

// ��Ԃ̕ύX
void Player::change_state(State state, GSuint motion, bool loop) {
    motion_ = motion;
    motion_loop_ = loop;
    state_ = state;
    state_timer_ = 0.0f;
}

// �ړ�����
void Player::move(float delta_time) {
    // �X�y�[�X�L�[�ōU��
    if (gsGetKeyState(GKEY_SPACE)) {
        // �U������𐶐�
        generate_attack_collider();
        change_state(State::Attack, MotionAttack);
        return;
    }
    // �������Ȃ���΃A�C�h�����
    GSint motion{ MotionIdle };
    // �O��ړ�����Ƃ��̑���
    float forward_speed{ 0.0f };
    // ���E�ړ�����Ƃ��̑���
    float side_speed{ 0.0f };
    // WASD�ړ�
    if (gsGetKeyState(GKEY_W)) {
        forward_speed = WalkSpeed;      // �O�i
        motion = MotionWalk;
    }
    else if (gsGetKeyState(GKEY_S)) {
        forward_speed = -WalkSpeed;     // ���
        motion = MotionWalk;
    }
    else if (gsGetKeyState(GKEY_A)) {
        side_speed = WalkSpeed;        // ���ړ�
        motion = MotionWalk;
    }
    else if (gsGetKeyState(GKEY_D)) {
        side_speed = -WalkSpeed;        // �E�ړ�
        motion = MotionWalk;
    }
    
    // �ړ���Ԃɂ���
    change_state(State::Move, motion);

    // �}�E�X�̍��E�ړ��ŕ�����ς���
    int mx, my, mz;
    gsGetMouseVelocity(&mx, &my, &mz);
    float yaw = (float)-mx * 0.5f;
    transform_.rotate(0.0f, yaw * delta_time, 0.0f);

    // ���s�ړ�����
    transform_.translate(side_speed * delta_time, 0.0f, forward_speed * delta_time);

}

// �U����
void Player::attack(float delta_time) {
    // �U�����[�V�����̏I����҂�
    if (state_timer_ >= mesh_.motion_end_time()) {
        move(delta_time);
    }
}

// �_���[�W��
void Player::damage(float delta_time) {
    // �_���[�W���[�V�����̏I����҂�
    if (state_timer_ >= mesh_.motion_end_time()) {
        move(delta_time);
    }
    // �Փ˂����ꍇ�́A�N���オ���ԂɕύX
    change_state(State::GetUp, MotionGetUp, false);
}

void Player::get_up(float delta_time){
    // �N���オ�胂�[�V�����̏I����҂�
    if (state_timer_ >= mesh_.motion_end_time() ) {
        move(delta_time);
    }
}

// �t�B�[���h�Ƃ̏Փ˔���
void Player::collide_field() {
    
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
        // ��_�̈ʒu����y���W�̂ݕ␳����
        position.y = intersect.y;
        // ���W��ύX����
        transform_.position(position);
        // �d�͂�����������
        velocity_.y = 0.0f;
    }

    //�O���E���h�O�ɏo�Ȃ��悤�ɃN�����v
    position = position.clamp(world_->field()->ground_min, world_->field()->ground_max);
    // ���W��ύX����
    transform_.position(position);
}

// �A�N�^�[�Ƃ̏Փˏ���
void Player::collide_actor(Actor& other) {
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

// �U������𐶐�����
void Player::generate_attack_collider() {
    // �U��������o��������ꏊ�̋���
    const float AttackColliderDistance{ 1.5f };
    // �U������̔��a
    const float AttackColliderRadius{ 1.0f };
    // �U��������o���ꏊ�̍���
    const float AttackColliderHeight{ 1.0f };

    // �U�����肪�L���ɂȂ�܂ł̒x������
    const float AttackCollideDelay{ 15.0f };
    // �U������̎���
    const float AttackCollideLifeSpan{ 5.0f };

    // �Փ˔�����o����������W�����߂�i�O���̈ʒu�j
    GSvector3 position = transform_.position() + transform_.forward() * AttackColliderDistance;
    // �����̕␳�i��������̍����j
    position.y += AttackColliderHeight;
    // �Փ˔���p�̋����쐬
    BoundingSphere collider{ AttackColliderRadius, position };
    // �Փ˔�����o��������
    world_->add_actor(new AttackCollider{ world_, collider,
        "PlayerAttackTag", "PlayerAttack", tag_, AttackCollideLifeSpan, AttackCollideDelay });
}