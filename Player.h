#pragma once

#include <KamataEngine.h>

using namespace KamataEngine;

// Cubeを仮モデルにしたプレイヤー
class Player {
public:
	Player() = default;
	~Player();

	void Initialize(const Vector3& position);
	void Update();
	void Draw(const Camera& camera);

	const Vector3& GetPosition() const;
	const Vector3& GetPreviousPosition() const;
	const Vector3& GetHalfSize() const;
	float GetVerticalVelocity() const;
	bool IsGrounded() const;

	// MapChipの衝突解決から呼び出す
	void SetPosition(const Vector3& position);
	void SetVerticalVelocity(float velocity);
	void SetGrounded(bool grounded);

private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_{};
	Vector3 position_{};
	Vector3 previousPosition_{};

	float verticalVelocity_ = 0.0f;
	float moveSpeed_ = 0.12f;
	float jumpPower_ = 0.28f;
	float gravity_ = 0.012f;
	Vector3 halfSize_{0.4f, 0.4f, 0.4f};
	bool isGrounded_ = false;
	bool isInitialized_ = false;
};
