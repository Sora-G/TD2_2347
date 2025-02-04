#pragma once
#include "MathUtilityForText.h"
#include "Model.h"
#include "WorldTransform.h"
class EnemyBullet
{
public: // メンバ関数

	//初期化
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);
	bool GetIsDead() { return isDead_; }

	//寿命＜ｆｒｍ＞
	static const int32_t kLifeTime = 60 * 5;

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	float GetRadius() { return radius_; }


	// 自キャラのワールド座標を取得
	Vector3 GetWorldPosition();

private: // メンバ変数

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;

	float radius_ = 0.5f;
};