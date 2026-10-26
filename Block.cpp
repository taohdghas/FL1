#include "Block.h"
#include "MyMath.h"

using namespace KamataEngine;

Block::~Block() {
	delete model_;
	model_ = nullptr;
}

void Block::Initialize(const Vector3& position, float size) {
	position_ = position;
	model_ = Model::CreateFromOBJ("cube");

	worldTransform_.Initialize();

	// 見た目は元のサイズのまま（cube.obj を1×1×1として扱う）
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.translation_ = position_;
	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	// 衝突判定のAABBはグリッド1マス分（size）に合わせる。
	// これにより隣接ブロック間に衝突の隙間ができない。
	halfSize_ = {0.5f * size, 0.5f * size, 0.5f * size};

	isInitialized_ = true;
}

void Block::Update() {
	if (!isInitialized_) {
		return;
	}

	worldTransform_.translation_ = position_;
	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Block::Draw(const Camera& camera) {
	if (!isInitialized_ || model_ == nullptr) {
		return;
	}
	model_->Draw(worldTransform_, camera);
}

void Block::SetPosition(const Vector3& position) { position_ = position; }
const Vector3& Block::GetPosition() const { return position_; }
const Vector3& Block::GetHalfSize() const { return halfSize_; }