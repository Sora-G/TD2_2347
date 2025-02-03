#pragma once
#include "EScene.h"

class BGM;
class Input;
class PlayScene
{
public:
	PlayScene();
	~PlayScene();

	void Initialize();
	void BeginScene();
	void Update();
	void EndScene();
	bool IsEnd() { return isEnd_; }
	EScene GetNextScene();
private:
	Input* input_;
	BGM* bgm_;
	bool isEnd_;
	EScene nextScene_;
};

