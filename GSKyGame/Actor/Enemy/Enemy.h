#ifndef ENEMY_H_
#define ENEMY_H_

#include "../Actor.h"
#include "../../Animation/AnimatedMesh/AnimatedMesh.h"

// 敵クラス
class Enemy : public Actor {
public:
    // 状態を表す列挙型
    enum class State {
        Idle,			// アイドル中
        Walk,			// 歩き中
        Damage,		// ダメージ中
        Down,		// 攻撃中
        Disappear   //消滅中
    };
public:
    // コンストラクタ
    Enemy(IWorld* world, const GSvector3& position);
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
    // 衝突リアクション
    virtual void react(Actor& other) override;

private:
    // 状態の更新
    void update_state(float delta_time);
    // 状態の変更
    void change_state(State state, GSuint motion, bool loop = true);

    // アイドル状態中
    void idle(float delta_time);
    // 移動中
    void walk(float delta_time);
    // ダメージ中
    void damage(float delta_time);
    //ダウン中
    void down(float delta_time);
    //消滅中
    void disappear(float delta_time);

    // 移動判定
    bool is_walk() const;

    //// ターゲット方向の角度を求める（符号付き）
    //float target_signed_angle() const;
    //// ターゲット方向の角度を求める（符号なし）
    //float target_angle() const;
    // ターゲットの距離を求める
    float target_distance() const;

    // フィールドとの衝突
    void collide_field();
    // アクターとの衝突処理
    void collide_actor(Actor& other);

private:
    // アニメーションメッシュ
    AnimatedMesh	mesh_;
    // モーション番号
    GSuint		motion_;
    // モーションのループ指定
    bool             motion_loop_;
    // 状態
    State		state_;
    // 状態タイマ
    float		state_timer_;
    // プレーヤー
    Actor* player_;
    //プレイヤーの向き
   GSvector3 angle_player_;
};

#endif
