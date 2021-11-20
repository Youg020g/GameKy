#ifndef TIMER_H_
#define TIMER_H_

// �������ԃN���X
class Timer {
public:
    // �R���X�g���N�^
    Timer(float time = 0.0f);
    // ������
    void initialize(float time);
    // �X�V
    void update(float delta_time);
    // �`�� 
    void draw() const;
    // �^�C���A�E�g���H
    bool is_timeout() const;

private:
    // �^�C�}�i�b�P�ʁj
    float time_{ 0.0f };
};

#endif
