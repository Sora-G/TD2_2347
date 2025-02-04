#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include<vector>
#include"Player.h"
#include"DebugCamera.h"
#include"Enemy.h"
#include"Skydome.h"
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

	//大親分による判定
	//衝突判定と応答
	void CheckAllCollisions();

	bool IsFinished() const { return isFinished; }

	bool IsClear() const { return isClear; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle = 0;


	// 3Dモデルデータ
	Model* model_ = nullptr;


	//自キャラ
	Player* player_ = nullptr;

	//敵
	Enemy* enemy_ = nullptr;


	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバックカメラ有効
	bool isDebugCameraActive_ = false;

	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	//天球
	Skydome* skyDome_ = nullptr;

	// 天球モデル
	Model* modelSkydome = nullptr;

	//終了フラグ
	bool isFinished = false;

	bool isClear = false;

	// 音
	uint32_t music;

	// 音声再生
	uint32_t playMusic;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};