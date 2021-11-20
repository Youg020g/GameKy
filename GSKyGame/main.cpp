#include <GSgame.h>
#include "Scene/SceneManager.h"
#include "Scene/TitleScene/TitleScene.h"
#include "Scene/GamePlayScene/GamePlayScene.h"


// マイゲームクラス
class MyGame : public gslib::Game {
    // シーンマネージャー
    SceneManager scene_maneger_;
public:
    // コンストラクタ
    MyGame() : gslib::Game{ 1024, 768 } {
    }
    // 開始
    void start() override {
        // タイトルシーンの追加
        scene_maneger_.add("TitleScene", new TitleScene());
        // ゲームプレイシーンの追加
        scene_maneger_.add("GamePlayScene", new GamePlayScene());
        // タイトルシーンから開始
        scene_maneger_.change("TitleScene");
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


};

// main関数
int main() {
    return MyGame().run();
}