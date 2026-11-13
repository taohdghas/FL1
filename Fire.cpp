#include "Fire.h"
#include "MyMath.h"

Model* Fire::model_ = nullptr;

Fire::~Fire() {}

void Fire::Initialize(const Vector3& position, float size) {

	position_ = position;

	// 当たり判定の大きさ
	halfSize_ = {size * 0.5f, size * 0.5f, size * 0.5f};

	isExtinguished_ = false;

	// 仮モデルとしてcubeを使用
	if (model_ == nullptr) {
		model_ = Model::CreateFromOBJ("cube");
	}

	// ワールド変換
	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.translation_ = position_;

	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	// 火の色を赤色にする
	objectColor_.Initialize();
	objectColor_.SetColor({1.0f, 0.0f, 0.0f, 1.0f});

	isInitialized_ = true;
}

void Fire::Draw(const Camera& camera) {

	// 初期化されていない、消えている場合は描画しない
	if (!isInitialized_ || isExtinguished_) {
		return;
	}

	if (model_ != nullptr) {
		model_->Draw(worldTransform_, camera, &objectColor_);
	}
}
// 火を消す
void Fire::Extinguish() { isExtinguished_ = true; }
// モデルの終了処理
void Fire::FinalizeModel() {

	delete model_;
	model_ = nullptr;
}