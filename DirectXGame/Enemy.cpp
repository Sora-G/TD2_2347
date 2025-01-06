#include "Enemy.h"
#include "cassert"
#include "TextureManager.h"
#include "MathUtilityForText.h"
#include "imgui.h"
#include "Player.h"

float Dot(const Vector3& v1, const Vector3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
float Length(const Vector3& v) { return std::sqrt(Dot(v, v)); }

Vector3 Normalize(const Vector3& v) {
	float length = Length(v);
	assert(length != 0.0f);
	return {v.x / length, v.y / length, v.z / length};
}

Enemy::~Enemy() {
	//弾の解放
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, const Vector3& position, Vector3 approachVelocity, Vector3 leaveVelocity) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	//テクスチャハンドルの読み込み
	textureHandle_ = TextureManager::Load("uvChecker.png");
	//ワールド変換の初期化
	worldTransform_.Initialize();
	//引数で受け取った初期座標を代入
	worldTransform_.translation_ = position;
	//速度に引数を代入
	approachVelocity_ = approachVelocity;
	leaveVelocity_ = leaveVelocity;

	//発射クールタイムを初期化
	shotCoolTimeMax_ = 120;
	shotCoolTime_ = shotCoolTimeMax_;
}

void Enemy::Update() { 

	switch (phase_) {
	case Enemy::Phase::Approach:
	default:

		// 移動
		worldTransform_.translation_ += approachVelocity_;
		//規定の位置に到達したら離脱
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Approach;
		}

		break;

	case Enemy::Phase::Leave:

		//移動（ベクトルを加算）
		worldTransform_.translation_ += leaveVelocity_;

		break;
	}
	
	//座標を基に行列の更新を行う
	worldTransform_.UpdateMatrix(true);

	//弾のクールタイムを減らす
	shotCoolTime_--;

	//クールタイムが０の時に攻撃
	if (shotCoolTime_ <= 0)
	{
		// 敵の攻撃処理
		Fire();

		//クールタイムを初期化
		shotCoolTime_ = shotCoolTimeMax_;
	}

	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Enemy");

	ImGui::DragFloat3("transform", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragInt("shotCoolTime", &shotCoolTime_, 0);
	ImGui::DragInt("shotMaxCoolTime", &shotCoolTimeMax_, 1);
	ImGui::DragFloat3("playerWorldPosition", &playerWorldPos.x, 0);
	ImGui::DragFloat3("enemyWorldPosition", &enemyWorldPos.x, 0);
	ImGui::DragFloat3("enemy to player", &e2pVector.x, 0);
	ImGui::End();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾の描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire() {

	assert(player_);

	// 弾の速度
	const float kBulletSpeed = 1.0f;

	//自キャラのワールド座標を取得
	playerWorldPos = player_->GetWorldPosition();
	//敵キャラのワールド座標を取得
	enemyWorldPos = GetWorldPosition();
	//敵キャラから自キャラへの差分ベクトルを求める
	e2pVector.x = playerWorldPos.x - enemyWorldPos.x;
	e2pVector.y = playerWorldPos.y - enemyWorldPos.y;
	e2pVector.z = playerWorldPos.z - enemyWorldPos.z;
	//ベクトルの正規化
	e2pNormal = Normalize(e2pVector);

	Vector3 velocity(e2pNormal.x * kBulletSpeed, e2pNormal.y * kBulletSpeed, e2pNormal.z * kBulletSpeed);

	// 速度ベクトルを自機の向きに合わせて回転させる
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し　初期化
	EnemyBullet* newBulelt = new EnemyBullet();
	newBulelt->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	bullets_.push_back(newBulelt);
}

Vector3 Enemy::GetWorldPosition() { 
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() {}
