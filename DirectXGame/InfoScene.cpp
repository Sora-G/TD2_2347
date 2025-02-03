#include "InfoScene.h"
#include "BGM.h"
#include "Input.h"

InfoScene::InfoScene()
{
}

InfoScene::~InfoScene()
{
	delete bgm_;
}

void InfoScene::Initialize()
{
	bgm_ = new BGM();
	bgm_->Initialize("BGM/TitleBGM.mp3");
	input_ = Input::GetInstance();
}

void InfoScene::BeginScene()
{
	bgm_->Play();
	isEnd_ = false;
}

void InfoScene::Update()
{
	if (input_->TriggerKey(DIK_SPACE))
	{

		isEnd_ = true;
	}

}

void InfoScene::EndScene()
{
	bgm_->Stop();
}

