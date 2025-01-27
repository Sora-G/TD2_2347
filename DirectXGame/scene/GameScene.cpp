#include "GameScene.h"
#include "AxisIndicator.h"
#include "SkyDome.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

// デストラクタ

GameScene::~GameScene() {
	delete model_;

	delete player_;

	delete debugCamera_;

	for (Enemy* e : enemies_)
	{
		delete e;
	}
	enemies_.clear();

	// 天球
	delete skyDome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// テクスチャ読み込み
	// textureHandle_ = TextureManager::Load("illustration.png");

	// 3Dモデルの生成
	model_ = Model::CreateFromOBJ("player", true);

	// ビュープロジェクション
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();

	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	// 軸方向表示が参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 敵の初期化&生成

	//敵のモデルデータ
	Model* modelEnemy_[8];

	// テクスチャ読み込み
	modelEnemy_[0] = Model::CreateFromOBJ("Venus", true);
	modelEnemy_[1] = Model::CreateFromOBJ("Sun", true);
	modelEnemy_[2] = Model::CreateFromOBJ("Mars", true);
	modelEnemy_[3] = Model::CreateFromOBJ("Moon", true);
	modelEnemy_[4] = Model::CreateFromOBJ("Jupiter", true);
	modelEnemy_[5] = Model::CreateFromOBJ("Earth", true);
	modelEnemy_[6] = Model::CreateFromOBJ("Spiral", true);
	modelEnemy_[7] = Model::CreateFromOBJ("SoftCream", true);

	for (int i = 0; i <= 7; i++)
	{

		Enemy* enemy = new Enemy();
		enemy->Initialize(modelEnemy_[i], Vector3{ 10.0f + i, 0.0f, 0.0f + i }, Vector3{ 0.0f, 0.0f, -0.1f + i });
		enemies_.push_back(enemy);

	}

	for (Enemy* e : enemies_) {
		e->SetPlayer(player_);
	}

	// 天球の生成
	modelSkydome_ = Model::CreateFromOBJ("Spece-Sphere", true);

	// 天球
	skyDome_ = new Skydome();

	// 天球の初期化
	skyDome_->Initialize(modelSkydome_, &viewProjection_);

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 700;
	viewProjection_.Initialize();
}

void GameScene::Update() {

	// 自キャラの更新
	player_->Update();

	// デバッグカメラの更新
	debugCamera_->Update();

	// 敵の更新

	for (Enemy* e : enemies_) {
		e->Update();
	}

	// 当たり判定
	CheckAllCollisions();

	// 天球の更新
	skyDome_->Update();

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = true;
	}

	if (input_->TriggerKey(DIK_1)) {
		isDebugCameraActive_ = false;
	}

#endif // _DEBUG

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
	else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// スカイドームの辨官
	skyDome_->Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	player_->Draw(viewProjection_);

	for (Enemy* e : enemies_) {
		e->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {

#pragma region 自キャラと敵キャラの当たり判定

	for (Enemy* e : enemies_)
	{

		CheckCollision(player_, e);
	}

#pragma endregion

#pragma region 自キャラと敵弾の当たり判定

	for (EnemyBullet* e : Enemybullets_)
	{
		CheckCollision(player_, e);
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	for (Enemy* e : enemies_)
	{
		for (PlayerBullet* pB : Playerbullets_)
		{
			CheckCollision(e, pB);
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	for (EnemyBullet* eB : Enemybullets_)
	{
		for (PlayerBullet* pB : Playerbullets_)
		{
			CheckCollision(eB, pB);
		}
	}

#pragma endregion

}


