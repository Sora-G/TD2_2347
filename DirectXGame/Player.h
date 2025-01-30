#pragma once

#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "WorldTransform.h"
#include "imgui.h"
#include <list>

// GameSceneの前方宣言
class GameScene;

class Player {

public:
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	/// <param name="textureHandle"></param>
	/// <param name="viewProjection"></param>
	void Initialize(Model* model, uint32_t textureHandle);

	///< summary>
	/// 更新
	///  </summary>
	void Update();

	///< summary>
	/// 描画
	///  </summary>
	void Draw(ViewProjection& viewProjection);

	void Rotate();

	void Attack();

	Vector3 GetWorldPosition();

	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	// 半径
	const float PlayerRad = 1;

	// 親となるワールドトランスフォーム
	void SetParent(const WorldTransform* parent);

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }



private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 自キャラ
	Player* player_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

	// キーボード入力
	Input* input_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;
};