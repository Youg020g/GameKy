#include "AttackCollider.h"

AttackCollider::AttackCollider(IWorld* world, const BoundingSphere& collider, const std::string& tag, const std::string& name,
    const std::string& owner_tag,
    float lifespan, float delay) :
    lifespan_timer_{ lifespan }, delay_timer_{ delay }{
    // ���[���h�̐ݒ�
    world_ = world;
    // �^�O���̐ݒ�
    tag_ = tag;
    // ���O�̐ݒ�
    name_ = name;
    // �Փ˔����������
    collider_ = BoundingSphere{ collider.radius };
    // ���W�̏�����
    transform_.position(collider.center);
    // ���G��Ԃɂ���
    enable_collider_ = false;

}

void AttackCollider::update(float delta_time) {
    // �x�����Ԃ��o�߂�����Փ˔����L���ɂ���
    if (delay_timer_ <= 0.0f) {
        // �Փ˔����L���ɂ���
        enable_collider_ = true;
        // �������s�����玀�S
        if (lifespan_timer_ <= 0.0f) {
            die();
        }
        // �����̍X�V
        lifespan_timer_ -= delta_time;
    }
    // �x�����Ԃ̍X�V
    delay_timer_ -= delta_time;

}

void AttackCollider::draw() const {
    // �Փ˔���̃f�o�b�O�\��
    collider().draw();
}

void AttackCollider::react(Actor& other) {
    // �����Ɠ����^�O�܂��́A�I�[�i�[�Ɠ����^�O�͏Փ˔��肵�Ȃ�
    if (other.tag() == tag() || other.tag() == owner_tag_) return;
    // �Փ˂����玀�S
    die();

}
