#pragma once

#include "EnemyBullet.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include <list>
#include"Sprite.h"
#include"Material.h"
#include <DirectXMath.h>

// 自機クラスの前方宣言
class Player;

class Enemy {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	Enemy();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	enum class Phase {

		Approach, // 接近する
		Leave,    // 離脱する
	};

	void Fire();

	// 発射間隔
	static const int kFireInterval = 10;

	// 接近フェーズ初期化
	void ApproacPphase();

	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetEnemyBullets() const { return bullets_; }

	// 衝突を検知したら呼び出されるコールバック関数
	void OnCollision();

	// 半径
	const float EnemyRad = 1;

	//敵の攻撃開始範囲
	//const float EnemyAttackRad;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	// フェーズ
	Phase phase_ = Phase::Approach;

	// キーボード入力
	Input* input_ = nullptr;

	// 弾のクールタイム
	int shotCollTime_;
	int shotCollTimeMax = 60;

	// 発射タイマー
	int32_t FireTimer = 0;

	// 弾
	EnemyBullet* bullet_ = nullptr;

	// 弾
	std::list<EnemyBullet*> bullets_;

	// 自キャラ
	Player* player_ = nullptr;

	

};
