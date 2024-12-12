#include "Player.h"
#include "cassert"
#include "TextureManager.h"

#include "MathUtilityForText.h"
#include "imgui.h"

void Player::Initialize(Model* model)
{
	//NULLポインタチェック
	assert(model);
	//引数のモデルをメンバ変数のモデルに代入
	model_ = model;
	//ワールド変換の初期化
	worldTransform_.Initialize();
	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();
}

void Player::Update()
{
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	//ワールド行列を転送
	worldTransform_.TransferMatrix();

	//移動関数
	Move();


	//ImGui表示
	ImGui::Begin("Player");
	ImGui::Text("[W]FRONT [S]BACK [D]RIGHT [A]LEFT [SPACE]UP [LSHIFT]DOWN");
	ImGui::DragFloat3("worldPos", &worldTransform_.translation_.x, 0.0f);
	ImGui::End();
}

void Player::Draw(ViewProjection& viewprojection)
{
	//3Dモデルの描画
	model_->Draw(worldTransform_, viewprojection);
}

void Player::Move()
{
	//移動ベクトル
	Vector3 moveVec = { 0.0f,0.0f,0.0f };
	//移動速度
	float kMoveSpd = 0.2f;

	//キー入力で移動
	//奥行の移動
	if (input_->PushKey(DIK_W))
	{
		moveVec.z += kMoveSpd;
	}
	if (input_->PushKey(DIK_S))
	{
		moveVec.z -= kMoveSpd;
	}
	//左右移動
	if (input_->PushKey(DIK_D))
	{
		moveVec.x += kMoveSpd;
	}
	if (input_->PushKey(DIK_A))
	{
		moveVec.x -= kMoveSpd;
	}
	//上下移動
	if (input_->PushKey(DIK_SPACE))
	{
		moveVec.y += kMoveSpd;
	}
	if (input_->PushKey(DIK_LSHIFT))
	{
		moveVec.y -= kMoveSpd;
	}

	//ワールド座標に移動ベクトルを加算
	worldTransform_.translation_ += moveVec;
}
