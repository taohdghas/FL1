#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

class Switch {
public:
	Switch() = default;
	~Switch();

	void Initialize(const Vector3& position,int groupId, float size = 2.0f);
	void Update();
	void Draw(const Camera& camera);
	// スイッチをONにする
	void Activate();
	// ON状態か取得
	bool IsActive() { return isActive_; }
	// グループIDを取得
	int GetGroupId() { return groupId_; }
    // 位置と当たり判定サイズを取得
	const Vector3& GetPosition(){ return position_; }
	const Vector3& GetHalfSize() { return halfSize_; }

	// モデル解放
	static void FinalizeModel();

private:
	static Model* model_;
	WorldTransform worldTransform_{};
	ObjectColor objectColor_{};
	Vector3 position_{};
	Vector3 halfSize_{0.5f, 0.5f, 0.5f};
	int groupId_ = 1;
	bool isActive_ = false;
	bool isInitialized_ = false;
};