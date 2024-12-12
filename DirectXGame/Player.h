#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"

class Player
{
public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(ViewProjection& viewprojection);

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 旋回処理
	/// </summary>
	void Rotate();

private:

	WorldTransform worldTransform_;		//ワールドトランスフォーム
	Model* model_ = nullptr;			//モデル	
	Input* input_ = nullptr;			//入力
};

