#pragma once
class Time
{
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Time();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Time();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	float  RestTime();
	bool IsFinish();
private: // メンバ変数

	float restTime = 30.0f;
	float elapsedTime = 0.0f;

};

