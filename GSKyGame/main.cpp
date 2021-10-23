#include <GSgame.h>
#include "Scene/SceneManager.h"
#include "Scene/GamePlayScene/GamePlayScene.h"

// マイゲームクラス
class MyGame : public gslib::Game {
public:
    // コンストラクタ
    MyGame() : gslib::Game{ 1024, 768 } {
    }
    // 開始
    void start() override {
        scene_maneger_.add("GamePlayScene", new GamePlayScene());
        scene_maneger_.change("GamePlayScene");
    }
    // 更新
    void update(float delta_time) {
        scene_maneger_.update(delta_time);
    }
    // 描画
    void draw() override {
        scene_maneger_.draw();
    }
    // 終了
    void end() {
        scene_maneger_.end();
    }

private:
    // シーンマネージャー
    SceneManager scene_maneger_;
};

// main関数
int main() {
    return MyGame().run();
}