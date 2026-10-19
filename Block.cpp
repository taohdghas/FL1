#include "Block.h"
#include "MyMath.h"

using namespace KamataEngine;

Block::~Block() {
	// モデルの解放
	delete model_;
	model_ = nullptr;
}

void Block::Initialize(const Vector3& position) {
	// 座標を設定
	position_ = position;

	// OBJファイルからモデルを生成
	model_ = Model::CreateFromOBJ("cube");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// 座標を設定
	worldTransform_.translation_ = position_;

	// ワールド行列を計算
	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 行列を転送
	worldTransform_.TransferMatrix();

	isInitialized_ = true;
}

void Block::Update() {
	if (!isInitialized_) {
		return;
	}

	// 座標を反映
	worldTransform_.translation_ = position_;

	// ワールド行列を計算
	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 行列を転送
	worldTransform_.TransferMatrix();
}

void Block::Draw(const Camera& camera) {
	if (!isInitialized_ || model_ == nullptr) {
		return;
	}

	// モデル描画
	model_->Draw(worldTransform_, camera);
}

void Block::SetPosition(const Vector3& position) { position_ = position; }

const Vector3& Block::GetPosition() const { return position_; }
