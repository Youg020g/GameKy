#ifndef GAME_PLAY_SCENE_H_
#define GAME_PLAY_SCENE_H_

#include "../IScene.h"
#include "../../World/World.h"
#include "../../Result/Result.h"

// �Q�[���v���C�V�[��
class GamePlayScene : public IScene {
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
    // �Q�[������Ԃ̍X�V
    void update_playing(float delta_time);
    // ���U���g��Ԃ̍X�V
    void update_result(float delta_time);

private:
    // �Q�[���v���C�V�[���̏��
    enum class State {
        Playing,    // �Q�[���v���C��
        Result      // ���U���g��
    };
    // ���
    State   state_{ State::Playing };
    // ���[���h�N���X
    World   world_;
    // ���U���g
    Result  result_;
    // ���U���g�p�^�C�}
    float   result_timer_{ 0.0f };
    // �I���t���O
    bool    is_end_{ false };
};

#endif
