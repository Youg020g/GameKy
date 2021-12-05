#ifndef RADAR_H_
#define RADAR_H_

#include "../Actor.h"

// ���[�_�[�N���X
class Radar : public Actor {
public:
    // �R���X�g���N�^
    Radar(IWorld* world);
    // �`��
    void draw_gui() const override;

private:
    // ���[�_�[�̓_��`��
    void draw_point(const GSvector3& position, const GScolor& color) const;
};

#endif
