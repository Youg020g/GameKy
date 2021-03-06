#ifndef CAMERA_TPS_H_
#define CAMERA_TPS_H_

#include "../Actor/Actor.h"

class CameraTPS : public Actor {
public:
    // コンストラクタ
    CameraTPS(IWorld* world, const GSvector3& position, const GSvector3& at);
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
};

#endif // !CAMERA_TPS_H_