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

	//敵のモデルデータ
	Model* modelEnemy_ = nullptr;
	Model* modelEnemy2_ = nullptr;
	Model* modelEnemy3_ = nullptr;
	Model* modelEnemy4_ = nullptr;
	Model* modelEnemy5_ = nullptr;
	Model* modelEnemy6_ = nullptr;

	// テクスチャ読み込み
	modelEnemy_ = Model::CreateFromOBJ("Venus", true);
	modelEnemy2_ = Model::CreateFromOBJ("Sun", true);
	modelEnemy3_ = Model::CreateFromOBJ("Mars", true);
	modelEnemy4_ = Model::CreateFromOBJ("Moon", true);
	modelEnemy5_ = Model::CreateFromOBJ("Jupiter", true);
	modelEnemy6_ = Model::CreateFromOBJ("Earth", true);


	enemy_->Initialize(modelEnemy_, Vector3{ 10.0f, 0.0f, 10.0f }, Vector3{ 0.0f, 0.0f, -0.1f });

	enemy2_->Initialize(modelEnemy2_, Vector3{ 10.0f, 2.0f, 10.0f }, Vector3{ 0.0f, 0.0f, 0.1f });

	enemy3_->Initialize(modelEnemy3_, Vector3{ 10.0f, 4.0f, 10.0f }, Vector3{ 0.0f, 0.0f, 0.0f });

	enemy4_->Initialize(modelEnemy4_, Vector3{ 10.0f, 6.0f, 10.0f }, Vector3{ 0.0f, 1.0f, 0.1f });

	enemy5_->Initialize(modelEnemy5_, Vector3{ 10.0f, -2.0f, 10.0f }, Vector3{ 1.0f, 0.0f, -0.1f });

	enemy6_->Initialize(modelEnemy6_, Vector3{ 10.0f, -4.0f, 10.0f }, Vector3{ 1.0f, 0.0f, 0.1f });


	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	enemy2_->SetPlayer(player_);

	enemy3_->SetPlayer(player_);

	enemy4_->SetPlayer(player_);

	enemy5_->SetPlayer(player_);

	enemy6_->SetPlayer(player_);

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

	enemy2_->Update();

	enemy3_->Update();

	enemy4_->Update();

	enemy5_->Update();

	enemy6_->Update();

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

	enemy2_->Draw(viewProjection_);

	enemy3_->Draw(viewProjection_);

	enemy4_->Draw(viewProjection_);

	enemy5_->Draw(viewProjection_);

	enemy6_->Draw(viewProjection_);

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
	Vector3 posA2, posB2;
	Vector3 posA3, posB3;
	Vector3 posA4, posB4;
	Vector3 posA5, posB5;
	Vector3 posA6, posB6;


	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<PlayerBullet*>& playerBullets2 = player_->GetBullets();
	const std::list<PlayerBullet*>& playerBullets3 = player_->GetBullets();
	const std::list<PlayerBullet*>& playerBullets4 = player_->GetBullets();
	const std::list<PlayerBullet*>& playerBullets5 = player_->GetBullets();
	const std::list<PlayerBullet*>& playerBullets6 = player_->GetBullets();

	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetEnemyBullets();
	const std::list<EnemyBullet*>& enemyBullets2 = enemy2_->GetEnemyBullets();
	const std::list<EnemyBullet*>& enemyBullets3 = enemy3_->GetEnemyBullets();
	const std::list<EnemyBullet*>& enemyBullets4 = enemy4_->GetEnemyBullets();
	const std::list<EnemyBullet*>& enemyBullets5 = enemy5_->GetEnemyBullets();
	const std::list<EnemyBullet*>& enemyBullets6 = enemy6_->GetEnemyBullets();


