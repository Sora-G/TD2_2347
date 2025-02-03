#include "OverScene.h"
#include "BGM.h"
#include "Input.h"

OverScene::OverScene()
{

}
OverScene::~OverScene()
{
	delete bgm_;
}

void OverScene::Initialize()
{
	bgm_ = new BGM();
	bgm_->Initialize("BGM/OverBGM.mp3");
	input_ = Input::GetInstance();
}

void OverScene::BeginScene()
{
	bgm_->Play();
	isEnd_ = false;
}

void OverScene::Update()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		isEnd_ = true;
	}
}

void OverScene::EndScene()
{
	bgm_->Stop();
}
