#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

#include "Block.h"
#include "Player.h"
#include <memory>
#include <string>
#include <vector>

class MapChip {
public:
	MapChip() = default;
	~MapChip();

	bool Initialize(const std::string& csvPath, float blockSize = 2.0f);
	void Update();

	// Playerと各BlockのAABB衝突を解決する
	void ResolvePlayerCollision(Player& player) const;
	void Draw(const Camera& camera);
	void Finalize();

	bool SaveCSV(const std::string& csvPath) const;
	bool LoadCSV(const std::string& csvPath);

	bool SetCell(int row, int column, int value);
	int GetCell(int row, int column) const;

	int GetRowCount() const;
	int GetColumnCount() const;

private:
	// 指定したマスのブロックだけを更新する
	void UpdateBlock(int row, int column);

	std::vector<std::vector<int>> mapData_;

	// 各マスに対応するブロック
	// nullptrならブロックなし
	std::vector<std::vector<std::unique_ptr<Block>>> blocks_;

	float blockSize_ = 2.0f;
};