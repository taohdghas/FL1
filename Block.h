#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

class Block {
public:
	Block() = default;
	~Block();

	// 初期化
	void Initialize(const Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw(const Camera& camera);

	// 座標設定
	void SetPosition(const Vector3& position);

	// 座標取得
	const Vector3& GetPosition() const;

private:
	// 3Dモデル
	Model* model_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform worldTransform_{};

	// ブロックの座標
	Vector3 position_{};

	// 初期化済みフラグ
	bool isInitialized_ = false;
};
