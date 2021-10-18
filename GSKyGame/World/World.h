#ifndef WORLD_H_
#define WORLD_H_

#include "IWorld.h"
#include "../Actor/ActorManager.h"

// ���[���h�N���X
class World : public IWorld {
public:
    // �R���X�g���N�^
    World() = default;
    // �f�X�g���N�^
    ~World();
    // �X�V
    void update(float delta_time);
    // �`��
    void draw() const;
    // ����
    void clear();
    // �J�����̒ǉ�
    void add_camera(Actor* camera);
    // ���C�g�̒ǉ�
    void add_light(Actor* light);
    // �t�B�[���h�̒ǉ�
    void add_field(Field* field);

    // �A�N�^�[��ǉ�
    virtual void add_actor(Actor* actor) override;
    // �A�N�^�[�̌���
    virtual Actor* find_actor(const std::string& name) const override;
    // �w�肵���^�O�������A�N�^�[�̌���
    virtual std::vector<Actor*> find_actor_with_tag(const std::string& tag) const override;
    // �A�N�^�[����Ԃ�
    virtual int count_actor() const override;
    // �w�肵���^�O�������A�N�^�[����Ԃ�
    virtual int count_actor_with_tag(const std::string& tag) const override;
    // ���b�Z�[�W�̑��M
    virtual void send_message(const std::string& message, void* param = nullptr) override;

    // �J�����̎擾
    virtual Actor* camera() override;
    // ���C�g�̎擾
    virtual Actor* light() override;
    // �t�B�[���h�̎擾
    virtual Field* field() override;

    //�Q�[���I�[�o�[
    virtual void game_over() override;
    // �Q�[���N���A
    virtual void game_clear() override;
    // �Q�[���I�[�o�[���H
    bool is_game_over() const;
    // �Q�[���N���A���H
    bool is_game_clear() const;

    virtual void game_over_after()override;

    // �R�s�[�֎~
    World(const World& other) = delete;
    World& operator = (const World& other) = delete;

private:
    // �A�N�^�[�}�l�[�W���[
    ActorManager  actors_;
    // ���C�g
    Actor* light_{ nullptr };
    // �J����
    Actor* camera_{ nullptr };
    // �t�B�[���h
    Field* field_{ nullptr };

    //�Q�[���I�[�o�[���H
    bool         is_game_over_{ false };
    // �Q�[���N���A���H
    bool         is_game_clear_{ false };

    //�Q�[���I�[�o�[�����̊J�n������
    bool         start_game_over_{ false };
    int game_over_timer_{ 0 };
};

#endif