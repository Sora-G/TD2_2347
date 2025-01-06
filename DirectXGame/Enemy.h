#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Vector3.h"
#include "EnemyBullet.h"
#include <list>

//自キャラの前方宣言
class Player;

class Enemy {
public:

	enum class Phase {
		Approach, // 接近
		Leave,    // 離脱
	};

	//デストラクタ
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, const Vector3& position, Vector3 approachVelocity, Vector3 leaveVelocity);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 発射弾
	/// </summary>
	void Fire();

	/// <summary>
	/// 自キャラのsetter
	/// </summary>
	/// <param name="player">自キャラ</param>
	void SetPlayer(Player* player) { player_ = player; } 

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び戻されるコールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	// 半径
	const float rad = 1.0f;

private:

	//ワールド変換
	WorldTransform worldTransform_;
	//モデル
	Model* model_;
	//テクスチャハンドル
	uint32_t textureHandle_;
	//接近速度
	Vector3 approachVelocity_;
	//離脱速度
	Vector3 leaveVelocity_;
	//フェーズ
	Phase phase_ = Phase::Approach;
	//弾
	std::list<EnemyBullet*> bullets_;
	//発射クールタイム
	int shotCoolTime_;
	int shotCoolTimeMax_;
	//自キャラ
	Player* player_ = nullptr;

	Vector3 playerWorldPos;
	Vector3 enemyWorldPos;
	Vector3 e2pVector;
	Vector3 e2pNormal;
};
