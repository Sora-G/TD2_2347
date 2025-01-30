#pragma once
#include "Input.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class RailCamera {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 worldAxis, Vector3 radian);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクションを取得</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	void SetParent(const WorldTransform* parent);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;

	// キーボード入力
	Input* input_ = nullptr;

	// レールカメラ
	bool isDebugCameraActive_ = false;

	//初期座標
	Vector3 saveStartPos;
};