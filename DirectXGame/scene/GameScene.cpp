#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

#define DEBUG

GameScene::GameScene() {}

GameScene::~GameScene() {
	//プレイヤーの解放
	delete playerModel_;
	delete player_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//プレイヤーのモデルの読み込み
	playerModel_ = Model::CreateFromOBJ("Player");

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//プレイヤーの生成
	player_ = new Player();
	//プレイヤーの初期化
	player_->Initialize(playerModel_);


	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が表示するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {

	//プレイヤーの更新処理
	player_->Update();

	//カメラ処理
	DebugCameraProc();
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

	//プレイヤーの描画
	player_->Draw(viewProjection_);

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

void GameScene::DebugCameraProc()
{
#ifdef DEBUG

	//キー操作でカメラのアクティブを切り替える
	if (input_->TriggerKey(DIK_C))
	{
		isDebugCameraActive_ = true;
	}
	if (input_->TriggerKey(DIK_X))
	{
		isDebugCameraActive_ = false;
	}

#endif // DEBUG

	//デバッグカメラがアクティブの時...
	if (isDebugCameraActive_)
	{
		//デバッグカメラの更新処理
		debugCamera_->Update();

		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
	else
	{
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
}
