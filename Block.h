#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

class Block {
public:
	Block() = default;
	~Block();

	// size: 衝突判定（AABB）の1辺の大きさ。
	// isGoal: 終点ブロックかどうか
	void Initialize(const Vector3& position, float size = 1.0f, bool isGoal = false);
	void Update();
	void Draw(const Camera& camera);

	void SetPosition(const Vector3& position);
	const Vector3& GetPosition() const;

	// AABB衝突判定用：ブロックの半サイズを返す
	const Vector3& GetHalfSize() const;

	// 終点ブロックか
	bool IsGoal() const { return isGoal_; }

private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_{};
	Vector3 position_{};
	Vector3 halfSize_{0.5f, 0.5f, 0.5f};
	bool isInitialized_ = false;
	bool isGoal_ = false;
};