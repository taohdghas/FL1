#include "Player.h"
#include "MyMath.h"
#include <KamataEngine.h>

using namespace KamataEngine;

Player::~Player() {
	delete model_;
	model_ = nullptr;
}

void Player::Initialize(const Vector3& position) {
	position_ = position;
	previousPosition_ = position;
	verticalVelocity_ = 0.0f;
	isGrounded_ = false;

	model_ = Model::CreateFromOBJ("cube");

	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	halfSize_ = {1.0f * worldTransform_.scale_.x, 1.0f * worldTransform_.scale_.y, 1.0f * worldTransform_.scale_.z};
	worldTransform_.translation_ = position_;
	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	isInitialized_ = true;
}

void Player::Update() {
	if (!isInitialized_) {
		return;
	}

	// 衝突判定で前フレームの位置と比較するため、移動前の座標を保存
	previousPosition_ = position_;

	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_A) || input->PushKey(DIK_LEFT)) {
		position_.x -= moveSpeed_;
	}
	if (input->PushKey(DIK_D) || input->PushKey(DIK_RIGHT)) {
		position_.x += moveSpeed_;
	}

	// 接地しているときだけジャンプできる
	if (isGrounded_ && input->TriggerKey(DIK_SPACE)) {
		verticalVelocity_ = jumpPower_;
		isGrounded_ = false;
	}

	// 毎フレーム重力を加える。着地判定はMapChip側で行う
	isGrounded_ = false;
	verticalVelocity_ -= gravity_;
	position_.y += verticalVelocity_;

	worldTransform_.translation_ = position_;
	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}

void Player::Draw(const Camera& camera) {
	if (!isInitialized_ || model_ == nullptr) {
		return;
	}
	model_->Draw(worldTransform_, camera);
}

const Vector3& Player::GetPosition() const { return position_; }
const Vector3& Player::GetPreviousPosition() const { return previousPosition_; }
const Vector3& Player::GetHalfSize() const { return halfSize_; }
float Player::GetVerticalVelocity() const { return verticalVelocity_; }
bool Player::IsGrounded() const { return isGrounded_; }

void Player::SetPosition(const Vector3& position) {
	position_ = position;
	// 衝突補正後も同じフレームで描画位置に反映する
	worldTransform_.translation_ = position_;
	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
}
void Player::SetVerticalVelocity(float velocity) { verticalVelocity_ = velocity; }
void Player::SetGrounded(bool grounded) { isGrounded_ = grounded; }
