#ifndef PLAYER_H_
#define PLAYER_H_

#include "../Actor.h"

// プレーヤークラス
class Player : public Actor {
public:
    // コンストラクタ
    Player(IWorld* world, const GSvector3& position);
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
    // 衝突リアクション
    virtual void react(Actor& other) override;
};

#endif
