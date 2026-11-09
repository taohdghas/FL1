#include "Block.h"
#include "MyMath.h"

using namespace KamataEngine;

Block::~Block() {
	delete model_;
	model_ = nullptr;
}

void Block::Initialize(const Vector3& position, float size, BlockType type) {
	position_ = position;
	type_ = type;
	isActive_ = true;
	model_ = Model::CreateFromOBJ("cube");

	worldTransform_.Initialize();

	// 見た目は元のサイズのまま
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.translation_ = position_;
	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	// 衝突判定のAABBはグリッド1マス分（size）に合わせる。
	halfSize_ = {0.5f * size, 0.5f * size, 0.5f * size};

	isInitialized_ = true;
}
//更新
void Block::Update() {
	if (!isInitialized_ || !isActive_) {
		return;
	}

	worldTransform_.translation_ = position_;
	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}
//描画
void Block::Draw(const Camera& camera) {
	if (!isInitialized_ || !isActive_ || model_ == nullptr) {
		return;
	}
	model_->Draw(worldTransform_, camera);
}
//燃える
void Block::Burn() { 
	//燃えるブロック以外は燃えない
	if (!IsBurnable()) { return; }
	isActive_ = false;
}

void Block::SetPosition(const Vector3& position) { position_ = position; }
const Vector3& Block::GetPosition() const { return position_; }
const Vector3& Block::GetHalfSize() const { return halfSize_; }