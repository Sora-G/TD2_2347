#pragma once
class BGM;
class Input;
class ClearScene
{
public:
	ClearScene();
	~ClearScene();

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

