#ifndef FIELD_H_
#define FIELD_H_

#include <gslib.h>

class Ray;
class Line;
class BoundingSphere;

//フィールドクラス
class Field {
public:
	//コンストラクタ
	Field(GSuint octree, GSuint collider, GSuint skybox);

	//更新
	void update(float delta_time);
	//描画
	void draw() const;

	//線分との衝突判定
	bool collide(const Line& line,
		GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;

	// レイとの衝突判定
	bool collide(const Ray& ray, float max_distance,
		GSvector3* intersect = nullptr, GSplane* plane = nullptr) const;

	//球体
	bool collide(const BoundingSphere& sphere, GSvector3* center = nullptr) const;

	Field(const Field& other) = default;
	Field& operator = (const Field& other) = delete;

	//グラウンドの広さ
	GSvector3 ground_max{ 20.0f,0.0f,20.0f };
	GSvector3 ground_min{ -20.0f,0.0f,-20.0f };

private:
	// 描画用オクトリー
	GSuint octree_;
	//衝突判定用オクトリー
	GSuint	collider_;
	// スカイボックス
	GSuint	skybox_;
};

#endif // !FIELD_H_
