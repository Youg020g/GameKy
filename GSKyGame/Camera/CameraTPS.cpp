#include "CameraTPS.h"
#include "../World/IWorld.h"
#include "../Field/Field.h"
#include "../CollisionDetection/Line/Line.h"

// �v���[���[����̑��΍��W
const GSvector3 PlayerOffset{ 0.0f, 2.0f, -4.0f };
// �J�����̒�������̕␳�l
const GSvector3 ReferencePointOffset{ 0.0f, 1.0f, 0.0f };


CameraTPS::CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& target) {
    // ���[���h��ݒ�
    world_ = world;
    // �^�O�̐ݒ�
    tag_ = "CamaraTag";
    // ���O�̐ݒ�
    name_ = "Camera";
    // ���_�̈ʒu��ݒ�
    transform_.position(position);
    // �����_��ݒ�i�����_�̕����Ɍ�����ς���j
    transform_.lookAt(target);

}

void CameraTPS::update(float delta_time) {
    //�v���[���[������
    Actor* player = world_->find_actor("Player");
    if (player == nullptr) return;
    // ���_�̈ʒu�����߂�i�v���[���[�̔w��̍��W�j
    GSvector3 position = PlayerOffset * player->transform().localToWorldMatrix();
    // �����_�̈ʒu�����߂�i�v���[���[�̓����̏����゠����̍��W�j
    GSvector3 at = player->transform().position() + ReferencePointOffset;

    // �t�B�[���h�Ƃ̏Փ˔���
    Line line{ at, position };
    GSvector3 intersect;
    if (world_->field()->collide(line, &intersect)) {
        position = intersect;
    }

    // �X���[�X�_���v�ɂ�銊�炩�ȕ��
    const float SmoothTime{ 12.0f };    // ��ԃt���[����
    const float MaxSpeed{ 1.0f };       // �ړ��X�s�[�h�̍ő�l
    position = GSvector3::smoothDamp(transform_.position(), position, velocity_,
        SmoothTime, MaxSpeed, delta_time);

    // ���_�̈ʒu��ݒ�
    transform_.position(position);
    // �����_��ݒ�i�����_�̕����Ɍ�����ς���j
    transform_.lookAt(at);
}

void CameraTPS::draw() const {
    // ���_�̈ʒu
    GSvector3 eye = transform_.position();
    // �����_�̈ʒu
    GSvector3 at = eye + transform_.forward();
    // �J�����̐ݒ�
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        eye.x, eye.y, eye.z,     // ���_�̈ʒu
        at.x, at.y, at.z,      // �����_�̈ʒu
        0.0f, 1.0f, 0.0f       // ���_�̏����
    );

}
