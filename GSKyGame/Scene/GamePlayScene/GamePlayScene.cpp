#include "GamePlayScene.h"
#include "../../Actor/Player/Player.h"
#include "../../Field/Field.h"
#include "../../Light/Light.h"
#include "../../Camera/CameraTPS.h"
#include "../../AssetsID/Assets.h"

// 開始
void GamePlayScene::start() {
    // 剣道部長メッシュの読み込み
    gsLoadMesh(Mesh_Player, "Assets/model/Kendo/kendo.msh");
    gsLoadSkeleton(Mesh_Player, "Assets/model/Kendo/kendo.skl");
    gsLoadAnimation(Mesh_Player, "Assets/model/Kendo/kendo.anm");

    // スカイボックスメッシュの読み込み
    gsLoadMesh(Mesh_Skybox, "Assets/model/sky/skydome.msh");
    // 校庭オクトリーの読み込み
    gsLoadOctree(Octree_Koutei, "Assets/model/koutei/koutei.oct");
    // 校庭衝突判定用オクトリーの読み込み
    gsLoadOctree(Octree_KouteiCollider, "Assets/model/koutei/koutei_collider.oct");

    // フィールドクラスの追加
    world_.add_field(new Field{ Octree_Koutei, Octree_KouteiCollider, Mesh_Skybox });
    // カメラクラスの追加
    world_.add_camera(new CameraTPS{
              &world_,GSvector3{ 0.0f, 2.0f, -4.0f }, GSvector3{ 0.0f, 1.0f, 0.0f } });
    // ライトクラスの追加
    world_.add_light(new Light{ &world_ });
    // プレーヤーを追加
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
}

// 更新
void GamePlayScene::update(float delta_time) {
    world_.update(delta_time);
}

// 描画
void GamePlayScene::draw() const {
    world_.draw();
}

// 終了しているか？
bool GamePlayScene::is_end() const {
    return false;
}

// 次のシーンを返す
std::string GamePlayScene::next() const {
    return "TitleScene";
}

// 終了
void GamePlayScene::end() {
    // ワールドを消去
    world_.clear();
}