#ifndef PLAYER_H_
#define PLAYER_H_

#include "../Actor.h"
#include "../../Animation/AnimatedMesh/AnimatedMesh.h"

// �v���[���[�N���X
class Player : public Actor {
public:
    // �v���[���[�̏��
    enum class State {
        Move,			// �ړ���
        Attack,		// �U����
        Damage,		// �_���[�W��
        Down,        //�_�E��
        GetUp       //�N���オ�蒆
    };

public:
    // �R���X�g���N�^
    Player(IWorld* world = nullptr, const GSvector3& position = GSvector3{ 0.0f, 0.0f, 0.0f });
    // �X�V
    void update(float delta_time) override;
    // �`��
    void draw() const override;
    // �Փ˃��A�N�V����
    virtual void react(Actor& other) override;

private:
    // ��Ԃ̍X�V
    void update_state(float delta_time);
    // ��Ԃ̕ύX
    void change_state(State state, GSuint motion, bool loop = true);
    // �ړ�����
    void move(float delta_time);
    // �U����
    void attack(float delta_time);
    // �_���[�W��
    void damage(float delta_time);
    //�N���オ�蒆
    void get_up(float delta_time);

    // �t�B�[���h�Ƃ̏Փˏ���
    void collide_field();
    // �A�N�^�[�Ƃ̏Փˏ���
    void collide_actor(Actor& other);
    // �U������𐶐�
    void generate_attack_collider();


private:
    // �A�j���[�V�������b�V��
    AnimatedMesh	mesh_;
    // ���[�V�����ԍ�
    GSuint		motion_;
    // ���[�V�����̃��[�v�w��
    bool		motion_loop_;
    // ���
    State		state_;
    // ��ԃ^�C�}
    float		state_timer_;
};

#endif