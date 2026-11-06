#pragma once
#include "ElementType.h"
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
	//現在のエレメント取得
	ElementType GetElement() const;

	// MapChipの衝突解決から呼び出す
	void SetPosition(const Vector3& position);
	void SetVerticalVelocity(float velocity);
	void SetGrounded(bool grounded);
	//エレメントを設定
	void SetElement(ElementType element);
private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_{};
	//現在のエレメント
	ElementType currentElement_ = ElementType::None;
	Vector3 position_{};
	Vector3 previousPosition_{};
	Vector3 halfSize_{0.5f, 0.5f, 0.5f};
	float verticalVelocity_ = 0.0f;
	float moveSpeed_ = 0.12f;
	float jumpPower_ = 0.28f;
	float gravity_ = 0.012f;
	bool isGrounded_ = false;
	bool isInitialized_ = false;
};
