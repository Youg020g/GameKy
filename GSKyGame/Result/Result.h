#ifndef RESULT_H_
#define RESULT_H_

#include <vector>
#include <string>

// ���U���g�N���X
class Result {
public:
    // �R���X�g���N�^
    Result();
    // ������
    void initialize();
    // �X�R�A�̒ǉ�
    void add_score(int value);
    // �X�V
    void update(float delta_time);
    // �`��
    void draw() const;
    // �����L���O�t�@�C���̕ۑ�
    void save(const std::string& file_name) const;
    // �����L���O�t�@�C���̓ǂݍ���
    void load(const std::string& file_name);

private:
    // �w�i�̕`��
    void draw_background() const;
    // �v���[���[�̕`��
    void draw_player() const;
    // �����L���O�̕`��
    void draw_ranking() const;
    // �]�����b�Z�[�W�̕`��
    void draw_grade_message() const;
    // �X�R�A����]�����v�Z����
    int calculate_grades() const;

private:
    // �����L���O�f�[�^
    std::vector<int> ranking_scores_;
    // �Ō�ɒǉ������X�R�A
    int final_score_{ 0 };
    // �Ō�ɒǉ������X�R�A�̃����N
    int final_rank_{ 0 };
    // �v���[���[�ړ��p�^�C�}
    float moving_timer_{ 0.0f };
};

#endif
