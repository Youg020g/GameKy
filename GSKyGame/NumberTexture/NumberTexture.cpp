#include "NumberTexture.h"
#include <sstream>
#include <iomanip>

// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
NumberTexture::NumberTexture(GSuint texture, int width, int height) :
    texture_{ texture }, width_{ width }, height_{ height } {
}

// •`‰æ
void NumberTexture::draw(const GSvector2& position, int num, int digit, char fill, const GScolor& color) const {
    std::stringstream ss;
    ss << std::setw(digit) << std::setfill(fill) << num;
    draw(position, ss.str(), color);
}

// •`‰æ
void NumberTexture::draw(const GSvector2& position, int num, const GScolor& color) const {
    draw(position, std::to_string(num), color);
}

// •`‰æ
void NumberTexture::draw(const GSvector2& position, const std::string& num, const GScolor& color) const {
    for (int i = 0; i < (int)num.size(); ++i) {
        if (num[i] == ' ') continue;
        const int n = (num[i] != '.') ? num[i] - '0' : 10; // ¬”“_‚Í9‚ÌŸ‚É‚ ‚é‚±‚Æ‚ğ‘O’ñ
        const GSrect rect(n * width_, 0.0f, (n * width_) + width_, height_);
        const GSvector2 pos{ position.x + i * width_, position.y };
        gsDrawSprite2D(texture_, &pos, &rect, NULL, &color, NULL, 0);
    }
}

void NumberTexture::score_text_texture (const GSvector2& position, const GScolor& color) const {
    // ”š‚É‘Î‰‚·‚é‰æ‘œ‚ğØ‚èo‚·‚½‚ß‚Ì‹éŒ`‚ğŒvZ‚·‚é
    GSrect rect(0.0f, 64.0f, 128.0f, 128.0f);
    // ”š‚ğ‚PŒ…•`‰æ
    gsDrawSprite2D(texture_, &position, &rect, NULL, &color, NULL, 0);
}
