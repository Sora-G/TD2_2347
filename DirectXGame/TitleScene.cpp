#include "TitleScene.h"
#include "BGM.h"
#include "Input.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
	delete bgm_;
}

void TitleScene::Initialize()
{
	bgm_ = new BGM();
	bgm_->Initialize("BGM/TitleBGM.mp3");
	input_ = Input::GetInstance();
}

void TitleScene::BeginScene()
{
	bgm_->Play();
	isEnd_ = false;
}

void TitleScene::Update()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		isEnd_ = true;
	}
}

void TitleScene::EndScene()
{
	bgm_->Stop();
}
