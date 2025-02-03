#pragma once
#include <cstdint>
#include <Input.h>
#include <Audio.h>


class BGM
{
public:
	~BGM();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::string& filePath);


	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void Play();

	void Stop();


private:


	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	// 音
	uint32_t music;

	// 音声再生
	uint32_t playMusic;
};

