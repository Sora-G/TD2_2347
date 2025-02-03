#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//3Dモデルデータ
	Model* playerModel_ = nullptr;
	Model* enemyModel_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//自キャラ
	Player* player_ = nullptr;

	// 敵キャラ
	Enemy* enemy_ = nullptr;

	//3Dモデル
	Model* modelSkydome_ = nullptr;

	//天球
	Skydome* skydome_ = nullptr;

	RailCamera* railCamera_ = nullptr;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = true;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//音
	uint32_t bgmHandle_;
	uint32_t playBGM_;

	//時間制限（タイマー)
	uint32_t StegeTimer = 30;

	Model* title = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
