#include "Score.h"
#include "../NumberTexture/NumberTexture.h"
#include "../AssetsID/Assets.h"
#include <gslib.h>
#include <algorithm> // std::min�֐��̂��߂ɕK�v

// �R���X�g���N�^
Score::Score(int score) :
    score_{ score } {
}

// �X�R�A�̏�����
void Score::initialize(int score) {
    score_ = score;
}

// �X�R�A�̉��Z
void Score::add(int score) {
    // �X�R�A�̏���l��9999999�Ƃ���
    score_ = std::min(score_ + score, 999);
}

// �X�R�A�̕`��
void Score::draw() const {
    static const NumberTexture number{ Texture_Number, 32, 64 };
    static const NumberTexture score_text{ Texture_Text,32,64 };
    number.draw(GSvector2{ 500.0f, 20.0f }, score_, 3);
    score_text.score_text_texture(GSvector2{ 356.0f, 20.0f });
}

// �X�R�A�̎擾
int Score::get() const {
    return score_;
}


// �X�R�A�̃N���A
void Score::clear() {
    score_ = 0;
}