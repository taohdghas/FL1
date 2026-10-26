#pragma once

#include <KamataEngine.h>

using namespace KamataEngine;

// プレイヤーキャラクター（仮モデルはCube）
class Player {
public:
	Player() = default;
	~Player();

	void Initialize(const Vector3& position);
	void Update();
	void Draw(const Camera& camera);

	const Vector3& GetPosition() const;

private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_{};
	Vector3 position_{};

	float verticalVelocity_ = 0.0f;
	float moveSpeed_ = 0.12f;
	float jumpPower_ = 0.28f;
	float gravity_ = 0.012f;
	float groundY_ = 0.5f;
	bool isGrounded_ = true;
	bool isInitialized_ = false;
};
