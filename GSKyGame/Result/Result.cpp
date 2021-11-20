#include "Result.h"
#include "../NumberTexture/NumberTexture.h"
#include "../AssetsID/Assets.h"
#include <gslib.h>
#include <fstream> // �t�@�C������ɕK�v

// �����L���O��
const int RankingCount{ 5 };
// �v���[���[�̈ړ�����
const float MovingTime{ 30.0f };

// �R���X�g���N�^
Result::Result() {
    // ����������
    initialize();
}

// ������
void Result::initialize() {
    // �v���[���[�ړ��p�^�C�}�̏�����
    moving_timer_ = 0.0f;
    // �ŏI�X�R�A�̏�����
    final_score_ = 0;
    // �ŏI�����N�̏�����
    final_rank_ = 0;
    // �����L���O�̏�����
    ranking_scores_.resize(RankingCount, 0);
}

// �X�R�A�̒ǉ�
void Result::add_score(int value) {
    // �X�R�A�̐ݒ�
    final_score_ = value;
    // ���ʂ����߂�
    final_rank_ = 0;
    for (int score : ranking_scores_) {
        // �ǉ����链�_�����_���̍����X�R�A�𐔂���
        if (final_score_ <= score) {
            ++final_rank_;
        }
    }
    // �����N�C���������H
    if (final_rank_ < (int)ranking_scores_.size()) {
        // �V�����X�R�A�̑}��
        ranking_scores_.insert(ranking_scores_.begin() + final_rank_, final_score_);
        // �ŉ��ʂ̃X�R�A���폜
        ranking_scores_.pop_back();
    }
}

// �X�V
void Result::update(float delta_time) {
    // �ړ����Ԃ̍X�V
    moving_timer_ = MIN(moving_timer_ + delta_time, MovingTime);
}

// �`��
void Result::draw() const {
    // �w�i��`��
    draw_background();
    // ���������̕\��
    draw_player();
    // �]�����b�Z�[�W�̕\��
    draw_grade_message();
    // �����L���O�̕\��
    draw_ranking();
}

// �����L���O�t�@�C���̕ۑ�
void Result::save(const std::string& file_name) const {
    // �t�@�C���̃I�[�v��
    std::ofstream file{ file_name };
    // �t�@�C�����I�[�v���ł��Ȃ������牽�����Ȃ�
    if (!file) return;
    // �X�R�A�̕ۑ�
    for (int score : ranking_scores_) {
        file << score << std::endl;
    }
}

// �����L���O�t�@�C���̓ǂݍ���
void Result::load(const std::string& file_name) {
    // �t�@�C���̃I�[�v��
    std::ifstream file{ file_name };
    // �t�@�C�����I�[�v���ł��Ȃ������牽�����Ȃ�
    if (!file) return;
    // �X�R�A�̓ǂݍ���
    for (int& score : ranking_scores_) {
        file >> score;
    }
}

// �w�i�̕`��
void Result::draw_background() const {
    // �������̐��w�i��\�� 
    static const GScolor bg_color{ 1.0f, 1.0f, 1.0f, 0.5f };
    static const GSvector2 bg_position{ 20.0f, 100.0f };
    gsDrawSprite2D(Texture_BlueBack, &bg_position, NULL, NULL, &bg_color, NULL, 0.0f);
}

// �v���[���[�̕`��
void Result::draw_player() const {
    // ���������̉摜�̍��W����`��Ԃœ�����
    static const GSvector2 kendo_position_start{ -1024.0f, 100.0f };    // �J�n�ʒu
    static const GSvector2 kendo_position_end{ 370.0f, 100.0f };        // �I���ʒu
    GSvector2 kendo_position = GSvector2::lerp(kendo_position_start,
        kendo_position_end, moving_timer_ / MovingTime);
    // �v���[���[�摜�̕`��
    gsDrawSprite2D(Texture_Kendo, &kendo_position, NULL, NULL, NULL, NULL, 0.0f);
}

// �����L���O�̕`��
void Result::draw_ranking() const {
    // �u���ʁv�̕�����\��
    static const GSrect    jyuni_rect{ 0, 192, 128, 256 };        // �u���ʁv�̐؂�o���ʒu
    static const GSvector2 jyuni_position{ 40.0f, 360.0f };       // �u���ʁv�̕\�����W
    static const GScolor   jyuni_color{ 1.0f, 0.0f, 0.0f, 1.0f }; // �u���ʁv�̐F�i�ԁj
    gsDrawSprite2D(Texture_Text, &jyuni_position, &jyuni_rect, NULL, &jyuni_color, NULL, 0.0f);
    // ��ʂT�ʂ̏��ʁi��ȂȂǁj�Ɠ��_��`��
    for (int i = 0; i < (int)ranking_scores_.size(); ++i) {
        GScolor rank_in_color{ 1.0f, 1.0f, 1.0f, 1.0f }; // ��
        // �����N�C��������A���̏��ʂ̕�����Ԃ�����
        if (i == final_rank_) {
            rank_in_color = GScolor{ 1.0f, 0.0f, 0.0f, 1.0f }; // ��
        }
        // �u��ȁv�u���ȁv�u�R�ʁv�u�S�ʁv�u�T�ʁv�̃����N��`��
        const GSrect rank_rect{ 128.0f, 60.0f * i, 256.0f, 60.0f * i + 60.0f };
        const GSvector2 rank_position{ 180.0f, 360.0f + 60.0f * i };
        gsDrawSprite2D(Texture_Text, &rank_position, &rank_rect, NULL, &rank_in_color, NULL, 0.0f);
        // �����L���O�f�[�^���̓��_��`��
        static const NumberTexture number{ Texture_Number, 32, 64 };
        number.draw(GSvector2{ 320.0f, 360.0f + 60.0f * i }, ranking_scores_[i], 3, '0', rank_in_color);
    }
}

// �]�����b�Z�[�W�̕`��
void Result::draw_grade_message() const {
    // �X�R�A����]�����v�Z����
    int grades = calculate_grades();
    // �]���̃^�C�g����\��
    GSrect grades1_rect{ 0.0f, 64.0f * grades, 1024, 64.0f * grades + 64.0f };
    static const GSvector2 grades1_position{ 40.0f, 120.0f };
    static const GScolor grades1_color{ 1.0f, 0.0f, 0.0f, 1.0f };
    gsDrawSprite2D(Texture_Result1, &grades1_position, &grades1_rect, NULL, &grades1_color, NULL, 0.0f);
    // �]���̕��ʂ�\��
    GSrect grades2_rect{ 0.0f, 200.0f * grades, 1024, 200.0f * grades + 180.0f };
    static const GSvector2 grades2_position{ 40.0f, 190.0f };
    gsDrawSprite2D(Texture_Result2, &grades2_position, &grades2_rect, NULL, NULL, NULL, 0.0f);
}

// �X�R�A����]�����v�Z����
int Result::calculate_grades() const {
    if (final_score_ >= 60) return 0;   // �M���d�h ��������
    if (final_score_ >= 50) return 1;   // �f���炵����������
    if (final_score_ >= 40) return 2;   // ���ʂ̌�������
    if (final_score_ >= 30) return 3;   // ����̌�������
    return 4;                           // ���Ȍ�������
}