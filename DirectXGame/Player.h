#pragma once
#include "Model.h"
#include"WorldTransform.h"
#include<Input.h>
#include"PlayerBullet.h"
#include<List>
/// <summary>
///	自キャラ
/// </summary>
class Player {

public://引数を書くところ
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);//void Initialize(Model* model, ViewProjection* viewProjection);


	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	void Rotate();

	//void Move();

	void Attack();

	~Player();

	// 自キャラのワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<std::shared_ptr<PlayerBullet>>& GetBullets() const { return bullets_; }

	float GetRadius() { return radius_; }

	int GetHp() { return hp_; }

private://関数（メンバ変数）


	//ワールド変換データ
	WorldTransform worldTransform_;


	// モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//キーボード入力
	Input* input_ = nullptr;

	//弾
	std::list<std::shared_ptr<PlayerBullet>> bullets_;

	float radius_ = 0.5f;

	int hp_ = 1;
	//ViewProjection* viewProjection_ = nullptr;
};