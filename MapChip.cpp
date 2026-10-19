
#include "MapChip.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// デストラクタ
MapChip::~MapChip() { Finalize(); }

// 初期化
bool MapChip::Initialize(const std::string& csvPath, float blockSize) {
	blockSize_ = blockSize;
	return LoadCSV(csvPath);
}

// 更新
void MapChip::Update() {
	
}

// 描画
void MapChip::Draw(const Camera& camera) {
	for (auto& row : blocks_) {
		for (auto& block : row) {
			if (block) {
				block->Draw(camera);
			}
		}
	}
}

// 終了処理
void MapChip::Finalize() {
	blocks_.clear();
	mapData_.clear();
}

// CSV読み込み
bool MapChip::LoadCSV(const std::string& csvPath) {
	std::ifstream file(csvPath);

	if (!file.is_open()) {
		return false;
	}

	// 読み込みに失敗しても現在のマップを維持できるよう、
	// 一時的な配列に読み込む
	std::vector<std::vector<int>> newMapData;
	std::string line;

	while (std::getline(file, line)) {
		// 空行は無視
		if (line.empty()) {
			continue;
		}

		std::vector<int> rowData;
		std::stringstream ss(line);
		std::string cell;

		while (std::getline(ss, cell, ',')) {
			// CSVの値を整数に変換
			std::stringstream cellStream(cell);
			int value = -1;
			char extra = '\0';

			if (!(cellStream >> value) || (cellStream >> extra)) {
				return false;
			}

			// 0: 空きマス、1: ブロック
			if (value != 0 && value != 1) {
				return false;
			}

			rowData.push_back(value);
		}

		if (rowData.empty()) {
			return false;
		}

		// 全行の列数が一致しているか確認
		if (!newMapData.empty() && rowData.size() != newMapData[0].size()) {
			return false;
		}

		newMapData.push_back(rowData);
	}

	file.close();

	if (newMapData.empty()) {
		return false;
	}

	// CSV読み込み成功。マップデータを更新
	mapData_ = std::move(newMapData);

	// CSV読み込み時はマップ全体を再構築
	blocks_.clear();
	blocks_.resize(mapData_.size());

	for (size_t row = 0; row < mapData_.size(); ++row) {
		blocks_[row].resize(mapData_[row].size());

		for (size_t column = 0; column < mapData_[row].size(); ++column) {
			UpdateBlock(static_cast<int>(row), static_cast<int>(column));
		}
	}

	return true;
}

// CSV保存
bool MapChip::SaveCSV(const std::string& csvPath) const {
	std::ofstream file(csvPath);

	if (!file.is_open()) {
		return false;
	}

	for (size_t row = 0; row < mapData_.size(); ++row) {
		for (size_t column = 0; column < mapData_[row].size(); ++column) {
			file << mapData_[row][column];

			if (column + 1 < mapData_[row].size()) {
				file << ',';
			}
		}

		file << '\n';
	}

	return file.good();
}

// 指定したマスのブロックだけを更新
void MapChip::UpdateBlock(int row, int column) {
	// ブロックを置かない場合
	if (mapData_[row][column] == 0) {
		blocks_[row][column].reset();
		return;
	}

	// すでにブロックがある場合は何もしない
	if (blocks_[row][column]) {
		return;
	}

	// マスの位置を計算
	Vector3 position{static_cast<float>(column) * blockSize_, static_cast<float>(static_cast<int>(mapData_.size()) - 1 - row) * blockSize_, 0.0f};

	// 変更されたマスのブロックだけ生成
	blocks_[row][column] = std::make_unique<Block>();
	blocks_[row][column]->Initialize(position);
}

// マスの値を変更
bool MapChip::SetCell(int row, int column, int value) {
	// 範囲チェック
	if (row < 0 || row >= static_cast<int>(mapData_.size())) {
		return false;
	}

	if (column < 0 || column >= static_cast<int>(mapData_[row].size())) {
		return false;
	}

	// 0か1以外は受け付けない
	if (value != 0 && value != 1) {
		return false;
	}

	// 値が変わらなければ何もしない
	if (mapData_[row][column] == value) {
		return true;
	}

	// マップデータを変更
	mapData_[row][column] = value;

	// 変更されたマスだけ更新
	UpdateBlock(row, column);

	return true;
}

// マスの値を取得
int MapChip::GetCell(int row, int column) const {
	if (row < 0 || row >= static_cast<int>(mapData_.size())) {
		return 0;
	}

	if (column < 0 || column >= static_cast<int>(mapData_[row].size())) {
		return 0;
	}

	return mapData_[row][column];
}

// 行数を取得
int MapChip::GetRowCount() const { return static_cast<int>(mapData_.size()); }

// 列数を取得
int MapChip::GetColumnCount() const {
	if (mapData_.empty()) {
		return 0;
	}

	return static_cast<int>(mapData_[0].size());
}
