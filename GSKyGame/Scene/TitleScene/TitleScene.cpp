#include "TitleScene.h"
#include "../../AssetsID/Assets.h"

// キャラクター画像の移動時間
const float MovingTime{ 30.0f };

// 開始
void TitleScene::start() {
    // 画像の読み込み
    gsLoadTexture(Texture_Title, "Assets/texture/title.png");
    gsLoadTexture(Texture_Kendo, "Assets/texture/mini_kendo.png");
    gsLoadTexture(Texture_Karate, "Assets/texture/mini_karate.png");
    gsLoadTexture(Texture_Start, "Assets/texture/osu.png");

    // 終了フラグの初期化
    is_end_ = false;

    //タイマの初期化
    player_moving_timer_ = 0.0f;
    enemy_moving_timer_ = 0.0f;
    text_timer_ = 0.0f;
}

// 更新
void TitleScene::update(float delta_time) {
    // 移動時間の更新
    player_moving_timer_ = MIN(player_moving_timer_ + delta_time, MovingTime);
    enemy_moving_timer_ = MIN(enemy_moving_timer_ + delta_time, MovingTime);

    //テキストタイマの更新
    text_timer_ += delta_time;

    // エンターキーを押したらシーン終了
    if (gsGetKeyTrigger(GKEY_SPACE)) {
        is_end_ = true;     // シーン終了
    }
}

// 描画
void TitleScene::draw() const {
    // 背景の描画
    draw_background(Texture_Title);
    //空手部長
    draw_karate();
    //剣道部長
    draw_player();

    // リザルト開始１秒後に「開始ボタンを押忍！」を点滅表示する
    if (text_timer_ >= 60.0f && fmod(text_timer_, 40.0f) < 20.0f) {
        static const GSvector2 position{ 250, 700 };
        gsDrawSprite2D(Texture_Start, &position, NULL, NULL, NULL, NULL, 0.0f);
    }
  
}

// 剣道部長の描画
void TitleScene::draw_player() const {
    // 剣道部長の画像の座標を線形補間で動かす
    static const GSvector2 kendo_position_start{ -1024.0f, 100.0f };    // 開始位置
    static const GSvector2 kendo_position_end{ 0.0f, 100.0f };        // 終了位置
    GSvector2 kendo_position = GSvector2::lerp(kendo_position_start,
        kendo_position_end, player_moving_timer_ / MovingTime);
    // プレーヤー画像の描画
    gsDrawSprite2D(Texture_Kendo, &kendo_position, NULL, NULL, NULL, NULL, 0.0f);
}

// 空手部長の描画
void TitleScene::draw_karate() const {
    // 空手部長の画像の座標を線形補間で動かす
    static const GSvector2 karate_position_start{ 512.0f, 128.0f };    // 開始位置
    static const GSvector2 karate_position_end{ -480.0f, 128.0f };        // 終了位置
    GSvector2 karate_position = GSvector2::lerp(karate_position_start,
        karate_position_end, enemy_moving_timer_ / MovingTime);
    // 空手部長画像の描画
    gsDrawSprite2D(Texture_Karate, &karate_position, NULL, NULL, NULL, NULL, 0.0f);
}

// 終了しているか？
bool TitleScene::is_end() const {
    return is_end_;         // 終了フラグを返す
}

// 次のシーン名を返す
std::string TitleScene::next() const {
    return "GamePlayScene"; // 次のシーン名を返す
}

// 終了
void TitleScene::end() {
    // 画像の削除
    gsDeleteTexture(Texture_Title);
    gsDeleteTexture(Texture_Kendo);
    gsDeleteTexture(Texture_Karate);
    gsDeleteTexture(Texture_Start);

}

// 背景の描画
void TitleScene::draw_background(GSuint id) const {
    GSrect rect{ 0.0f, 0.0f,1024.0f, 768.0f };
    gsDrawSprite2D(id, NULL, &rect, NULL, NULL, NULL, 0.0f);
}




