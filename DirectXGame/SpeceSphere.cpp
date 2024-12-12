#include "SpeceSphere.h"
#include <cassert>

// インストラクタ
SpeceSphere::SpeceSphere() {}

// デストラクタ
SpeceSphere::~SpeceSphere() {}

void SpeceSphere::Initialize(Model* model, ViewProjection* viewProjection) {
	// NULLポインタチェック
	assert(model);

	// 引数
	modelSykDome_ = model;

	viewProjection_ = viewProjection;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
}

void SpeceSphere::Update() {}

void SpeceSphere::Draw() {

	// 3Dモデルの描画
	modelSykDome_->Draw(worldTransform_, *viewProjection_);
}