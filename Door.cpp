
#include "Door.h"

// モデル
Model* Door::model_ = nullptr;

// デストラクタ
Door::~Door() {}

// 初期化
void Door::Initialize(const Vector3& position, int groupId, float size) {
	position_ = position;
	groupId_ = groupId;

	halfSize_ = {size * 0.5f, size * 0.5f, size * 0.5f};

	if (model_ == nullptr) {
		model_ = Model::CreateFromOBJ("cube");
	}

	worldTransform_.Initialize();

	// Blockと同じモデルサイズにする
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	worldTransform_.translation_ = position_;

	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	objectColor_.Initialize();
	objectColor_.SetColor({0.3f, 0.3f, 1.0f, 1.0f});

	isOpen_ = false;
	isInitialized_ = true;
}

// 更新
void Door::Update() {
	
}
// 描画
void Door::Draw(const Camera& camera) {
	if (!isInitialized_ || isOpen_) {
		return;
	}

	if (model_ != nullptr) {
		model_->Draw(worldTransform_, camera, &objectColor_);
	}
}
// 扉を開く
void Door::Open() { isOpen_ = true; }
// モデル解放
void Door::FinalizeModel() {
	delete model_;
	model_ = nullptr;
}
