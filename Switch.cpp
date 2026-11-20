#include "Switch.h"
#include "MyMath.h"

// モデル
Model* Switch::model_ = nullptr;

// デストラクタ
Switch::~Switch() {}
// 初期化
void Switch::Initialize(const Vector3& position, int groupId, float size) {
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
	objectColor_.SetColor({0.5f, 0.5f, 0.5f, 1.0f});

	isActive_ = false;
	isInitialized_ = true;
}
//更新
void Switch::Update() {
	
}
//描画
void Switch::Draw(const Camera& camera) {
	if (!isInitialized_) {
		return;
	}

	if (model_ != nullptr) {
		model_->Draw(worldTransform_, camera, &objectColor_);
	}
}
// スイッチをONにする
void Switch::Activate() {
	if (isActive_) {
		return;
	}

	isActive_ = true;

	// ONになったら黄色に変更
	objectColor_.SetColor({1.0f, 1.0f, 0.0f, 1.0f});
}
// モデル解放
void Switch::FinalizeModel() {
	delete model_;
	model_ = nullptr;
}