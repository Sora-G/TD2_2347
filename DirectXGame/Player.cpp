#include "Player.h"
#include "cassert"
#include "MathUtilityForText.h"
#include "imgui.h"

Player::~Player() {
	//弾の開放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 position) {
	//NULLポインタチェック
	assert(model);
	//引数で受け取ったデータをメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;
	//ワールド変換の初期化
	worldTransform_.Initialize();
	//引数の座標を代入
	worldTransform_.translation_ = position;
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}	

void Player::Update() {
	//アフィン変換
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix(true);

	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動する速さ
	const float kCharacterSpd = 0.2f;

	//押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_A)){
		move.x -= kCharacterSpd;
	}else if (input_->PushKey(DIK_D)){
		move.x += kCharacterSpd;
	}

	//押した方向で移動ベクトルを変更（上下）
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpd;
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpd;
	}

	//座標移動（ベクトルの加算）
	worldTransform_.translation_ += move;

	//移動制限
	const float kMoveLimitX = 27.5f;
	const float kMoveLimitY = 15.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//旋回処理
	Rotate();

	//キャラクターの攻撃処理
	Attack();

	//弾の更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//デスフラグの立った弾を削除
	//remove_if → リストにある物を消す。これをしないとリストの中がnullだらけになる。更新処理とまとめて処理をさせるのは✖
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//キャラクターの座標を画面表示する処理
	ImGui::Begin("Player");

	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.02f);

	ImGui::End();
}

void Player::Draw(ViewProjection& viewProjection) {
	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	
	//弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Rotate() {
	//回転速さ[ラジアン/フレーム]
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_Q)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_E)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {
	//発射キーを押したら
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		//弾を生成し　初期化
		PlayerBullet* newBulelt = new PlayerBullet();
		newBulelt->Initialize(model_, GetWorldPosition(), velocity);

		//弾を登録する
		bullets_.push_back(newBulelt);
	}

}

Vector3 Player::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() {}

void Player::SetParent(const WorldTransform* parent) { 
	//親子関係を結ぶ
	worldTransform_.parent_ = parent; 
}
