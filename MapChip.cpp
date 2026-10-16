#include "MapChip.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace KamataEngine;

MapChip::~MapChip() { Finalize(); }

bool MapChip::Initialize(const std::string& csvPath, float blockSize) {
	// 以前のマップがあれば解放する
	Finalize();

	blockSize_ = blockSize;

	// CSVファイルを開く
	std::ifstream file(csvPath);

	if (!file.is_open()) {
		OutputDebugStringA(("MapChip: CSVを開けませんでした: " + csvPath + "\n").c_str());
		return false;
	}

	// CSVの内容を一時的に保存する
	std::vector<std::vector<int>> mapData;

	std::string line;

	while (std::getline(file, line)) {
		// 空行は無視する
		if (line.empty()) {
			continue;
		}

		// Windows形式の改行で残る'\r'を取り除く
		if (!line.empty() && line.back() == '\r') {
			line.pop_back();
		}

		std::vector<int> row;
		std::stringstream stream(line);
		std::string cell;

		while (std::getline(stream, cell, ',')) {
			try {
				size_t parsedLength = 0;
				int value = std::stoi(cell, &parsedLength);

				// 数字以外が混ざっていたらエラー
				if (parsedLength != cell.size()) {
					return false;
				}

				// 今回は0と1だけを使用する
				if (value != 0 && value != 1) {
					return false;
				}

				row.push_back(value);
			} catch (...) {
				return false;
			}
		}

		if (!row.empty()) {
			mapData.push_back(row);
		}
	}

	file.close();

	if (mapData.empty()) {
		return false;
	}

	// CSVの行・列に合わせてブロックを生成する
	const size_t rowCount = mapData.size();

	for (size_t row = 0; row < rowCount; ++row) {
		for (size_t column = 0; column < mapData[row].size(); ++column) {
			// 0は空白なので何も生成しない
			if (mapData[row][column] == 0) {
				continue;
			}

			auto block = std::make_unique<Block>();

			// CSVの上の行を、ゲーム画面でも上側に配置する
			Vector3 position = {static_cast<float>(column) * blockSize_, static_cast<float>(rowCount - 1 - row) * blockSize_, 0.0f};

			block->Initialize(position);

			blocks_.push_back(std::move(block));
		}
	}

	return true;
}

void MapChip::Update() {
	for (const auto& block : blocks_) {
		block->Update();
	}
}

void MapChip::Draw(const Camera& camera) {
	for (const auto& block : blocks_) {
		block->Draw(camera);
	}
}

void MapChip::Finalize() { blocks_.clear(); }

size_t MapChip::GetBlockCount() const { return blocks_.size(); }