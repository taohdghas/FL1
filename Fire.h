#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

class Fire {
public:
	Fire() = default;
	~Fire();
	//初期化
	void Initialize(const Vector3& position, float size = 2.0f);
	//描画
	void Draw(const Camera& camera);
	// 火を消す
	void Extinguish();
	// モデルの終了処理
	static void FinalizeModel();
	// 消えているか
	bool IsExtinguished() const { return isExtinguished_; }

	// 位置と当たり判定の取得
	const Vector3& GetPosition() const { return position_; }
	const Vector3& GetHalfSize() const { return halfSize_; }

private:
	static Model* model_;
	WorldTransform worldTransform_{};
	ObjectColor objectColor_{};

	Vector3 position_{};
	Vector3 halfSize_{0.5f, 0.5f, 0.5f};

	bool isExtinguished_ = false;
	bool isInitialized_ = false;
};