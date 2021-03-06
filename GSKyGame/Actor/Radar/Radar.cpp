#include "Radar.h"
#include "../../World/IWorld.h"
#include "../Actor.h"
#include "../../AssetsID/Assets.h"

// レーダーの画面上の表示位置
const GSvector2 RaderPosition{ 16.0f, 20.0f };
// レーダーの画像サイズ
const float RaderSize{ 256.0f };
// レーダーの範囲（プレーヤー位置から-20〜20の範囲）
const float RaderRange{ 20.0f };
// グラウンドのサイズ
const float GroundSize{ 21.0f };

// コンストラクタ
Radar::Radar(IWorld* world) {
    world_ = world;
    tag_ = "RadarTag";
    name_ = "Radar";
    enable_collider_ = false; // 衝突判定をしない
}

void Radar::draw_gui() const{
    // レーダーの背景を描画
    static const GScolor4 back_color{ 1.0f, 1.0f, 1.0f, 0.5f }; // 半透明の指定
    gsDrawSprite2D(Texture_Radar, &RaderPosition, NULL, NULL, &back_color, NULL, 0.0f);

    //プレイヤーの検索
    Actor* player = world_->find_actor("Player");
    // レーダーの座標系に変換する行列
    GSmatrix4 radar_matrix{ GS_MATRIX4_IDENTITY };
    if (player != nullptr) {
        // プレーヤーの逆行列を取得（プレーヤーの位置を基準とする座標を計算するため）
        radar_matrix = player->transform().worldToLocalMatrix();
    }

    // レーダーの上方向をプレーヤーの進行方向に合わせる
    radar_matrix *= GSmatrix4::rotateY(180.0f);

    for (float i = -GroundSize; i <= GroundSize; i += 1.0f){
        // 枠の点の色（水色）
        static const GScolor frame_color{ 0.0f, 1.0f, 1.0f, 1.0 };
        // 4辺の点を描画
        draw_point(radar_matrix.multiplyPoint(GSvector3{ i, 0.0f, -GroundSize }), frame_color);
        draw_point(radar_matrix.multiplyPoint(GSvector3{ i, 0.0f,  GroundSize }), frame_color);
        draw_point(radar_matrix.multiplyPoint(GSvector3{ -GroundSize, 0.0f, i }), frame_color);
        draw_point(radar_matrix.multiplyPoint(GSvector3{ GroundSize, 0.0f, i }), frame_color);
    }

    // 敵の位置を描画
    for (Actor* enemy : world_->find_actor_with_tag("EnemyTag")) {
        // 敵の点の色（緑）
        static const GScolor enemy_color{ 0.0f, 1.0f, 0.0f, 1.0 };
        // 敵の点を描画
        draw_point(radar_matrix.multiplyPoint(enemy->transform().position()), enemy_color);
    }
    // プレーヤーの点の色（赤）
    static const GScolor player_color{ 1.0f, 0.0f, 0.0f, 1.0 };
    // プレーヤーの位置を描画（プレーヤーの点は原点）
    draw_point(GSvector3{ 0.0f, 0.0f, 0.0f }, player_color);

}

void Radar::draw_point(const GSvector3& position, const GScolor& color) const{
    // レーダーの2D座標にして正規化する(表示範囲を-1.0〜1.0の座標に変換)
    GSvector2 normalized_position{ position.x / RaderRange,  position.z / RaderRange };

    // レーダーの範囲外は描画しない
    if (normalized_position.x < -1.0f || normalized_position.x > 1.0f
        || normalized_position.y < -1.0f || normalized_position.y > 1.0f) {
        return;
    }

    // レーダーの画像サイズの半分(128ピクセル)
    const float RaderHalfSize{ RaderSize / 2.0f };
    // レーダーの画像サイズに合わせて拡大する(-128〜128)
    GSvector2 point_position = normalized_position * RaderHalfSize;
    // 原点（プレーヤーの位置）がレーダーの中央になるように補正(0〜256)
    point_position += GSvector2{ RaderHalfSize, RaderHalfSize };
    // レーダーの表示位置に合わせて平行移動
    point_position += RaderPosition;

    // 点の画像の中心位置を補正値する（画像の中央が原点になるように）
    static const GSvector2 center{ 4.0f, 4.0f };
    // 点の画像を描画
    gsDrawSprite2D(Texture_RadarPoint, &point_position, NULL, &center, &color, NULL, 0.0f);


}


