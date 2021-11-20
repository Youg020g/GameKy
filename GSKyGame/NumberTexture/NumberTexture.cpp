#include "NumberTexture.h"
#include <sstream>
#include <iomanip>

// �R���X�g���N�^
NumberTexture::NumberTexture(GSuint texture, int width, int height) :
    texture_{ texture }, width_{ width }, height_{ height } {
}

// �`��
void NumberTexture::draw(const GSvector2& position, int num, int digit, char fill, const GScolor& color) const {
    std::stringstream ss;
    ss << std::setw(digit) << std::setfill(fill) << num;
    draw(position, ss.str(), color);
}

// �`��
void NumberTexture::draw(const GSvector2& position, int num, const GScolor& color) const {
    draw(position, std::to_string(num), color);
}

// �`��
void NumberTexture::draw(const GSvector2& position, const std::string& num, const GScolor& color) const {
    for (int i = 0; i < (int)num.size(); ++i) {
        if (num[i] == ' ') continue;
        const int n = (num[i] != '.') ? num[i] - '0' : 10; // �����_��9�̎��ɂ��邱�Ƃ�O��
        const GSrect rect(n * width_, 0.0f, (n * width_) + width_, height_);
        const GSvector2 pos{ position.x + i * width_, position.y };
        gsDrawSprite2D(texture_, &pos, &rect, NULL, &color, NULL, 0);
    }
}

void NumberTexture::score_text_texture (const GSvector2& position, const GScolor& color) const {
    // �����ɑΉ�����摜��؂�o�����߂̋�`���v�Z����
    GSrect rect(0.0f, 64.0f, 128.0f, 128.0f);
    // �������P���`��
    gsDrawSprite2D(texture_, &position, &rect, NULL, &color, NULL, 0);
}
