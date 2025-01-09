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

	delete enemy_;

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

	// テクスチャ読み込み
	modelEnemy_ = Model::CreateFromOBJ("Venus", true);

	enemy_ = new Enemy();

	enemy_->Initialize(modelEnemy_, Vector3{ 10.0f, 0.0f, 10.0f }, Vector3{ 0.0f, 0.0f, -0.1f });

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

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
	enemy_->Update();

	// 当たり判定
	CheckAllCollisions();

	// 天球の更新
	skyDome_->Update();

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = true;
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

	// 敵の描画
	enemy_->Draw(viewProjection_);

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

	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetEnemyBullets();

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾の当たり判定
	for (EnemyBullet* bullet_ : enemyBullets) {

		// 敵弾の座標
		posB = bullet_->GetWorldPosition();

		// 座標AとBの距離を求める
		float DistancePosAPosB = powf(posB.x - posA.x, 2.0) + powf(posB.y - posA.y, 2.0) + powf(posB.z - posA.z, 2.0);

		// 弾と弾の交差判定
		if (DistancePosAPosB <= powf(bullet_->EnemyBulletRad + player_->PlayerRad, 2.0)) {

			// 自キャラの衝突時のコールバックを呼び出す
			player_->OnCollision();

			// 敵弾の衝突時のコールバックを呼び出す
			bullet_->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	// 敵キャラの座標
	posA = enemy_->GetWorldPosition();

	// 敵キャラと自弾の当たり判定
	for (PlayerBullet* PlayerBullet_ : playerBullets) {

		// 自弾の座標
		posB = PlayerBullet_->GetWorldPosition();

		// 座標AとBの距離を求める
		float DistancePosAPosB = powf(posB.x - posA.x, 2.0) + powf(posB.y - posA.y, 2.0) + powf(posB.z - posA.z, 2.0);

		// 弾と弾の交差判定
		if (DistancePosAPosB <= powf(PlayerBullet_->PlayerBulletRad + enemy_->EnemyRad, 2.0)) {

			// 敵キャラの衝突時のコールバックを呼び出す
			enemy_->OnCollision();

			// 自弾の衝突時のコールバックを呼び出す
			PlayerBullet_->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	// 敵キャラと自弾の当たり判定
	for (PlayerBullet* PlayerBullet_ : playerBullets) {

		for (EnemyBullet* EnemyBullet_ : enemyBullets) {

			// 自弾の座標
			posA = PlayerBullet_->GetWorldPosition();

			// 敵弾の座標
			posB = EnemyBullet_->GetWorldPosition();

			// 座標AとBの距離を求める
			float DistancePosAPosB = powf(posB.x - posA.x, 2.0) + powf(posB.y - posA.y, 2.0) + powf(posB.z - posA.z, 2.0);

			// 弾と弾の交差判定
			if (DistancePosAPosB <= powf(PlayerBullet_->PlayerBulletRad + EnemyBullet_->EnemyBulletRad, 2.0)) {

				// 敵キャラの衝突時のコールバックを呼び出す
				EnemyBullet_->OnCollision();

				// 自弾の衝突時のコールバックを呼び出す
				PlayerBullet_->OnCollision();
			}
		}
	}
#pragma endregion
}
