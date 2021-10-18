#include "World.h"
#include "../Field/Field.h"
#include "../Actor/Actor.h"

// �f�X�g���N�^
World::~World() {
    clear();
}

// �X�V
void World::update(float delta_time) {
    // �t�B�[���h�̍X�V
    field_->update(delta_time);
    // �A�N�^�[�̍X�V
    actors_.update(delta_time);
    // �A�N�^�[�̏Փ�
    actors_.collide();
    // �A�N�^�[�̒x���X�V
    actors_.late_update(delta_time);
    // �A�N�^�[�̏���
    actors_.remove();
    // �J�����̍X�V
    camera_->update(delta_time);
    // ���C�g�̍X�V
    light_->update(delta_time);

    if (start_game_over_) {
        game_over_timer_ += delta_time;

        if (game_over_timer_ >= 120) {
            game_over_after();
            game_over_timer_ = 0;
            start_game_over_ = false;
        }
    }

}

// �`��
void World::draw() const {
    // �J�����̕`��
    camera_->draw();
    // ���C�g�̕`��
    light_->draw();
    // �t�B�[���h�̕`��
    field_->draw();
    // �A�N�^�[�̕`��
    actors_.draw();
    // �������A�N�^�[�̕`��
    actors_.draw_transparent();
    // GUI�̕`��
    actors_.draw_gui();

}

// ����
void World::clear() {
    // �A�N�^�[������
    actors_.clear();

    // �Q�[���I�[�o�[�t���O�̏�����
    is_game_over_ = false;
    // �Q�[���N���A�t���O�̏�����
    is_game_clear_ = false;
    //�Q�[���I�[�o�[�����̊J�n������������
    start_game_over_ = false;
    // �J����������
    delete camera_;
    camera_ = nullptr;
    // ���C�g������
    delete light_;
    light_ = nullptr;
    // �t�B�[���h������
    delete field_;
    field_ = nullptr;
}

// �J�����̒ǉ�
void World::add_camera(Actor* camera) {
    delete camera_;	// ���݂̃J�������폜
    camera_ = camera;
}

// ���C�g�̒ǉ�
void World::add_light(Actor* light) {
    delete light_;	// ���݂̃��C�g���폜
    light_ = light;
}

// �t�B�[���h�̒ǉ�
void World::add_field(Field* field) {
    delete field_;	// ���݂̃t�B�[���h���폜
    field_ = field;
}

// �A�N�^�[�̒ǉ�
void World::add_actor(Actor* actor) {
    actors_.add(actor);
}

// �A�N�^�[�̌���
Actor* World::find_actor(const std::string& name) const {
    return actors_.find(name);
}

// �w�肵���^�O�������A�N�^�[�̌���
std::vector<Actor*> World::find_actor_with_tag(const std::string& tag) const {
    return actors_.find_with_tag(tag);
}

// �A�N�^�[����Ԃ�
int World::count_actor() const {
    return actors_.count();
}

// �w�肵���^�O�������A�N�^�[����Ԃ�
int World::count_actor_with_tag(const std::string& tag) const {
    return actors_.count_with_tag(tag);
}

// ���b�Z�[�W���M
void World::send_message(const std::string& message, void* param) {
    actors_.send_message(message, param);
}

// �J�����̎擾
Actor* World::camera() {
    return camera_;
}

// ���C�g�̎擾
Actor* World::light() {
    return light_;
}

// �t�B�[���h�̎擾
Field* World::field() {
    return field_;
}

// �Q�[���I�[�o�[
void World::game_over() {
    start_game_over_ = true;
}

// �Q�[���N���A�[
void World::game_clear() {
    is_game_clear_ = true;
}

// �Q�[���I�[�o�[���H
bool World::is_game_over() const {
    return is_game_over_;
}

// �Q�[���N���A���H
bool World::is_game_clear() const {
    return is_game_clear_;
}

void World::game_over_after() {
    is_game_over_ = true;
}