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

	for (PlayerBullet* pB : Playerbullets_) {
		delete pB;
	}
	Playerbullets_.clear();

	delete debugCamera_;

	for (Enemy* e : enemies_)
	{
		delete e;
	}
	enemies_.clear();

	for (EnemyBullet* eB : Enemybullets_) {
		delete eB;
	}
	Enemybullets_.clear();

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

	player_->SetGameScene(this);

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

	// 敵キャラのゲームシーンを渡す
	for (Enemy* e : enemies_) {
		e->SetGameScene(this);
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

	for (PlayerBullet* pb : Playerbullets_) {
		pb->Update();
	}

	// デバッグカメラの更新
	debugCamera_->Update();

	// 敵の更新

	for (Enemy* e : enemies_) {
		e->Update();
	}

	for (EnemyBullet* eB : Enemybullets_) {
		eB->Update();
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

	Playerbullets_.remove_if([](PlayerBullet* pB) {
		if (pB->IsDead()) {
			delete pB;
			pB = nullptr;
			return true;
		}
		return false;
		});

	Enemybullets_.remove_if([](EnemyBullet* e) {
		if (e->IsDead()) {
			delete e;
			e = nullptr;
			return true;
		}
		return false;
		});

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

	for (PlayerBullet* pb : Playerbullets_) {
		pb->Draw(viewProjection_);
	}

	for (Enemy* e : enemies_) {
		e->Draw(viewProjection_);
	}

	for (EnemyBullet* eB : Enemybullets_) {
		eB->Draw(viewProjection_);
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

#pragma region 自キャラと敵弾の当たり判定

	// 自キャラと敵弾の当たり判定
	for (EnemyBullet* eB : Enemybullets_) {

		// 自キャラの座標
		posA = player_->GetWorldPosition();

		// 敵弾の座標
		posB = eB->GetWorldPosition();

		// 座標AとBの距離を求める
		float DistancePosAPosB = powf(posB.x - posA.x, 2.0) + powf(posB.y - posA.y, 2.0) + powf(posB.z - posA.z, 2.0);

		// 弾と弾の交差判定
		if (DistancePosAPosB <= powf(eB->EnemyBulletRad + player_->PlayerRad, 2.0)) {

			// 自キャラの衝突時のコールバックを呼び出す
			player_->OnCollision();

			// 敵弾の衝突時のコールバックを呼び出す
			eB->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	// 敵キャラと自弾の当たり判定
	for (PlayerBullet* pB : Playerbullets_) {
		// 敵キャラの座標
		for (Enemy* e : enemies_) {
			e->GetWorldPosition();
			// 自弾の座標
			posB = pB->GetWorldPosition();

			// 座標AとBの距離を求める
			float DistancePosAPosB = powf(posB.x - posA.x, 2.0) + powf(posB.y - posA.y, 2.0) + powf(posB.z - posA.z, 2.0);

			// 弾と弾の交差判定
			if (DistancePosAPosB <= powf(pB->PlayerBulletRad + e->EnemyRad, 2.0)) {

				// 敵キャラの衝突時のコールバックを呼び出す

				e->OnCollision();

				// 自弾の衝突時のコールバックを呼び出す
				pB->OnCollision();
			}
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	// 敵キャラと自弾の当たり判定
	for (PlayerBullet* pB : Playerbullets_) {

		for (EnemyBullet* eB : Enemybullets_) {

			// 自弾の座標
			posA = pB->GetWorldPosition();

			// 敵弾の座標
			posB = eB->GetWorldPosition();

			// 座標AとBの距離を求める
			float DistancePosAPosB = powf(posB.x - posA.x, 2.0) + powf(posB.y - posA.y, 2.0) + powf(posB.z - posA.z, 2.0);

			// 弾と弾の交差判定
			if (DistancePosAPosB <= powf(pB->PlayerBulletRad + eB->EnemyBulletRad, 2.0)) {

				// 敵キャラの衝突時のコールバックを呼び出す
				eB->OnCollision();

				// 自弾の衝突時のコールバックを呼び出す
				pB->OnCollision();
			}
		}
	}
#pragma endregion

}

void GameScene::AddPlayerBullet(PlayerBullet* playerBullet) {
	// リストに登録
	Playerbullets_.push_back(playerBullet);
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {

	// リストに登録
	Enemybullets_.push_back(enemyBullet);
}

