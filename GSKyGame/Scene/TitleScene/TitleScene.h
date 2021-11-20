#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

#include "../IScene.h"
#include <gslib.h>

// �^�C�g���V�[��
class TitleScene : public IScene {
public:
    // �J�n
    virtual void start() override;
    // �X�V
    virtual void update(float delta_time) override;
    // �`��
    virtual void draw() const override;
    // �I�����Ă��邩�H
    virtual bool is_end() const override;
    // ���̃V�[����Ԃ�
    virtual std::string next() const override;
    // �I��
    virtual void end() override;

    
private:
    //���������̕`��
    void draw_player()const;
    //��蕔���̕`��
    void draw_karate() const;
    // �w�i�̕`��
    void draw_background(GSuint id) const;

    // �I���t���O
    bool is_end_{ false };
    // ���������ړ��p�^�C�}
    float player_moving_timer_{ 0.0f };
    // ��蕔���ړ��p�^�C�}
    float enemy_moving_timer_{ 0.0f };
    //�e�L�X�g�_�ŗp�^�C�}
    float   text_timer_{ 0.0f };

};

#endif // !TITLE_SCENE_H_


