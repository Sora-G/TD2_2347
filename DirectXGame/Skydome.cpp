#include "Skydome.h"
#include "cassert"

void Skydome::Initialize(Model* model) {
	// NULLポインタチェック
	assert(model);
	// 引数で受け取ったデータをメンバ変数に記録
	model_ = model;
	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Skydome::Update() {
	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix(false);
}

void Skydome::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
