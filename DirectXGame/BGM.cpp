#include "BGM.h"

BGM::~BGM()
{
}

void BGM::Initialize(const std::string& filePath)
{
	input_ = Input::GetInstance();

	audio_ = Audio::GetInstance();

	// 音
	music = audio_->LoadWave(filePath);




}

void BGM::Update()
{
}

void BGM::Draw()
{
}

void BGM::Play()
{
	audio_->StopWave(playMusic);
	// 音声再生
	playMusic = audio_->PlayWave(music, true);
}

void BGM::Stop()
{
	audio_->StopWave(playMusic);
}
