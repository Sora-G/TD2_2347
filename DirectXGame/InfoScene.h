#pragma once
#include "EScene.h"

class BGM;
class Input;
class InfoScene
{
public:
	InfoScene();
	~InfoScene();

	void Initialize();
	void BeginScene();
	void Update();
	void EndScene();
	bool IsEnd() { return isEnd_; }


private:
	Input* input_;
	BGM* bgm_;
	bool isEnd_;

};

