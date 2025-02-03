#pragma once
#include "EScene.h"
#include"GameScene.h"
#include"Player.h"

class BGM;
class Input;
class PlayScene
{
public:
	PlayScene();
	~PlayScene();

	void Initialize();
	void BeginScene();
	void Update();
	void Draw();
	void EndScene();
	bool IsEnd() { return isEnd_; }
	EScene GetNextScene();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

private:
	Input* input_;
	BGM* bgm_;
	bool isEnd_;
	EScene nextScene_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//3Dモデルデータ
	Model* playerModel_ = nullptr;

	//3Dモデルデータ
	Model* enemyModel_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;
	RailCamera* railCamera_ = nullptr;

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
};

