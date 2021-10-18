#include "World.h"
#include "../Field/Field.h"
#include "../Actor/Actor.h"

// デストラクタ
World::~World() {
    clear();
}

// 更新
void World::update(float delta_time) {
    // フィールドの更新
    field_->update(delta_time);
    // アクターの更新
    actors_.update(delta_time);
    // アクターの衝突
    actors_.collide();
    // アクターの遅延更新
    actors_.late_update(delta_time);
    // アクターの消去
    actors_.remove();
    // カメラの更新
    camera_->update(delta_time);
    // ライトの更新
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

// 描画
void World::draw() const {
    // カメラの描画
    camera_->draw();
    // ライトの描画
    light_->draw();
    // フィールドの描画
    field_->draw();
    // アクターの描画
    actors_.draw();
    // 半透明アクターの描画
    actors_.draw_transparent();
    // GUIの描画
    actors_.draw_gui();

}

// 消去
void World::clear() {
    // アクターを消去
    actors_.clear();

    // ゲームオーバーフラグの初期化
    is_game_over_ = false;
    // ゲームクリアフラグの初期化
    is_game_clear_ = false;
    //ゲームオーバー処理の開始を許可を初期化
    start_game_over_ = false;
    // カメラを消去
    delete camera_;
    camera_ = nullptr;
    // ライトを消去
    delete light_;
    light_ = nullptr;
    // フィールドを消去
    delete field_;
    field_ = nullptr;
}

// カメラの追加
void World::add_camera(Actor* camera) {
    delete camera_;	// 現在のカメラを削除
    camera_ = camera;
}

// ライトの追加
void World::add_light(Actor* light) {
    delete light_;	// 現在のライトを削除
    light_ = light;
}

// フィールドの追加
void World::add_field(Field* field) {
    delete field_;	// 現在のフィールドを削除
    field_ = field;
}

// アクターの追加
void World::add_actor(Actor* actor) {
    actors_.add(actor);
}

// アクターの検索
Actor* World::find_actor(const std::string& name) const {
    return actors_.find(name);
}

// 指定したタグ名を持つアクターの検索
std::vector<Actor*> World::find_actor_with_tag(const std::string& tag) const {
    return actors_.find_with_tag(tag);
}

// アクター数を返す
int World::count_actor() const {
    return actors_.count();
}

// 指定したタグ名を持つアクター数を返す
int World::count_actor_with_tag(const std::string& tag) const {
    return actors_.count_with_tag(tag);
}

// メッセージ送信
void World::send_message(const std::string& message, void* param) {
    actors_.send_message(message, param);
}

// カメラの取得
Actor* World::camera() {
    return camera_;
}

// ライトの取得
Actor* World::light() {
    return light_;
}

// フィールドの取得
Field* World::field() {
    return field_;
}

// ゲームオーバー
void World::game_over() {
    start_game_over_ = true;
}

// ゲームクリアー
void World::game_clear() {
    is_game_clear_ = true;
}

// ゲームオーバーか？
bool World::is_game_over() const {
    return is_game_over_;
}

// ゲームクリアか？
bool World::is_game_clear() const {
    return is_game_clear_;
}

void World::game_over_after() {
    is_game_over_ = true;
}