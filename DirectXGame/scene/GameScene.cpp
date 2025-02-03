#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <math.h>

#include "TitleScene.h"
#include "PlayScene.h"
#include "OverScene.h"
#include "ClearScene.h"
#include "InfoScene.h"

#define DEBUG

GameScene::GameScene() {}

GameScene::~GameScene() {
	
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();



	//wavファイルを読み込んで追加
	bgmHandle_ = audio_->LoadWave("sound/n003.wav");
	playBGM_ = audio_->PlayWave(bgmHandle_, true);

	titleScene_ = new TitleScene();
	titleScene_->Initialize();
	currentScene_ = EScene::Initialize;
	infoScene_ = new InfoScene();
	infoScene_->Initialize();
	playScene_ = new PlayScene();
	playScene_->Initialize();
	overScene_ = new OverScene();
	overScene_->Initialize();
	clearScene_ = new ClearScene();
	clearScene_->Initialize();


}

void GameScene::Update() {


	switch (currentScene_)
	{
	case EScene::Initialize:
		titleScene_->BeginScene();
		currentScene_ = EScene::TitleScene;
		break;
	case EScene::TitleScene:
		titleScene_->Update();
		if (titleScene_->IsEnd())
		{
			titleScene_->EndScene();
			infoScene_->BeginScene();
			currentScene_ = EScene::InfoScene;
		}
		break;
	case EScene::InfoScene:
		infoScene_->Update();
		if (infoScene_->IsEnd()) {
			infoScene_->EndScene();
			currentScene_ = EScene::PlayScene;
		}
		break;
	case EScene::PlayScene:
		playScene_->Update();
		if (playScene_->IsEnd()) {
			playScene_->EndScene();

			currentScene_ = playScene_->GetNextScene();
		}

		break;
	case EScene::OverScene:
		overScene_->Update();
		if (overScene_->IsEnd()) {
			overScene_->EndScene();

		}
		break;
	case EScene::ClearScene:
		clearScene_->Update();
		if (clearScene_->IsEnd()) {
			clearScene_->EndScene();
			currentScene_ = EScene::TitleScene;
		}
		break;
	default:
		break;
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

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	playScene_->Draw();
	

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