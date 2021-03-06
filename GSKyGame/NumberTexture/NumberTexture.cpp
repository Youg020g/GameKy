#include "NumberTexture.h"
#include <sstream>
#include <iomanip>

// コンストラクタ
NumberTexture::NumberTexture(GSuint texture, int width, int height) :
    texture_{ texture }, width_{ width }, height_{ height } {
}

// 描画
void NumberTexture::draw(const GSvector2& position, int num, int digit, char fill, const GScolor& color) const {
    std::stringstream ss;
    ss << std::setw(digit) << std::setfill(fill) << num;
    draw(position, ss.str(), color);
}

// 描画
void NumberTexture::draw(const GSvector2& position, int num, const GScolor& color) const {
    draw(position, std::to_string(num), color);
}

// 描画
void NumberTexture::draw(const GSvector2& position, const std::string& num, const GScolor& color) const {
    for (int i = 0; i < (int)num.size(); ++i) {
        if (num[i] == ' ') continue;
        const int n = (num[i] != '.') ? num[i] - '0' : 10; // 小数点は9の次にあることを前提
        const GSrect rect(n * width_, 0.0f, (n * width_) + width_, height_);
        const GSvector2 pos{ position.x + i * width_, position.y };
        gsDrawSprite2D(texture_, &pos, &rect, NULL, &color, NULL, 0);
    }
}

void NumberTexture::score_text_texture (const GSvector2& position, const GScolor& color) const {
    // 数字に対応する画像を切り出すための矩形を計算する
    GSrect rect(0.0f, 64.0f, 128.0f, 128.0f);
    // 数字を１桁描画
    gsDrawSprite2D(texture_, &position, &rect, NULL, &color, NULL, 0);
}
