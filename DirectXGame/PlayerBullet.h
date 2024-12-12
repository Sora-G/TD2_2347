#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class PlayerBullet {

public:                  //モデル                 初期座標                 速度
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	void Draw(ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	Vector3 GetWorldPosition();

	// 半径
	const float PlayerBulletRad = 1;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;


};
