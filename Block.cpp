#include "Block.h"
#include "MyMath.h"

using namespace KamataEngine;

// 共有モデル実体
Model* Block::normalModel_ = nullptr;
Model* Block::burnableModel_ = nullptr;

Block::~Block() {}

void Block::Initialize(const Vector3& position, float size, bool isGoal, BlockType type) {
	position_ = position;
	type_ = type;
	isActive_ = true;
  isGoal_ = isGoal;
	 // モデルが読み込まれていなければ読み込む
    // 通常
    if (normalModel_ == nullptr) {
        normalModel_ = Model::CreateFromOBJ("cube");
    }

    // 燃える
    if (burnableModel_ == nullptr) {
        burnableModel_ = Model::CreateFromOBJ("burn");
    }

    // ゴール
    if (isGoal_) {
        goalModel_ = Model::CreateFromOBJ("cube");
    }

	worldTransform_.Initialize();
	// 見た目は元のサイズのまま
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.translation_ = position_;
	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	// 衝突判定のAABBはグリッド1マス分に合わせる
	halfSize_ = {0.5f * size, 0.5f * size, 0.5f * size};

	isInitialized_ = true;
}
//共有モデル解放
void Block::FinalizeModels() { 
	delete normalModel_;
	delete burnableModel_;
	normalModel_ = nullptr;
	burnableModel_ = nullptr;
}

// 更新
void Block::Update() {
	if (!isInitialized_ || !isActive_) {
		return;
	}

	worldTransform_.translation_ = position_;
	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}
// 描画
void Block::Draw(const Camera& camera) {
	if (!isInitialized_ || !isActive_ ) {
		return;
	}
	//ブロックの種類に応じてモデルを選択
	Model* drawModel = nullptr;
	
	switch (type_) {
	case BlockType::Normal:
		drawModel = normalModel_;
		break;
	case BlockType::Burnable:
		drawModel = burnableModel_;
		break;
	}
	if (drawModel != nullptr) {
		drawModel->Draw(worldTransform_, camera);
	}
}
// 燃える
void Block::Burn() {
	// 燃えるブロック以外は燃えない
	if (!IsBurnable()) {
		return;
	}
	isActive_ = false;
}

void Block::SetPosition(const Vector3& position) { position_ = position; }
const Vector3& Block::GetPosition() const { return position_; }
const Vector3& Block::GetHalfSize() const { return halfSize_; }