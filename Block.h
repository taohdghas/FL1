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

	//初期化
	void Initialize(const Vector3& position, float size = 1.0f, bool isGoal = false,BlockType type = BlockType::Normal);
    //共有モデル解放
	static void FinalizeModels();
	//更新
	void Update();
	//描画
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

	// 終点ブロックか
	bool IsGoal() const { return isGoal_; }

private:
	//種類ごとの共有モデル
	static Model* normalModel_; // 通常ブロック
	static Model* burnableModel_; // 燃えるブロック
	WorldTransform worldTransform_{};
	BlockType type_ = BlockType::Normal;
	Vector3 position_{};
	Vector3 halfSize_{0.5f, 0.5f, 0.5f};
	bool isInitialized_ = false;
	bool isActive_ = true;//ブロックが存在しているか
	bool isGoal_ = false;
};