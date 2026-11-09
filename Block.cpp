#include "Block.h"
#include "MyMath.h"

using namespace KamataEngine;

Block::~Block() {
	delete model_;
	model_ = nullptr;
}

void Block::Initialize(const Vector3& position, float size, bool isGoal) {
	position_ = position;
	isGoal_ = isGoal;

	// 終点は専用モデルがあれば差し替える。無ければ cube を使う。
	if (isGoal_) {
		model_ = Model::CreateFromOBJ("cube");
		if (model_ == nullptr) {
			model_ = Model::CreateFromOBJ("cube");
		}
	} else {
		model_ = Model::CreateFromOBJ("cube");
	}

	worldTransform_.Initialize();

	// 見た目は元のサイズのまま。終点は少し大きくして視認性を上げる
	float visualScale = isGoal_ ? 1.05f : 1.0f;
	worldTransform_.scale_ = {visualScale, visualScale, visualScale};
	worldTransform_.translation_ = position_;
	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	// 衝突判定のAABBはグリッド1マス分（size）に合わせる。
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