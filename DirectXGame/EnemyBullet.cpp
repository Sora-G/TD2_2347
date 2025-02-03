#include "EnemyBullet.h"
#include "cassert"
#include "TextureManager.h"
#include "MathUtilityForText.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("Rock/Rock.png");
	// ワールド座標の初期化
	worldTransform_.Initialize();
	// 引数で受け取った初期座標を代入
	worldTransform_.translation_ = position;
	// 引数で受け取った速度を代入
	velocity_ = velocity;
}

void EnemyBullet::Update() {
	// 座標を移動させる（１フレーム分の移動量を足しこむ）
	worldTransform_.translation_ += velocity_;

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix(true);
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	// モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetWorldPosition() { 
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void EnemyBullet::OnCollision() {
	//デスフラグを立てる
	EnemyBulletIsDead_ = true;
}
