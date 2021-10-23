#ifndef ASSETS_H_
#define ASSETS_H_

enum {                      // メッシュデータ
    Mesh_Player,            // プレーヤ
    Mesh_Enemy,             // 敵
    Mesh_Skybox             // スカイボックス
};

enum {                      // オクトリーデータ
    Octree_Koutei,          // 校庭
    Octree_KouteiCollider   // 校庭衝突判定
};

enum {                      // テクスチャデータ
    Texture_Title,          // タイトル
    Texture_Kendo,          // 剣道部長
    Texture_Karate,         // 空手部長
    Texture_Start,          // 開始メッセージ
    Texture_Number,         // 数字フォント
    Texture_Text,           // テキスト画像
    Texture_BlueBack,       // リザルト用背景
    Texture_Result1,        // リザルト用テキスト1
    Texture_Result2,        // リザルト用テキスト2
    Texture_Radar,          // レーダー画像
    Texture_RadarPoint      // レーダーの点
};

#endif