#pragma once
#include <KamataEngine.h>
#include "MyMath.h"

using namespace KamataEngine;

class Door {
public:
	Door() = default;
	~Door();

	void Initialize(const Vector3& position, int groupId, float size = 2.0f);
	void Update();
	void Draw(const Camera& camera);
	void Open();

	bool IsOpen() const { return isOpen_; }
	int GetGroupId() const { return groupId_; }

	const Vector3& GetPosition() const { return position_; }
	const Vector3& GetHalfSize() const { return halfSize_; }

	static void FinalizeModel();

private:
	static Model* model_;

	WorldTransform worldTransform_{};
	ObjectColor objectColor_{};

	Vector3 position_{};
	Vector3 halfSize_{0.5f, 0.5f, 0.5f};

	int groupId_ = 1;

	bool isOpen_ = false;
	bool isInitialized_ = false;
};