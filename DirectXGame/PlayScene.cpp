#include "PlayScene.h"
#include "BGM.h"
#include "Input.h"
#include"RailCamera.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include"Time.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
	delete bgm_;
	delete playerModel_;
	delete player_;
	delete enemyModel_;
	delete enemy_;
	delete modelSkydome_;


	delete skydome_;
	delete railCamera_;
	delete debugCamera_;
}

void PlayScene::Initialize()
{
	bgm_ = new BGM();
	bgm_->Initialize("BGM/PlayBGM.mp3");
	input_ = Input::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("ufoTexture.png");

	//3Dモデルの読み込み
	playerModel_ = Model::CreateFromOBJ("ufo", true);
	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	Vector3 playerPosition{ 0.0f, 0.0f, 40.0f };
	player_->Initialize(playerModel_, textureHandle_, playerPosition);
	//自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	//3Dモデルの読み込み
	enemyModel_ = Model::CreateFromOBJ("Earth", true);

	//敵キャラの生成
	enemy_ = new Enemy();
	//敵キャラの初期化
	enemy_->Initialize(enemyModel_, Vector3(5.0f, 0.0f, 50.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("Spece-Sphere", true);

	//ビュープロジェクションの初期化
	viewProjection_.farZ = 300.0f;
	viewProjection_.Initialize();





	//天球の生成
	skydome_ = new Skydome();
	//天球の初期化
	skydome_->Initialize(modelSkydome_);

	//レールカメラの生成
	railCamera_ = new RailCamera();
	//レールカメラの初期化
	railCamera_->Initialize(Vector3(0.0f, 0.0f, -300.0f), Vector3(0.0f, 0.0f, 0.0f));



	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が表示するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

}

void PlayScene::BeginScene()
{
	bgm_->Play();
	isEnd_ = false;
}

void PlayScene::Update()
{
	//自キャラの更新処理
	player_->Update();

	//敵キャラの更新所理
	enemy_->Update();


	//天球の更新処理
	skydome_->Update();

	//衝突判定
	CheckAllCollisions();

	//デバッグカメラの更新
	debugCamera_->Update();

#ifdef DEBUG

	if (input_->TriggerKey(DIK_Z)) {
		isDebugCameraActive_ = true;
	}
	if (input_->TriggerKey(DIK_X)) {
		isDebugCameraActive_ = false;
	}
	if (input_->TriggerKey(DIK_C)) {
		viewProjection_.Initialize();
	}

#endif // DEBUG

	//カメラの処理(レール)
	if (isDebugCameraActive_ == true) {
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
	else {
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}

	if (Time::IsFinish)
	{
		nextScene_ = EScene::ClearScene;
		isEnd_ = true;
	}
	if (isDead_ == true)
	{
		nextScene_ = EScene::OverScene;

		isEnd_ = true;
	}
}

void PlayScene::Draw()
{
	//自キャラの描画
	player_->Draw(viewProjection_);

	//敵キャラの描画
	enemy_->Draw(viewProjection_);

	//天球の描画
	skydome_->Draw(viewProjection_);
}

void PlayScene::EndScene()
{
	bgm_->Stop();
}
EScene PlayScene::GetNextScene()
{
	return nextScene_;
}

void PlayScene::CheckAllCollisions()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定

	//自キャラの座標
	posA = player_->GetWorldPosition();

	for (EnemyBullet* enemyBullet : enemyBullets) {
		//敵弾の座標
		posB = enemyBullet->GetWorldPosition();

		//座標AとBの距離を求める
		float posA2posB = powf(posB.x - posA.x, 2.0f) + powf(posB.y - posA.y, 2.0f) + powf(posB.z - posA.z, 2.0f);

		//敵と弾の交差判定
		if (posA2posB <= powf(enemyBullet->rad + player_->rad, 2.0f)) {
			//自キャラの衝突時コールバック関数を呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバック関数を呼び出す
			enemyBullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	//敵キャラの座標
	posA = enemy_->GetWorldPosition();

	for (PlayerBullet* playerBullet : playerBullets) {
		// 自弾の座標
		posB = playerBullet->GetWorldPosition();

		// 座標AとBの距離を求める
		float posA2posB = powf(posB.x - posA.x, 2.0f) + powf(posB.y - posA.y, 2.0f) + powf(posB.z - posA.z, 2.0f);

		// 敵と弾の交差判定
		if (posA2posB <= powf(playerBullet->rad + enemy_->rad, 2.0f)) {
			// 敵キャラの衝突時コールバック関数を呼び出す
			enemy_->OnCollision();
			// 自弾の衝突時コールバック関数を呼び出す
			playerBullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	for (PlayerBullet* playerBullet : playerBullets) {
		for (EnemyBullet* enemyBullet : enemyBullets) {
			// 自弾の座標
			posA = playerBullet->GetWorldPosition();
			// 敵弾の座標
			posB = enemyBullet->GetWorldPosition();

			// 座標AとBの距離を求める
			float posA2posB = powf(posB.x - posA.x, 2.0f) + powf(posB.y - posA.y, 2.0f) + powf(posB.z - posA.z, 2.0f);

			// 敵と弾の交差判定
			if (posA2posB <= powf(playerBullet->rad + enemyBullet->rad, 2.0f)) {
				// 自弾の衝突時コールバック関数を呼び出す
				playerBullet->OnCollision();
				// 敵弾の衝突時コールバック関数を呼び出す
				enemyBullet->OnCollision();
			}
		}
	}

#pragma endregion
}
