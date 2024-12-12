#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

/// <summary>
/// 天球
/// </summary>
class SpeceSphere {

public:
	// メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	SpeceSphere();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SpeceSphere();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデルデータ
	Model* modelSykDome_ = nullptr;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;
};
