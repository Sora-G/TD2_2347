#include "Enemy.h"
#include <MathUtilityForText.h>
#include <TextureManager.h>
#include"Player.h"
#include <cassert>

float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }


Vector3 Normalize(const Vector3& v) {
	float length = Length(v);
	assert(length != 0.0f);
	return { v.x / length, v.y / length, v.z / length };
}

static Vector3 Subtract(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	//返り値
	return result;
}

static Vector3 Multiply(const Vector3& v1, const float v2)
{
	Vector3 result;
	result.x = v1.x * v2;
	result.y = v1.y * v2;
	result.z = v1.z * v2;

	//返り値
	return result;
}

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	textureHandle_ = TextureManager::Load("Earth/Earth.png");

	// ワールド変換の初期化
	worldTransform_.Initialize();

	//初期座標の初期化
	worldTransform_.translation_ = position;

	//移動の初期化
	velocity_ = velocity;

	//接近の初期化
	phase_ = Phase::Approach;

	// 接近速度の初期化
	//approachVel=Vector3(0.0f,0.0f,0.01f);

	//離脱の初期化
	//phase_ = Phase::Leave;
	// 離脱速度の初期化
	//leaveVel=Vector3(0.0f,0.01f,0.0f);
}

void Enemy::Update() {

	// 行列更新
	worldTransform_.UpdateMatrix();

	//// 弾の座標を移動させる（１フレーム分の移動量を足しこむ）
	//worldTransform_.translation_ += velocity_;


	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
		//移動（ベクトルを加算）
		worldTransform_.translation_.z += -0.1f;

		// 敵の発射関数の呼び出し
		if (fireTimer <= 0) {
			Fire();
			// 発射タイマーを初期化
			fireTimer = kFireInterval;
		}
		else {
			fireTimer--;
		}
		//規定の位置に到達したら離脱
		if (worldTransform_.translation_.z < 0.0f)
		{
			phase_ = Phase::Leave;
		}
		break;

	case Enemy::Phase::Leave:
		// 移動（ベクトルを加算)
		worldTransform_.translation_ += Vector3(0.0f, 0.1f, 0.0f);
		break;
	}

	for (std::shared_ptr<EnemyBullet> bullet : bullets_) {
		bullet->Update();
	}

	bullets_.remove_if([this](std::shared_ptr<EnemyBullet> a) { return a->GetIsDead(); });
}

void Enemy::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (std::shared_ptr <EnemyBullet> bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}



// 敵の攻撃処理
void Enemy::Fire()
{
	assert(player_);

	//敵の弾の速さ
	const float kBulletSpeed = -1.0f;


	//自キャラのワールド座標を取得
	playerWorldPosition = player_->GetWorldPosition();
	//敵キャラのワールド座標を取得
	enemyWorldPosition = Enemy::GetWorldPosition();

	//敵キャラから自キャラへの差分ベクトルを求める(当たり判定)
	Vector3 difference = Subtract(enemyWorldPosition, playerWorldPosition);
	//ベクトルの正規化
	Vector3 normalize = Normalize(difference);

	//ベクトルの長さを、速さに合わせる
	Vector3 velocity = Multiply(normalize, kBulletSpeed);

	//Vector3 velocity(0, 0, -kBulletSpeed);

	//速度ベクトルをプレイヤーの向きに合わせて回転させる
	//velocity = TransformNormal(velocity, worldTransform_.matWorld_);	

	//敵の弾を生成し、初期化
	std::shared_ptr<EnemyBullet> newBullet(new EnemyBullet);
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//弾を登録する
	bullets_.push_back(newBullet);
}

//class Enemy
Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() {
	hp_--;
}