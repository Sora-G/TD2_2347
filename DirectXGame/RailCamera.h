#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"


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

	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns>ワールドトランスフォームを取得</returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent">親となるワールドトランスフォームをセット</param>
	void SetParent(const WorldTransform* parent);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//キーボード入力
	Input* input_ = nullptr;
};
