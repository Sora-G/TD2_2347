#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"

Vector3 operator-(const Vector3& a, const Vector3& b) {
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}




GameScene::GameScene() {}

GameScene::~GameScene() {
	// デストラクタ

	//3Dモデルデータの開放
	delete model_;

	//自キャラの解放
	delete player_;

	//敵の解放
	delete enemy_;

	//カメラの解放
	delete debugCamera_;

	//天球の解放
	delete skyDome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();


	//ファイル名を指定してテクスチャを読み込む
	textureHandle = TextureManager::Load("ufo/ufoTexture.png");

	// 音
	music = audio_->LoadWave("sound/n003.wav");

	audio_->PauseWave(music);

	// 音声再生
	playMusic = audio_->PlayWave(music, true);


	//3Dモデルデータの生成
	model_ = Model::Create();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//自キャラの生成
	player_ = new Player();

	//自キャラの初期化
	player_->Initialize(model_, textureHandle);

	//敵の生成
	enemy_ = new Enemy();

	//敵の初期化(ここをいじれば敵の位置が変わる）
	enemy_->Initialize(model_, Vector3(0.0f, 6.0f, 50.0f), Vector3(0.0f, 0.0f, -1.0f));

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);

	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	// 天球モデルの生成
	modelSkydome = Model::CreateFromOBJ("Spece-Sphere", true);
	skyDome_ = new Skydome();
	skyDome_->Initialize(modelSkydome, &viewProjection_);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	//敵の更新
	enemy_->Update();

	CheckAllCollisions();

	// デバックカメラの更新
	debugCamera_->Update();

	// 天球の更新
	skyDome_->Update();

	if (enemy_->GetHp() <= 0) {
		isFinished = true;
		isClear = true;

		audio_->StopWave(playMusic);
	}
	if (player_->GetHp() <= 0) {
		isFinished = true;

		audio_->StopWave(playMusic);
	}

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = true;
	}
#endif

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

	//自キャラの描画
	player_->Draw(viewProjection_);

	//敵の描画
	enemy_->Draw(viewProjection_);
	// 天球の描画
	skyDome_->Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

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

void GameScene::CheckAllCollisions()
{
	Vector3 posA, posB;


	Vector3 A2B;
	float len;
	float radius;
	//自弾リストの取得
	const std::list<std::shared_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

	// 敵弾リストの取得
	const std::list<std::shared_ptr<EnemyBullet>>& enemyBullets = enemy_->GetEnemyBullets();


	// 自キャラと敵弾全ての当たり判定
	for (std::shared_ptr<EnemyBullet> bullet : enemyBullets)
	{
		// 自キャラの座標
		posA = player_->GetWorldPosition();

		//敵弾の座標
		posB = bullet->GetWorldPosition();

		//座標AとBの距離
		A2B = posA - posB;
		len = Length(A2B);
		radius = player_->GetRadius() + bullet->GetRadius();

		//球と球の交差判定
		if (len <= radius)
		{
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

	for (std::shared_ptr<PlayerBullet> bullet : playerBullets) {
		// 自キャラの座標
		posA = enemy_->GetWorldPosition();

		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 座標AとBの距離
		A2B = posA - posB;
		len = Length(A2B);
		radius = enemy_->GetRadius() + bullet->GetRadius();

		// 球と球の交差判定
		if (len <= radius) {
			// 自キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

	for (std::shared_ptr<PlayerBullet> bullet : playerBullets) {
		for (std::shared_ptr<EnemyBullet> bullet2 : enemyBullets) {
			// 自キャラの座標
			posA = bullet2->GetWorldPosition();

			// 敵弾の座標
			posB = bullet->GetWorldPosition();

			// 座標AとBの距離
			A2B = posA - posB;
			len = Length(A2B);
			radius = bullet2->GetRadius() + bullet->GetRadius();

			// 球と球の交差判定
			if (len <= radius) {
				// 自キャラの衝突時コールバックを呼び出す
				bullet2->OnCollision();
				// 敵弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}

}