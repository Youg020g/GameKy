#include <GSgame.h> // gslib::Game�ɕK�v

// �Q�[���N���X
class MyGame : public gslib::Game {
};

// ���C���֐�
int main() {
    return MyGame().run();
}