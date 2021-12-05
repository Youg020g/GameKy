#include "GamePlayScene.h"
#include "../../Actor/Player/Player.h"
#include "../../Field/Field.h"
#include "../../Light/Light.h"
#include "../../Camera/CameraTPS.h"
#include "../../AssetsID/Assets.h"
#include "../../Actor/Enemy/Enemy.h"
#include "../../Actor/EnemyGenerator/EnemyGenerator.h"
#include "../../Actor/Radar/Radar.h"
#include <GSmusic.h>


// 開始
void GamePlayScene::start() {
    // 剣道部長メッシュの読み込み
    gsLoadMesh(Mesh_Player, "Assets/model/Kendo/kendo.msh");
    gsLoadSkeleton(Mesh_Player, "Assets/model/Kendo/kendo.skl");
    gsLoadAnimation(Mesh_Player, "Assets/model/Kendo/kendo.anm");
    // 空手部長メッシュの読み込み
    gsLoadMesh(Mesh_Enemy, "Assets/model/Karate/karate.msh");
    gsLoadSkeleton(Mesh_Enemy, "Assets/model/Karate/karate.skl");
    gsLoadAnimation(Mesh_Enemy, "Assets/model/Karate/karate.anm");

    // スカイボックスメッシュの読み込み
    gsLoadMesh(Mesh_Skybox, "Assets/model/sky/skydome.msh");
    // 校庭オクトリーの読み込み
    gsLoadOctree(Octree_Koutei, "Assets/model/koutei/koutei.oct");
    // 校庭衝突判定用オクトリーの読み込み
    gsLoadOctree(Octree_KouteiCollider, "Assets/model/koutei/koutei_collider.oct");

    gsLoadTexture(Texture_Number, "Assets/texture/num.png");
    gsLoadTexture(Texture_Text, "Assets/texture/text.png");


    // リザルト中の背景の読み込み
    gsLoadTexture(Texture_BlueBack, "Assets/texture/blue.png");
    // 評価タイトルの読み込み
    gsLoadTexture(Texture_Result1, "Assets/texture/result1.png");
    // 評価文章 の読み込み
    gsLoadTexture(Texture_Result2, "Assets/texture/result2.png");
    // 剣道部長画像の読み込み
    gsLoadTexture(Texture_Kendo, "Assets/texture/mini_kendo.png");
    // 開始ボタンを押忍の読み込み
    gsLoadTexture(Texture_Start, "Assets/texture/osu.png");

    // レーダーの背景の画像を読み込み
    gsLoadTexture(Texture_Radar, "Assets/texture/radar.png");
    // レーダーの点の画像を読み込み
    gsLoadTexture(Texture_RadarPoint, "Assets/texture/pt.png");

    // ゲーム開始時の効果音の読み込み
    gsLoadSE(Se_GameStart, "Assets/sound/Select.wav", 1, GWAVE_DEFAULT);
    // 剣道部長攻撃時効果音の読み込み
    gsLoadSE(Se_PlayerAttack, "Assets/sound/Attack1.wav", 1, GWAVE_DEFAULT);
    // 剣道部長ダメージ効果音の読み込み
    gsLoadSE(Se_PlayerDamage, "Assets/sound/Damage2.wav", 1, GWAVE_DEFAULT);
    // 空手部長ダメージ効果音の読み込み
    gsLoadSE(Se_EnemeyDamage, "Assets/sound/Attack2.wav", 1, GWAVE_DEFAULT);
    // タイムアウトの効果音の読み込み
    gsLoadSE(Se_Timeout, "Assets/sound/timeend.wav", 1, GWAVE_DEFAULT);

    // ゲームプレイ中用BGMの読み込み
    gsLoadMusic(Sound_PlayingBGM, "Assets/sound/kendo.wav", GS_TRUE);
    // リザルト用BGMの読み込み
    gsLoadMusic(Sound_ResultBGM, "Assets/sound/ed.wav", GS_TRUE);

    // BGMのバインド
    gsBindMusic(Sound_PlayingBGM);
    // BGMの再生
    gsPlayMusic();
    // 開始効果音（いくぜ！の掛け声）
    gsPlaySE(Se_GameStart);


    // フィールドクラスの追加
    world_.add_field(new Field{ Octree_Koutei, Octree_KouteiCollider, Mesh_Skybox });
    // カメラクラスの追加
    world_.add_camera(new CameraTPS{
              &world_,GSvector3{ 0.0f, 2.0f, -4.0f }, GSvector3{ 0.0f, 1.0f, 0.0f } });
    // ライトクラスの追加
    world_.add_light(new Light{ &world_ });
    // レーダークラスを追加
    world_.add_actor(new Radar{ &world_ });
    // プレーヤーを追加
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
    //エネミー生成クラス
    world_.add_actor(new EnemyGenerator{ &world_});
    // タイマの初期化
    world_.timer().initialize(60.0f);

    // リザルトの初期化
    result_.initialize();
    // ランキングファイルの読み込み
    result_.load("Assets/ranking.txt");
    // 状態の初期化
    state_ = State::Playing;
    //　シーン終了フラグの初期化
    is_end_ = false;

}

