#include "ClearScene.h"
#include "BGM.h"
#include "Input.h"

ClearScene::ClearScene()
{
}

ClearScene::~ClearScene()
{
	delete bgm_;
}

void ClearScene::Initialize()
{
	bgm_ = new BGM();
	bgm_->Initialize("BGM/ClearBGM.mp3");
	input_ = Input::GetInstance();
}

void ClearScene::BeginScene()
{
	bgm_->Play();
	isEnd_ = false;
}

void ClearScene::Update()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		isEnd_ = true;
	}
}

void ClearScene::EndScene()
{
	bgm_->Stop();
}
