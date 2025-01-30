#include "RailCamera.h"
#include "MathUtilityForText.h"
#include "imgui.h"

void RailCamera::Initialize(Vector3 worldAxis, Vector3 radian) {

	saveStartPos = worldAxis;

	// ワールドトランスフォームの初期設定
	worldTransform_.translation_ = worldAxis;
	worldTransform_.rotation_ = radian;

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 700.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update() {

	// カメラの移動速度
	Vector3 moveSpd = {};
	const float kMoveSpd = 0.5f;
	worldTransform_.translation_ += moveSpd;

	// カメラの回転速度
	Vector3 rotateSpd = {};
	worldTransform_.rotation_ += rotateSpd;

	// ワールドトランスフォームのワールド行列再計算
	worldTransform_.UpdateMatrixRail();

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	moveSpd.z = kMoveSpd;

	worldTransform_.translation_ += moveSpd;

	if (worldTransform_.translation_.z >= 100.0f) {
		worldTransform_.translation_.z = saveStartPos.z;
	}

	// カメラの座標を画面表示する処理
	ImGui::Begin("RailCamera");

	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.02f);

	ImGui::End();


}

void RailCamera::SetParent(const WorldTransform* parent)
{
	worldTransform_.parent_ = parent;
}