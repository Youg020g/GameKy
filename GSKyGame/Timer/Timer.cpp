#include "Timer.h"
#include "../NumberTexture/NumberTexture.h"
#include "../AssetsID/Assets.h"
#include <gslib.h>
#include <sstream>
#include <iomanip>

// �R���X�g���N�^
Timer::Timer(float time) : time_{ time } {
}

// ������
void Timer::initialize(float time) {
    time_ = time;
}

// �X�V
void Timer::update(float delta_time) {
    // �b�P�ʂōX�V�A�[���ȉ��ɂȂ�Ȃ��悤�ɐ�������
    time_ = MAX(time_ - (delta_time / 60.0f), 0.0f);
}

// �`��
void Timer::draw() const {
    // 5�b�ȉ��ɂȂ�ƁA0.2�b�����ɐԂ��_�ł���
    GScolor color{ 1.0f, 1.0f, 1.0f, 1.0f };
    if (!is_timeout() && time_ <= 5.0f && std::fmod(time_, 0.4f) <= 0.2f) {
        color = GScolor{ 1.0f, 0.0f, 0.0f, 1.0f }; // �ԐF
    }
    // �u���ԁv�̕����\��
    static const GSvector2 jikan_position{ 700.0f, 20.0f };
    static const GSrect    jikan_rect{ 0.0f, 0.0f, 128.0f, 64.0f };
    gsDrawSprite2D(Texture_Text, &jikan_position, &jikan_rect, NULL, &color, NULL, 0.0f);

    if (!is_timeout()) {
        // �^�C�}�l�̕\��
        static const NumberTexture number_texture{ Texture_Number, 32, 64 };
        // �^�C�}�l�̏����w��(�E�l�߁E4�����E�[���Ŗ��߂�E�����_�ȉ���1���܂Łj
        std::stringstream ss;
        ss << std::setw(4) << std::setfill('0') << std::fixed << std::setprecision(1) << time_;
        // ���l�̕\��
        number_texture.draw(GSvector2{ 860.0f, 20.0f }, ss.str(), color);
    }
    else {
        // �u�I���v�����̕\��
        static const GSrect syuryo_rect{ 0, 128, 128, 192 };
        static const GSvector2 syuryo_position{ 860, 20 };
        gsDrawSprite2D(Texture_Text, &syuryo_position, &syuryo_rect, NULL, NULL, NULL, 0.0f);
    }
}

// ���Ԑ؂ꂩ�H
bool Timer::is_timeout() const {
    return time_ <= 0.0f;
}