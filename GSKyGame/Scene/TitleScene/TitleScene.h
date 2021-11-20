#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

#include "../IScene.h"
#include <gslib.h>

// タイトルシーン
class TitleScene : public IScene {
public:
    // 開始
    virtual void start() override;
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
    // 終了しているか？
    virtual bool is_end() const override;
    // 次のシーンを返す
    virtual std::string next() const override;
    // 終了
    virtual void end() override;

    
private:
    //剣道部長の描画
    void draw_player()const;
    //空手部長の描画
    void draw_karate() const;
    // 背景の描画
    void draw_background(GSuint id) const;

    // 終了フラグ
    bool is_end_{ false };
    // 剣道部長移動用タイマ
    float player_moving_timer_{ 0.0f };
    // 空手部長移動用タイマ
    float enemy_moving_timer_{ 0.0f };
    //テキスト点滅用タイマ
    float   text_timer_{ 0.0f };

};

#endif // !TITLE_SCENE_H_


