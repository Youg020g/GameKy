#include <GSgame.h>
#include "Scene/SceneManager.h"
#include "Scene/GamePlayScene/GamePlayScene.h"

// �}�C�Q�[���N���X
class MyGame : public gslib::Game {
public:
    // �R���X�g���N�^
    MyGame() : gslib::Game{ 1024, 768 } {
    }
    // �J�n
    void start() override {
        scene_maneger_.add("GamePlayScene", new GamePlayScene());
        scene_maneger_.change("GamePlayScene");
    }
    // �X�V
    void update(float delta_time) {
        scene_maneger_.update(delta_time);
    }
    // �`��
    void draw() override {
        scene_maneger_.draw();
    }
    // �I��
    void end() {
        scene_maneger_.end();
    }

private:
    // �V�[���}�l�[�W���[
    SceneManager scene_maneger_;
};

// main�֐�
int main() {
    return MyGame().run();
}