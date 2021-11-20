#include "Score.h"
#include "../NumberTexture/NumberTexture.h"
#include "../AssetsID/Assets.h"
#include <gslib.h>
#include <algorithm> // std::min関数のために必要

// コンストラクタ
Score::Score(int score) :
    score_{ score } {
}

// スコアの初期化
void Score::initialize(int score) {
    score_ = score;
}

// スコアの加算
void Score::add(int score) {
    // スコアの上限値を9999999とする
    score_ = std::min(score_ + score, 999);
}

// スコアの描画
void Score::draw() const {
    static const NumberTexture number{ Texture_Number, 32, 64 };
    static const NumberTexture score_text{ Texture_Text,32,64 };
    number.draw(GSvector2{ 500.0f, 20.0f }, score_, 3);
    score_text.score_text_texture(GSvector2{ 356.0f, 20.0f });
}

// スコアの取得
int Score::get() const {
    return score_;
}


// スコアのクリア
void Score::clear() {
    score_ = 0;
}