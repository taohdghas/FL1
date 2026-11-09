#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

//ブロック種類
enum class BlockType {
	Normal,//通常
	Burnable,//燃える
};

class Block {
public:
	Block() = default;
	~Block();

	// size: 衝突判定（AABB）の1辺の大きさ。
	void Initialize(const Vector3& position, float size = 1.0f,BlockType type = BlockType::Normal);
	void Update();
	void Draw(const Camera& camera);
	//燃やす
	void Burn();
	//燃えるブロックか判定
	bool IsBurnable()const { return type_ == BlockType::Burnable; }
	//ブロックが有効か判定
	bool IsActive() const{ return isActive_; }

	void SetPosition(const Vector3& position);
	const Vector3& GetPosition() const;
	//ブロックの半サイズを返す
	const Vector3& GetHalfSize() const;
	//ブロック種類を取得
	BlockType GetType() const { return type_; }

private:
	Model* model_ = nullptr;
	WorldTransform worldTransform_{};
	BlockType type_ = BlockType::Normal;
	Vector3 position_{};
	Vector3 halfSize_{0.5f, 0.5f, 0.5f};
	bool isInitialized_ = false;
	bool isActive_ = true;//ブロックが存在しているか
};