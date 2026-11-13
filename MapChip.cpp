#include "MapChip.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// デストラクタ
MapChip::~MapChip() { Finalize(); }
// 初期化
bool MapChip::Initialize(const std::string& csvPath, float blockSize) {
	blockSize_ = blockSize;
	return LoadCSV(csvPath);
}
// 更新
void MapChip::Update() {}
// 描画
void MapChip::Draw(const Camera& camera) {
	for (auto& row : blocks_) {
		for (auto& block : row) {
			if (block) {
				block->Draw(camera);
			}
		}
	}
	// 火のギミック描画
	for (auto& row : fires_) {
		for (auto& fire : row) {
			if (fire) {
				fire->Draw(camera);
			}
		}
	}
}
// 終了処理
void MapChip::Finalize() {
	blocks_.clear();
	mapData_.clear();
	fires_.clear();
	playerSpawnPosition_ = {};
	hasPlayerSpawnPosition_ = false;
	hasGoal_ = false;
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

	// プレイヤー開始位置が複数ないか確認
	int spawnCount = 0;

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

			// 0: 空きマス
			// 1: 通常ブロック
			// 2: ゴール
			// 3: プレイヤー開始位置
			// 4: 燃えるブロック
			// 5: 網目状ブロック
			// 6: 火のギミック
			if (value < 0 || value > 6) {
				return false;
			}

			// プレイヤー開始位置の数を確認
			if (value == 3) {
				spawnCount++;

				// 開始位置は1つだけ
				if (spawnCount > 1) {
					return false;
				}
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

	fires_.clear();
	fires_.resize(mapData_.size());

	hasGoal_ = false;

	for (size_t row = 0; row < mapData_.size(); ++row) {
		blocks_[row].resize(mapData_[row].size());
		fires_[row].resize(mapData_[row].size());

		for (size_t column = 0; column < mapData_[row].size(); ++column) {
			if (mapData_[row][column] == 2) {
				hasGoal_ = true;
			}

			UpdateBlock(static_cast<int>(row), static_cast<int>(column));
		}
	}

	// プレイヤー開始位置を更新
	UpdatePlayerSpawnPosition();

	return true;
}
// CSV保存
bool MapChip::SaveCSV(const std::string& csvPath) {
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
// マスの値を変更
bool MapChip::SetCell(int row, int column, int value) {

	// 範囲チェック
	if (row < 0 || row >= static_cast<int>(mapData_.size())) {
		return false;
	}

	if (column < 0 || column >= static_cast<int>(mapData_[row].size())) {
		return false;
	}

	// 値の範囲チェック
	if (value < 0 || value > 6) {
		return false;
	}

	// プレイヤー開始位置は1つだけにする
	if (value == 3) {
		for (size_t r = 0; r < mapData_.size(); ++r) {
			for (size_t c = 0; c < mapData_[r].size(); ++c) {
				if (mapData_[r][c] == 3) {
					mapData_[r][c] = 0;

					UpdateBlock(static_cast<int>(r), static_cast<int>(c));
				}
			}
		}
	}

	// 値が変わらなければ何もしない
	if (mapData_[row][column] == value) {
		UpdatePlayerSpawnPosition();
		return true;
	}

	// マップデータを変更
	mapData_[row][column] = value;

	// 終点フラグを再計算
	hasGoal_ = false;

	for (auto& rowData : mapData_) {
		for (int v : rowData) {
			if (v == 2) {
				hasGoal_ = true;
				break;
			}
		}

		if (hasGoal_) {
			break;
		}
	}

	// 変更されたマスを破棄して再生成
	blocks_[row][column].reset();
	UpdateBlock(row, column);

	// プレイヤー開始位置を更新
	UpdatePlayerSpawnPosition();

	return true;
}
// マスの値を取得
int MapChip::GetCell(int row, int column) {
	if (row < 0 || row >= static_cast<int>(mapData_.size())) {
		return 0;
	}

	if (column < 0 || column >= static_cast<int>(mapData_[row].size())) {
		return 0;
	}

	return mapData_[row][column];
}
// 列数を取得
int MapChip::GetColumnCount() {
	if (mapData_.empty()) {
		return 0;
	}

	return static_cast<int>(mapData_[0].size());
}
// Playerと各BlockのAABB衝突を解決する
void MapChip::ResolvePlayerCollision(Player& player) {

	// プレイヤーの現在位置と前フレーム位置を取得
	Vector3 pos = player.GetPosition();
	const Vector3 prevPos = player.GetPreviousPosition();
	const Vector3 playerHalf = player.GetHalfSize();
	const ElementType playerElement = player.GetElement();

	// 接地フラグ
	bool grounded = false;

	// 垂直速度の上書き用
	float newVerticalVelocity = player.GetVerticalVelocity();

	for (size_t row = 0; row < blocks_.size(); ++row) {
		for (size_t column = 0; column < blocks_[row].size(); ++column) {

			Block* block = blocks_[row][column].get();

			if (block == nullptr || !block->IsActive()) {
				continue;
			}

			// ゴールは当たり判定を持たない
			if (block->IsGoal()) {
				continue;
			}

			const Vector3 blockPos = block->GetPosition();
			const Vector3 blockHalf = block->GetHalfSize();

			// AABB同士の重なりチェック
			const bool overlapX = std::abs(pos.x - blockPos.x) < (playerHalf.x + blockHalf.x);

			const bool overlapY = std::abs(pos.y - blockPos.y) < (playerHalf.y + blockHalf.y);

			const bool overlapZ = std::abs(pos.z - blockPos.z) < (playerHalf.z + blockHalf.z);

			if (!(overlapX && overlapY && overlapZ)) {
				continue;
			}

			// 火エレメント接続中に燃えるブロックに触れた時
			if (playerElement == ElementType::Fire && block->IsBurnable()) {
				block->Burn();
				continue;
			}

			// 水エレメント接続中に網目ブロックを通過
			if (playerElement == ElementType::Water && block->GetType() == BlockType::Mesh) {
				continue;
			}

			// 前フレームでY方向に重なっていたか
			const bool prevOverlapY = std::abs(prevPos.y - blockPos.y) < (playerHalf.y + blockHalf.y);

			// Y方向の重なりが新たに発生した場合
			if (!prevOverlapY) {
				if (pos.y > blockPos.y) {
					// 上に乗っている
					pos.y = blockPos.y + blockHalf.y + playerHalf.y;

					if (newVerticalVelocity <= 0.0f) {
						newVerticalVelocity = 0.0f;
						grounded = true;
					}
				} else {
					// 下から頭をぶつけた
					pos.y = blockPos.y - blockHalf.y - playerHalf.y;

					if (newVerticalVelocity > 0.0f) {
						newVerticalVelocity = 0.0f;
					}
				}

				continue;
			}

			const float penX = (playerHalf.x + blockHalf.x) - std::abs(pos.x - blockPos.x);

			const float penZ = (playerHalf.z + blockHalf.z) - std::abs(pos.z - blockPos.z);

			if (penX < penZ) {
				// X軸で押し戻す
				if (pos.x > blockPos.x) {
					pos.x = blockPos.x + blockHalf.x + playerHalf.x;
				} else {
					pos.x = blockPos.x - blockHalf.x - playerHalf.x;
				}
			} else {
				// Z軸で押し戻す
				if (pos.z > blockPos.z) {
					pos.z = blockPos.z + blockHalf.z + playerHalf.z;
				} else {
					pos.z = blockPos.z - blockHalf.z - playerHalf.z;
				}
			}
		}
	}

	// 解決後の位置をプレイヤーへ反映
	player.SetPosition(pos);
	player.SetVerticalVelocity(newVerticalVelocity);
	player.SetGrounded(grounded);
}

// プレイヤー開始位置を取得
bool MapChip::GetPlayerSpawnPosition(Vector3& position) {
	if (!hasPlayerSpawnPosition_) {
		return false;
	}

	position = playerSpawnPosition_;
	return true;
}
// プレイヤーが終点に到達したか
bool MapChip::HasPlayerReachedGoal(const Player& player) const {

	const Vector3 pos = player.GetPosition();
	const Vector3 playerHalf = player.GetHalfSize();

	for (size_t row = 0; row < blocks_.size(); ++row) {
		for (size_t column = 0; column < blocks_[row].size(); ++column) {

			const Block* block = blocks_[row][column].get();

			if (block == nullptr || !block->IsGoal()) {
				continue;
			}

			const Vector3 blockPos = block->GetPosition();
			const Vector3 blockHalf = block->GetHalfSize();

			const bool overlapX = std::abs(pos.x - blockPos.x) < (playerHalf.x + blockHalf.x);

			const bool overlapY = std::abs(pos.y - blockPos.y) < (playerHalf.y + blockHalf.y);

			const bool overlapZ = std::abs(pos.z - blockPos.z) < (playerHalf.z + blockHalf.z);

			if (overlapX && overlapY && overlapZ) {
				return true;
			}
		}
	}

	return false;
}
// 火のギミックとの接触判定
bool MapChip::ResolveFireCollision(Player& player) {
	// プレイヤーの情報を取得
	const Vector3& playerPos = player.GetPosition();
	const Vector3& playerHalf = player.GetHalfSize();
	ElementType playerElement = player.GetElement();

	// マップ上の火をすべて調べる
	for (auto& row : fires_) {
		for (auto& fire : row) {

			// 火が存在しない、消えている場合は無視
			if (!fire || fire->IsExtinguished()) {
				continue;
			}

			const Vector3& firePos = fire->GetPosition();
			const Vector3& fireHalf = fire->GetHalfSize();

			// AABBによる接触判定
			bool overlapX = std::abs(playerPos.x - firePos.x) < (playerHalf.x + fireHalf.x);

			bool overlapY = std::abs(playerPos.y - firePos.y) < (playerHalf.y + fireHalf.y);

			bool overlapZ = std::abs(playerPos.z - firePos.z) < (playerHalf.z + fireHalf.z);

			// 接触していなければ次の火を調べる
			if (!overlapX || !overlapY || !overlapZ) {
				continue;
			}

			// 水なら火を消す
			if (playerElement == ElementType::Water) {
				fire->Extinguish();
				continue;
			}

			// 水以外なら火に触れたことを通知
			return true;
		}
	}

	return false;
}
// 指定したマスのブロックだけ更新
void MapChip::UpdateBlock(int row, int column) {

	const int value = mapData_[row][column];
	// 範囲チェック
	blocks_[row][column].reset();
	fires_[row][column].reset();
	// 空きマスまたはプレイヤー開始位置
	if (value == 0 || value == 3) {
		return;
	}
	// マスの位置を計算
    Vector3 position{
	static_cast<float>(column) * blockSize_,
	static_cast<float>(static_cast<int>(mapData_.size()) - 1 - row) * blockSize_, 0.0f};


	if (value == 6) {
		fires_[row][column] = std::make_unique<Fire>();
		fires_[row][column]->Initialize(position,blockSize_);
		return;
	}

	// ブロックの種類とゴール判定を決定
	BlockType type = BlockType::Normal;
	bool isGoal = false;

	if (value == 1) {
		// 通常ブロック
		type = BlockType::Normal;
	} else if (value == 2) {
		// ゴール
		type = BlockType::Normal;
		isGoal = true;
	} else if (value == 4) {
		// 燃えるブロック
		type = BlockType::Burnable;
	} else if (value == 5) {
		// 網目状ブロック
		type = BlockType::Mesh;
	} else {
		return;
	}

	// 既存ブロックの種類とゴール判定が同じなら何もしない
	if (blocks_[row][column] && blocks_[row][column]->GetType() == type && blocks_[row][column]->IsGoal() == isGoal) {
		return;
	}

	// 種類が違う場合は作り直す
	blocks_[row][column].reset();

	blocks_[row][column] = std::make_unique<Block>();

	blocks_[row][column]->Initialize(position, blockSize_, isGoal, type);
}

// プレイヤー開始位置を更新
void MapChip::UpdatePlayerSpawnPosition() {
	hasPlayerSpawnPosition_ = false;

	for (size_t row = 0; row < mapData_.size(); ++row) {
		for (size_t column = 0; column < mapData_[row].size(); ++column) {

			if (mapData_[row][column] == 3) {

				playerSpawnPosition_ = {static_cast<float>(column) * blockSize_, static_cast<float>(static_cast<int>(mapData_.size()) - 1 - static_cast<int>(row)) * blockSize_, 0.0f};

				hasPlayerSpawnPosition_ = true;
				return;
			}
		}
	}
}