#pragma once
#include <KamataEngine.h>
#include <memory>
#include <string>
#include <vector>
#include "Block.h"

using namespace KamataEngine;

class MapChip {
public:
	MapChip() = default;
	~MapChip();

	// CSVを読み込んでマップを作成する
	// csvPath : CSVファイルのパス
	// blockSize : ブロック同士の間隔
	// 成功したらtrue、失敗したらfalse
	bool Initialize(const std::string& csvPath, float blockSize = 1.0f);

	void Update();
	void Draw(const Camera& camera);

	// マップのブロックをすべて解放する
	void Finalize();

	// 生成されたブロック数を取得する
	size_t GetBlockCount() const;

private:
	std::vector<std::unique_ptr<Block>> blocks_;
	float blockSize_ = 1.0f;
};