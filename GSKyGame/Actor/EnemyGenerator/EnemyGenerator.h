#ifndef ENEMY_GENERATOR_H_
#define ENEMY_GENERATOR_H_

#include "../Actor.h"

// �G�����N���X
class EnemyGenerator : public Actor {
public:
    // �R���X�g���N�^
    EnemyGenerator(IWorld* world);
    // �X�V
    virtual void update(float delta_time) override;
    // �`��
    virtual void draw() const override;
private:
    //��������
    float time_{ 30.0f };
    // �������ԃ^�C�}
    float timer_{ 0.0f };
    //�����̍ő吔
    int enemy_num_max_{ 20 };
};

#endif