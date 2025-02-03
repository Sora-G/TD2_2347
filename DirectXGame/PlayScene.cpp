#include "PlayScene.h"
#include "BGM.h"
#include "Input.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
	delete bgm_;
}

void PlayScene::Initialize()
{
	bgm_ = new BGM();
	bgm_->Initialize("BGM/PlayBGM.mp3");
	input_ = Input::GetInstance();
}

void PlayScene::BeginScene()
{
	bgm_->Play();
	isEnd_ = false;
}

void PlayScene::Update()
{


	if (敵の体力が０になったら)
	{
		nextScene_ = EScene::ClearScene;
		isEnd_ = true;
	}
	if (自機の体力が０なったら)
	{
		nextScene_ = EScene::OverScene;

		isEnd_ = true;
	}
}

void PlayScene::EndScene()
{
	bgm_->Stop();
}
EScene PlayScene::GetNextScene()
{
	return nextScene_;
}