#pragma region 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾の当たり判定1
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

	// 自キャラと敵弾の当たり判定2
	for (EnemyBullet* bullet_ : enemyBullets2) {

		// 敵弾の座標
		posB = bullet_->GetWorldPosition();

		// 座標AとBの距離を求める
		float DistancePosAPosB2 = powf(posB.x - posA.x, 2.0) + powf(posB.y - posA.y, 2.0) + powf(posB.z - posA.z, 2.0);

		// 弾と弾の交差判定
		if (DistancePosAPosB2 <= powf(bullet_->EnemyBulletRad + player_->PlayerRad, 2.0)) {

			// 自キャラの衝突時のコールバックを呼び出す
			player_->OnCollision();

			// 敵弾の衝突時のコールバックを呼び出す
			bullet_->OnCollision();
		}
	}

	// 自キャラと敵弾の当たり判定3
	for (EnemyBullet* bullet_ : enemyBullets3) {

		// 敵弾の座標
		posB = bullet_->GetWorldPosition();

		// 座標AとBの距離を求める
		float DistancePosAPosB3 = powf(posB.x - posA.x, 2.0) + powf(posB.y - posA.y, 2.0) + powf(posB.z - posA.z, 2.0);

		// 弾と弾の交差判定
		if (DistancePosAPosB3 <= powf(bullet_->EnemyBulletRad + player_->PlayerRad, 2.0)) {

			// 自キャラの衝突時のコールバックを呼び出す
			player_->OnCollision();

			// 敵弾の衝突時のコールバックを呼び出す
			bullet_->OnCollision();
		}
	}

	// 自キャラと敵弾の当たり判定4
	for (EnemyBullet* bullet_ : enemyBullets4) {

		// 敵弾の座標
		posB = bullet_->GetWorldPosition();

		// 座標AとBの距離を求める
		float DistancePosAPosB4 = powf(posB.x - posA.x, 2.0) + powf(posB.y - posA.y, 2.0) + powf(posB.z - posA.z, 2.0);

		// 弾と弾の交差判定
		if (DistancePosAPosB4 <= powf(bullet_->EnemyBulletRad + player_->PlayerRad, 2.0)) {

			// 自キャラの衝突時のコールバックを呼び出す
			player_->OnCollision();

			// 敵弾の衝突時のコールバックを呼び出す
			bullet_->OnCollision();
		}
	}

	// 自キャラと敵弾の当たり判定5
	for (EnemyBullet* bullet_ : enemyBullets5) {

		// 敵弾の座標
		posB = bullet_->GetWorldPosition();

		// 座標AとBの距離を求める
		float DistancePosAPosB5 = powf(posB.x - posA.x, 2.0) + powf(posB.y - posA.y, 2.0) + powf(posB.z - posA.z, 2.0);

		// 弾と弾の交差判定
		if (DistancePosAPosB5 <= powf(bullet_->EnemyBulletRad + player_->PlayerRad, 2.0)) {

			// 自キャラの衝突時のコールバックを呼び出す
			player_->OnCollision();

			// 敵弾の衝突時のコールバックを呼び出す
			bullet_->OnCollision();
		}
	}

	// 自キャラと敵弾の当たり判定6
	for (EnemyBullet* bullet_ : enemyBullets6) {

		// 敵弾の座標
		posB = bullet_->GetWorldPosition();

		// 座標AとBの距離を求める
		float DistancePosAPosB6 = powf(posB.x - posA.x, 2.0) + powf(posB.y - posA.y, 2.0) + powf(posB.z - posA.z, 2.0);

		// 弾と弾の交差判定
		if (DistancePosAPosB6 <= powf(bullet_->EnemyBulletRad + player_->PlayerRad, 2.0)) {

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
	posA2 = enemy2_->GetWorldPosition();
	posA3 = enemy3_->GetWorldPosition();
	posA4 = enemy4_->GetWorldPosition();
	posA5 = enemy5_->GetWorldPosition();
	posA6 = enemy6_->GetWorldPosition();

	// 敵キャラと自弾の当たり判定1
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

	// 敵キャラと自弾の当たり判定2
	for (PlayerBullet* PlayerBullet2_ : playerBullets2) {

		// 自弾の座標
		posB2 = PlayerBullet2_->GetWorldPosition();

		// 座標AとBの距離を求める
		float DistancePosAPosB2 = powf(posB2.x - posA2.x, 2.0) + powf(posB2.y - posA2.y, 2.0) + powf(posB2.z - posA2.z, 2.0);

		// 弾と弾の交差判定
		if (DistancePosAPosB2 <= powf(PlayerBullet2_->PlayerBulletRad + enemy2_->EnemyRad, 2.0)) {

			// 敵キャラの衝突時のコールバックを呼び出す
			enemy2_->OnCollision();

			// 自弾の衝突時のコールバックを呼び出す
			PlayerBullet2_->OnCollision();
		}
	}

	// 敵キャラと自弾の当たり判定3
	for (PlayerBullet* PlayerBullet3_ : playerBullets3) {

		// 自弾の座標
		posB3 = PlayerBullet3_->GetWorldPosition();

		// 座標AとBの距離を求める
		float DistancePosAPosB3 = powf(posB3.x - posA3.x, 2.0) + powf(posB3.y - posA3.y, 2.0) + powf(posB3.z - posA3.z, 2.0);

		// 弾と弾の交差判定
		if (DistancePosAPosB3 <= powf(PlayerBullet3_->PlayerBulletRad + enemy3_->EnemyRad, 2.0)) {

			// 敵キャラの衝突時のコールバックを呼び出す
			enemy3_->OnCollision();

			// 自弾の衝突時のコールバックを呼び出す
			PlayerBullet3_->OnCollision();
		}
	}

	// 敵キャラと自弾の当たり判定4
	for (PlayerBullet* PlayerBullet4_ : playerBullets4) {

		// 自弾の座標
		posB4 = PlayerBullet4_->GetWorldPosition();

		// 座標AとBの距離を求める
		float DistancePosAPosB4 = powf(posB4.x - posA4.x, 2.0) + powf(posB4.y - posA4.y, 2.0) + powf(posB4.z - posA4.z, 2.0);

		// 弾と弾の交差判定
		if (DistancePosAPosB4 <= powf(PlayerBullet4_->PlayerBulletRad + enemy4_->EnemyRad, 2.0)) {

			// 敵キャラの衝突時のコールバックを呼び出す
			enemy4_->OnCollision();

			// 自弾の衝突時のコールバックを呼び出す
			PlayerBullet4_->OnCollision();
		}
	}

	// 敵キャラと自弾の当たり判定5
	for (PlayerBullet* PlayerBullet5_ : playerBullets5) {

		// 自弾の座標
		posB5 = PlayerBullet5_->GetWorldPosition();

		// 座標AとBの距離を求める
		float DistancePosAPosB5 = powf(posB5.x - posA5.x, 2.0) + powf(posB5.y - posA5.y, 2.0) + powf(posB5.z - posA5.z, 2.0);

		// 弾と弾の交差判定
		if (DistancePosAPosB5 <= powf(PlayerBullet5_->PlayerBulletRad + enemy5_->EnemyRad, 2.0)) {

			// 敵キャラの衝突時のコールバックを呼び出す
			enemy5_->OnCollision();

			// 自弾の衝突時のコールバックを呼び出す
			PlayerBullet5_->OnCollision();
		}
	}

	// 敵キャラと自弾の当たり判定6
	for (PlayerBullet* PlayerBullet6_ : playerBullets6) {

		// 自弾の座標
		posB6 = PlayerBullet6_->GetWorldPosition();

		// 座標AとBの距離を求める
		float DistancePosAPosB6 = powf(posB6.x - posA6.x, 2.0) + powf(posB6.y - posA6.y, 2.0) + powf(posB6.z - posA6.z, 2.0);

		// 弾と弾の交差判定
		if (DistancePosAPosB6 <= powf(PlayerBullet6_->PlayerBulletRad + enemy6_->EnemyRad, 2.0)) {

			// 敵キャラの衝突時のコールバックを呼び出す
			enemy6_->OnCollision();

			// 自弾の衝突時のコールバックを呼び出す
			PlayerBullet6_->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	// 敵キャラと自弾の当たり判定1
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

	// 敵キャラと自弾の当たり判定2
	for (PlayerBullet* PlayerBullet2_ : playerBullets2) {

		for (EnemyBullet* EnemyBullet2_ : enemyBullets2) {

			// 自弾の座標
			posA2 = PlayerBullet2_->GetWorldPosition();

			// 敵弾の座標
			posB2 = EnemyBullet2_->GetWorldPosition();

			// 座標AとBの距離を求める
			float DistancePosAPosB2 = powf(posB2.x - posA2.x, 2.0) + powf(posB2.y - posA2.y, 2.0) + powf(posB2.z - posA2.z, 2.0);

			// 弾と弾の交差判定
			if (DistancePosAPosB2 <= powf(PlayerBullet2_->PlayerBulletRad + EnemyBullet2_->EnemyBulletRad, 2.0)) {

				// 敵キャラの衝突時のコールバックを呼び出す
				EnemyBullet2_->OnCollision();

				// 自弾の衝突時のコールバックを呼び出す
				PlayerBullet2_->OnCollision();
			}
		}
	}

	// 敵キャラと自弾の当たり判定3
	for (PlayerBullet* PlayerBullet3_ : playerBullets3) {

		for (EnemyBullet* EnemyBullet3_ : enemyBullets3) {

			// 自弾の座標
			posA3 = PlayerBullet3_->GetWorldPosition();

			// 敵弾の座標
			posB3 = EnemyBullet3_->GetWorldPosition();

			// 座標AとBの距離を求める
			float DistancePosAPosB3 = powf(posB3.x - posA3.x, 2.0) + powf(posB3.y - posA3.y, 2.0) + powf(posB3.z - posA3.z, 2.0);

			// 弾と弾の交差判定
			if (DistancePosAPosB3 <= powf(PlayerBullet3_->PlayerBulletRad + EnemyBullet3_->EnemyBulletRad, 2.0)) {

				// 敵キャラの衝突時のコールバックを呼び出す
				EnemyBullet3_->OnCollision();

				// 自弾の衝突時のコールバックを呼び出す
				PlayerBullet3_->OnCollision();
			}
		}
	}

	// 敵キャラと自弾の当たり判定4
	for (PlayerBullet* PlayerBullet4_ : playerBullets4) {

		for (EnemyBullet* EnemyBullet4_ : enemyBullets4) {

			// 自弾の座標
			posA4 = PlayerBullet4_->GetWorldPosition();

			// 敵弾の座標
			posB4 = EnemyBullet4_->GetWorldPosition();

			// 座標AとBの距離を求める
			float DistancePosAPosB4 = powf(posB4.x - posA4.x, 2.0) + powf(posB4.y - posA4.y, 2.0) + powf(posB4.z - posA4.z, 2.0);

			// 弾と弾の交差判定
			if (DistancePosAPosB4 <= powf(PlayerBullet4_->PlayerBulletRad + EnemyBullet4_->EnemyBulletRad, 2.0)) {

				// 敵キャラの衝突時のコールバックを呼び出す
				EnemyBullet4_->OnCollision();

				// 自弾の衝突時のコールバックを呼び出す
				PlayerBullet4_->OnCollision();
			}
		}
	}

	// 敵キャラと自弾の当たり判定5
	for (PlayerBullet* PlayerBullet5_ : playerBullets5) {

		for (EnemyBullet* EnemyBullet5_ : enemyBullets5) {

			// 自弾の座標
			posA5 = PlayerBullet5_->GetWorldPosition();

			// 敵弾の座標
			posB5 = EnemyBullet5_->GetWorldPosition();

			// 座標AとBの距離を求める
			float DistancePosAPosB5 = powf(posB5.x - posA5.x, 2.0) + powf(posB5.y - posA5.y, 2.0) + powf(posB5.z - posA5.z, 2.0);

			// 弾と弾の交差判定
			if (DistancePosAPosB5 <= powf(PlayerBullet5_->PlayerBulletRad + EnemyBullet5_->EnemyBulletRad, 2.0)) {

				// 敵キャラの衝突時のコールバックを呼び出す
				EnemyBullet5_->OnCollision();

				// 自弾の衝突時のコールバックを呼び出す
				PlayerBullet5_->OnCollision();
			}
		}
	}

	// 敵キャラと自弾の当たり判定6
	for (PlayerBullet* PlayerBullet6_ : playerBullets6) {

		for (EnemyBullet* EnemyBullet6_ : enemyBullets6) {

			// 自弾の座標
			posA6 = PlayerBullet6_->GetWorldPosition();

			// 敵弾の座標
			posB6 = EnemyBullet6_->GetWorldPosition();

			// 座標AとBの距離を求める
			float DistancePosAPosB6 = powf(posB6.x - posA6.x, 2.0) + powf(posB6.y - posA6.y, 2.0) + powf(posB6.z - posA6.z, 2.0);

			// 弾と弾の交差判定
			if (DistancePosAPosB6 <= powf(PlayerBullet6_->PlayerBulletRad + EnemyBullet6_->EnemyBulletRad, 2.0)) {

				// 敵キャラの衝突時のコールバックを呼び出す
				EnemyBullet6_->OnCollision();

				// 自弾の衝突時のコールバックを呼び出す
				PlayerBullet6_->OnCollision();
			}
		}
	}

#pragma endregion
}
