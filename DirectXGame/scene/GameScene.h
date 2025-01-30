#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"DebugCamera.h"
#include"Enemy.h"
#include"SkyDome.h"
#include <list>

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

	//衝突判定と応答
	void CheckAllCollisions();

	void AddPlayerBullet(PlayerBullet* playerBullet);

	void AddEnemyBullet(EnemyBullet* enemyBullet);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>

	/// ゲームシーン用
	uint32_t textureHandle_ = 0;

	// 3Dモデルの生成
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 自キャラ
	Player* player_ = nullptr;

	//敵
	Enemy* enemy_ = nullptr;
	//敵
	std::list<Enemy*> enemies_ = {};
	// 弾
	std::list<EnemyBullet*> Enemybullets_;


	// 弾
	std::list<PlayerBullet*> Playerbullets_ = {};

	// デバッグカメラ
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	// 天球
	Skydome* skyDome_ = nullptr;

	// 天球モデルデータ
	Model* modelSkydome_ = nullptr;

	
	// 判定対象AとBの座標
	Vector3 posA, posB;

	/// </summary>
};