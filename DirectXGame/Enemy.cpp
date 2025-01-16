#include "Enemy.h"
#include "MathUtilityForText.h"
#include "Player.h"
#include <TextureManager.h>
#include <cassert>

float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
float Length(const Vector3& v) { return std::sqrt(Dot(v, v)); }

Vector3 Normalize(const Vector3& v) {
	float length = Length(v);
	assert(length != 0.0f);
	return { v.x / length, v.y / length, v.z / length };
}

Enemy::Enemy() {}

Enemy::~Enemy() {

	// bullet_の解放
	for (EnemyBullet* bullet : bullets_) {

		delete bullet;
	}
}

void Enemy::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	// NULLポインタチェック
	assert(model);

	// 引数
	model_ = model;
	//textureHandle_ = textureHandle;



	// viewProjection_ = viewProjection;

	velocity_ = velocity;

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	// 弾のクールタイム
	/*shotCollTimeMax = 20;
	shotCollTime_ = shotCollTimeMax;*/

	// 弾発射
	// Fire();

	// 接近フェーズ初期化
	ApproacPphase();
}

void Enemy::Update() {

	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}

		return false;
		});

	// アフィン変換行列の作成
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 座標を移動させる(１フレーム分の移動量を差し込む)
	// worldTransform_.translation_ += velocity_;

	// worldTransform_.UpdateMatrix();

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	switch (phase_) {

	case Enemy::Phase::Approach:
	default:

		// 移動(ベクトルを加算)
		//worldTransform_.translation_ += velocity_;

		// 規定の位置に到着したら離脱
		/*if (worldTransform_.translation_.z < 0.0f) {

			phase_ = Phase::Leave;
		}*/

		break;

	case Enemy::Phase::Leave:

		// 移動(ベクトルを加算)
		//worldTransform_.translation_ += Vector3(0.05f, 0.05f, 0.0f);

		break;
	}

	shotCollTime_--;

	if (shotCollTime_ <= 0) {
		// 攻撃処理
		Fire();

		// クールタイム初期化
		shotCollTime_ = shotCollTimeMax;
	}

	// 弾更新
	for (EnemyBullet* bullet : bullets_) {

		bullet->Update();
	}
}

void Enemy::Draw(ViewProjection& viewProjection) {

	// 3Dモデルの描画
	model_->Draw(worldTransform_, viewProjection);

	// 弾描画
	for (EnemyBullet* bullet : bullets_) {

		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire() {

	assert(player_);

	// 弾の速度
	const float kEnemyBulletSpeed = 0.5f;

	// 自機のワールド座標取得
	Vector3 PlayerWorldPos = player_->GetWorldPosition();

	// 敵のワールド座標取得
	Vector3 EnemyWorldPos = GetWorldPosition();

	// 敵キャラから自キャラへの差分ベクトルを求める
	Vector3 Difference = { PlayerWorldPos.x - EnemyWorldPos.x, PlayerWorldPos.y - EnemyWorldPos.y, PlayerWorldPos.z - EnemyWorldPos.z };

	// ベクトルの正規化
	Vector3 Normal = Normalize(Difference);

	// ベクトルの長さを速さに合わせる
	Vector3 velocity(Normal * kEnemyBulletSpeed);

	if (sqrt(pow(Difference.x, 2.0) + pow(Difference.y, 2.0) + pow(Difference.z, 2.0) * 1.0) < 20.0f) {
		// 弾を生成し、初期化
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);

	}



}

// 接近フェーズ
void Enemy::ApproacPphase() {
	// 発射タイマー初期化
	FireTimer = kFireInterval;

	// 発射タイマーカウントダウン
	FireTimer--;

	// 指定時間に達した
	if (FireTimer <= 0) {

		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		FireTimer = kFireInterval;
	}


	//一定距離に入ったら攻撃

}

Vector3 Enemy::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;



	return worldPos;
}

void Enemy::OnCollision() {}
