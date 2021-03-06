#include "Timer.h"
#include "../NumberTexture/NumberTexture.h"
#include "../AssetsID/Assets.h"
#include <gslib.h>
#include <sstream>
#include <iomanip>

// コンストラクタ
Timer::Timer(float time) : time_{ time } {
}

// 初期化
void Timer::initialize(float time) {
    time_ = time;
}

// 更新
void Timer::update(float delta_time) {
    // 秒単位で更新、ゼロ以下にならないように制限する
    time_ = MAX(time_ - (delta_time / 60.0f), 0.0f);
}

// 描画
void Timer::draw() const {
    // 5秒以下になると、0.2秒おきに赤く点滅する
    GScolor color{ 1.0f, 1.0f, 1.0f, 1.0f };
    if (!is_timeout() && time_ <= 5.0f && std::fmod(time_, 0.4f) <= 0.2f) {
        color = GScolor{ 1.0f, 0.0f, 0.0f, 1.0f }; // 赤色
    }
    // 「時間」の文字表示
    static const GSvector2 jikan_position{ 700.0f, 20.0f };
    static const GSrect    jikan_rect{ 0.0f, 0.0f, 128.0f, 64.0f };
    gsDrawSprite2D(Texture_Text, &jikan_position, &jikan_rect, NULL, &color, NULL, 0.0f);

    if (!is_timeout()) {
        // タイマ値の表示
        static const NumberTexture number_texture{ Texture_Number, 32, 64 };
        // タイマ値の書式指定(右詰め・4文字・ゼロで埋める・小数点以下は1桁まで）
        std::stringstream ss;
        ss << std::setw(4) << std::setfill('0') << std::fixed << std::setprecision(1) << time_;
        // 数値の表示
        number_texture.draw(GSvector2{ 860.0f, 20.0f }, ss.str(), color);
    }
    else {
        // 「終了」文字の表示
        static const GSrect syuryo_rect{ 0, 128, 128, 192 };
        static const GSvector2 syuryo_position{ 860, 20 };
        gsDrawSprite2D(Texture_Text, &syuryo_position, &syuryo_rect, NULL, NULL, NULL, 0.0f);
    }
}

// 時間切れか？
bool Timer::is_timeout() const {
    return time_ <= 0.0f;
}