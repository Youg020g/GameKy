#ifndef PLAYER_H_
#define PLAYER_H_

#include "../Actor.h"
#include "../../Animation/AnimatedMesh/AnimatedMesh.h"

// プレーヤークラス
class Player : public Actor {
public:
    // プレーヤーの状態
    enum class State {
        Move,			// 移動中
        Attack,		// 攻撃中
        Damage		// ダメージ中
    };

public:
    // コンストラクタ
    Player(IWorld* world = nullptr, const GSvector3& position = GSvector3{ 0.0f, 0.0f, 0.0f });
    // 更新
    void update(float delta_time) override;
    // 描画
    void draw() const override;
    // 衝突リアクション
    virtual void react(Actor& other) override;

private:
    // 状態の更新
    void update_state(float delta_time);
    // 状態の変更
    void change_state(State state, GSuint motion, bool loop = true);
    // 移動処理
    void move(float delta_time);
    // 攻撃中
    void attack(float delta_time);
    // ダメージ中
    void damage(float delta_time);

    // フィールドとの衝突処理
    void collide_field();

private:
    // アニメーションメッシュ
    AnimatedMesh	mesh_;
    // モーション番号
    GSuint		motion_;
    // モーションのループ指定
    bool		motion_loop_;
    // 状態
    State		state_;
    // 状態タイマ
    float		state_timer_;
};

#endif