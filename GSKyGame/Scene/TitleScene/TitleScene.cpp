#include "TitleScene.h"
#include "../../AssetsID/Assets.h"

// �L�����N�^�[�摜�̈ړ�����
const float MovingTime{ 30.0f };

// �J�n
void TitleScene::start() {
    // �摜�̓ǂݍ���
    gsLoadTexture(Texture_Title, "Assets/texture/title.png");
    gsLoadTexture(Texture_Kendo, "Assets/texture/mini_kendo.png");
    gsLoadTexture(Texture_Karate, "Assets/texture/mini_karate.png");
    gsLoadTexture(Texture_Start, "Assets/texture/osu.png");

    // �I���t���O�̏�����
    is_end_ = false;

    //�^�C�}�̏�����
    player_moving_timer_ = 0.0f;
    enemy_moving_timer_ = 0.0f;
    text_timer_ = 0.0f;
}

// �X�V
void TitleScene::update(float delta_time) {
    // �ړ����Ԃ̍X�V
    player_moving_timer_ = MIN(player_moving_timer_ + delta_time, MovingTime);
    enemy_moving_timer_ = MIN(enemy_moving_timer_ + delta_time, MovingTime);

    //�e�L�X�g�^�C�}�̍X�V
    text_timer_ += delta_time;

    // �G���^�[�L�[����������V�[���I��
    if (gsGetKeyTrigger(GKEY_SPACE)) {
        is_end_ = true;     // �V�[���I��
    }
}

// �`��
void TitleScene::draw() const {
    // �w�i�̕`��
    draw_background(Texture_Title);
    //��蕔��
    draw_karate();
    //��������
    draw_player();

    // ���U���g�J�n�P�b��Ɂu�J�n�{�^�������E�I�v��_�ŕ\������
    if (text_timer_ >= 60.0f && fmod(text_timer_, 40.0f) < 20.0f) {
        static const GSvector2 position{ 250, 700 };
        gsDrawSprite2D(Texture_Start, &position, NULL, NULL, NULL, NULL, 0.0f);
    }
  
}

// ���������̕`��
void TitleScene::draw_player() const {
    // ���������̉摜�̍��W����`��Ԃœ�����
    static const GSvector2 kendo_position_start{ -1024.0f, 100.0f };    // �J�n�ʒu
    static const GSvector2 kendo_position_end{ 0.0f, 100.0f };        // �I���ʒu
    GSvector2 kendo_position = GSvector2::lerp(kendo_position_start,
        kendo_position_end, player_moving_timer_ / MovingTime);
    // �v���[���[�摜�̕`��
    gsDrawSprite2D(Texture_Kendo, &kendo_position, NULL, NULL, NULL, NULL, 0.0f);
}

// ��蕔���̕`��
void TitleScene::draw_karate() const {
    // ��蕔���̉摜�̍��W����`��Ԃœ�����
    static const GSvector2 karate_position_start{ 512.0f, 128.0f };    // �J�n�ʒu
    static const GSvector2 karate_position_end{ -480.0f, 128.0f };        // �I���ʒu
    GSvector2 karate_position = GSvector2::lerp(karate_position_start,
        karate_position_end, enemy_moving_timer_ / MovingTime);
    // ��蕔���摜�̕`��
    gsDrawSprite2D(Texture_Karate, &karate_position, NULL, NULL, NULL, NULL, 0.0f);
}

// �I�����Ă��邩�H
bool TitleScene::is_end() const {
    return is_end_;         // �I���t���O��Ԃ�
}

// ���̃V�[������Ԃ�
std::string TitleScene::next() const {
    return "GamePlayScene"; // ���̃V�[������Ԃ�
}

// �I��
void TitleScene::end() {
    // �摜�̍폜
    gsDeleteTexture(Texture_Title);
    gsDeleteTexture(Texture_Kendo);
    gsDeleteTexture(Texture_Karate);
    gsDeleteTexture(Texture_Start);

}

// �w�i�̕`��
void TitleScene::draw_background(GSuint id) const {
    GSrect rect{ 0.0f, 0.0f,1024.0f, 768.0f };
    gsDrawSprite2D(id, NULL, &rect, NULL, NULL, NULL, 0.0f);
}




