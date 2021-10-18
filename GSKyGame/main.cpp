#include <GSgame.h> // gslib::Gameに必要

// ゲームクラス
class MyGame : public gslib::Game {
};

// メイン関数
int main() {
    return MyGame().run();
}