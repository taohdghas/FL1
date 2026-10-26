#include "Player.h"
#include "MyMath.h"
#include "KamataEngine.h"
#include <Windows.h>

using namespace KamataEngine;

Player::~Player() {
	// Model::CreateFromOBJで生成したモデルを解放
	delete model_;
	model_ = nullptr;
}

void Player::Initialize(const Vector3& position) {
	position_ = position;
	groundY_ = position.y;
	verticalVelocity_ = 0.0f;
	isGrounded_ = true;

	// 仮のプレイヤーモデルとしてCubeを使用
	model_ = Model::CreateFromOBJ("cube");

	worldTransform_.Initialize();
	worldTransform_.scale_ = {0.8f, 0.8f, 0.8f};
	worldTransform_.translation_ = position_;
	worldTransform_.matWorld_ = MyMath::MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	isInitialized_ = true;
}

void Player::Update() {
	if (!isInitialized_) {
		return;
	}

	Input* input = Input::GetInstance();

	// 左右移動：A/D または左右方向キー
	if (input->PushKey(DIK_A) || input->PushKey(DIK_LEFT)) {
		position_.x -= moveSpeed_;
	}
	if (input->PushKey(DIK_D) || input->PushKey(DIK_RIGHT)) {
		position_.x += moveSpeed_;
	}

	// 接地中にSpaceキーを押したときだけジャンプ
	if (isGrounded_ && input->TriggerKey(DIK_SPACE)) {
		verticalVelocity_ = jumpPower_;
		isGrounded_ = false;
	}

	// 重力と垂直方向の移動
	if (!isGrounded_) {
		verticalVelocity_ -= gravity_;
		position_.y += verticalVelocity_;

		// 現段階では固定された平面を地面として扱う
		if (position_.y <= groundY_) {
			position_.y = groundY_;
			verticalVelocity_ = 0.0f;
			isGrounded_ = true;
		}
	}

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
