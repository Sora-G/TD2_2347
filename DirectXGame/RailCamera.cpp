#include "RailCamera.h"
#include "MathUtilityForText.h"
#include "imgui.h"

void RailCamera::Initialize(Vector3 worldAxis, Vector3 radian) {
	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = worldAxis;
	worldTransform_.rotation_ = radian;
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//インスタンスを取得
	input_ = Input::GetInstance();
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 600.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update() { 

	// カメラの移動速度
	Vector3 moveSpd = {};
	//const float kMoveSpd = 0.2f;

	//座標の位置を加算
	//if (input_->PushKey(DIK_T))
	//{
	//	moveSpd.y += kMoveSpd;
	//}
	//if (input_->PushKey(DIK_G)) {
	//	moveSpd.y -= kMoveSpd;
	//}
	//if (input_->PushKey(DIK_F)) {
	//	moveSpd.x -= kMoveSpd;
	//}
	//if (input_->PushKey(DIK_H)) {
	//	moveSpd.x += kMoveSpd;
	//}
	//
	//moveSpd.z = static_cast<float>(input_->GetWheel()) / 5.0f;

	worldTransform_.translation_ += moveSpd;


	//カメラの回転速度
	Vector3 rotateSpd = {};
	//const float kLotateSpd = 0.02f;

	//if (input_->PushKey(DIK_U)) {
	//	rotateSpd.x += kLotateSpd;
	//}
	//if (input_->PushKey(DIK_I)) {
	//	rotateSpd.x -= kLotateSpd;
	//}
	//if (input_->PushKey(DIK_Y)){
	//	rotateSpd.y += kLotateSpd;
	//}
	//if (input_->PushKey(DIK_R)) {
	//	rotateSpd.y -= kLotateSpd;
	//}

	worldTransform_.rotation_ += rotateSpd;


	//ワールドトランスフォームのワールド行列再計算
	worldTransform_.UpdateMatrixRail();

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//カメラの座標を画面表示する処理
	ImGui::Begin("RailCamera");

	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.02f);

	ImGui::End();
}

void RailCamera::SetParent(const WorldTransform* parent)
{
	worldTransform_.parent_ = parent;
}