// 更新
void GamePlayScene::update(float delta_time) {
    // 状態別の更新
    switch (state_) {
    case State::Playing: update_playing(delta_time); break;
    case State::Result:  update_result(delta_time);  break;
    }

    //// テスト用にZキーを押したら敵を生成するようにする
    //if (gsGetKeyTrigger(GKEY_Z)) {
    //    world_.add_actor(new Enemy{ &world_, GSvector3{0.0f, 0.0f, -20.0f} });
    //}
    
}

// ゲームプレイ中の更新
void GamePlayScene::update_playing(float delta_time) {
    // ワールドクラスの更新
    world_.update(delta_time);
    // ゲームオーバーになったらリザルト状態に遷移
    if (world_.is_game_over()) {
        // タイムアウトの効果音を再生
        gsPlaySE(Se_Timeout);
        // BGMを停止
        gsStopMusic();
        // リザルトシーン用のBGMを再生
        gsBindMusic(Sound_ResultBGM);
        gsPlayMusic();

        // 点数を追加
        result_.add_score(world_.score_.get());
        // リザルト中タイマを初期化
        result_timer_ = 0.0f;
        // リザルト状態に遷移
        state_ = State::Result;
    }
}

// リザルト中の更新
void GamePlayScene::update_result(float delta_time) {
    // リザルトの更新
    result_.update(delta_time);
    // リザルト中タイマの更新
    result_timer_ += delta_time;
    // 最低1秒間はリザルト表示、スペースキーを押したらシーン終了
    if (result_timer_ >= 60.0f && gsGetKeyTrigger(GKEY_SPACE)) {
        is_end_ = true;
    }
}

// 描画
void GamePlayScene::draw() const {
    // ワールドの描画
    world_.draw();
    // リザルト中はリザルトを描画
    if (state_ == State::Result) {
        // リザルトの描画
        result_.draw();
        // リザルト開始１秒後に「開始ボタンを押忍！」を点滅表示する
        if (result_timer_ >= 60.0f && fmod(result_timer_, 40.0f) < 20.0f) {
            static const GSvector2 position{ 250, 700 };
            gsDrawSprite2D(Texture_Start, &position, NULL, NULL, NULL, NULL, 0.0f);
        }
    }

    //gsDrawText("モーション番号 = %d", world_.count_actor_with_tag("EnemyTag"));
}

// 終了しているか？
bool GamePlayScene::is_end() const {
    return is_end_;
}

// 次のシーンを返す
std::string GamePlayScene::next() const {
    return "TitleScene";
}

// 終了
void GamePlayScene::end() {
    // BGMの停止
    gsStopMusic();
    // ランキングファイルの保存
    result_.save("Assets/ranking.txt");
    // ワールドを消去
    world_.clear();
    // アセットの削除
    gsDeleteMesh(Mesh_Player);
    gsDeleteSkeleton(Mesh_Player);
    gsDeleteAnimation(Mesh_Player);
    gsDeleteMesh(Mesh_Enemy);
    gsDeleteSkeleton(Mesh_Enemy);
    gsDeleteAnimation(Mesh_Enemy);
    gsDeleteMesh(Mesh_Skybox);
    gsDeleteOctree(Octree_Koutei);
    gsDeleteTexture(Texture_Number);
    gsDeleteTexture(Texture_Text);
    gsDeleteTexture(Texture_BlueBack);
    gsDeleteTexture(Texture_Result1);
    gsDeleteTexture(Texture_Result2);
    gsDeleteTexture(Texture_Start);
    gsDeleteTexture(Texture_Radar); 
    gsDeleteTexture(Texture_RadarPoint);
    gsDeleteMusic(Sound_PlayingBGM);
    gsDeleteMusic(Sound_ResultBGM);
    gsDeleteSE(Se_GameStart);
    gsDeleteSE(Se_PlayerAttack);
    gsDeleteSE(Se_PlayerDamage);
    gsDeleteSE(Se_EnemeyDamage);
    gsDeleteSE(Se_Timeout);

}
