#ifndef ENEMY_H_
#define ENEMY_H_

#include "../Actor.h"
#include "../../Animation/AnimatedMesh/AnimatedMesh.h"

// �G�N���X
class Enemy : public Actor {
public:
    // ��Ԃ�\���񋓌^
    enum class State {
        Idle,			// �A�C�h����
        Walk,			// ������
        Turn,			// �U�������
        Damage,		// �_���[�W��
        Attack		// �U����
    };
public:
    // �R���X�g���N�^
    Enemy(IWorld* world, const GSvector3& position, Actor* player);
    // �X�V
    virtual void update(float delta_time) override;
    // �`��
    virtual void draw() const override;
    // �Փ˃��A�N�V����
    virtual void react(Actor& other) override;

private:
    // ��Ԃ̍X�V
    void update_state(float delta_time);
    // ��Ԃ̕ύX
    void change_state(State state, GSuint motion, bool loop = true);

    // �A�C�h����Ԓ�
    void idle(float delta_time);
    // �ړ���
    void walk(float delta_time);
    // �_���[�W��
    void damage(float delta_time);

    // �ړ�����
    bool is_walk() const;

    // �^�[�Q�b�g�����̊p�x�����߂�i�����t���j
    float target_signed_angle() const;
    // �^�[�Q�b�g�����̊p�x�����߂�i�����Ȃ��j
    float target_angle() const;
    // �^�[�Q�b�g�̋��������߂�
    float target_distance() const;

private:
    // �A�j���[�V�������b�V��
    AnimatedMesh	mesh_;
    // ���[�V�����ԍ�
    GSuint		motion_;
    // ���[�V�����̃��[�v�w��
    bool             motion_loop_;
    // ���
    State		state_;
    // ��ԃ^�C�}
    float		state_timer_;
    // �v���[���[
    Actor* player_;
};

#endif
