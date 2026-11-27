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
	// スイッチ描画
	for (auto& row : switches_) {
		for (auto& sw : row) {
			if (sw) {
				sw->Draw(camera);
			}
		}
	}

	// 扉描画
	for (auto& row : doors_) {
		for (auto& door : row) {
			if (door) {
				door->Draw(camera);
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
			// 7001～7999: スイッチ
			// 8001～8999: ドア
			bool isValidValue = (value >= 0 && value <= 6) || (value >= 7001 && value <= 7999) || (value >= 8001 && value <= 8999);

			if (!isValidValue) {
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
	fires_.clear();
	switches_.clear();
	doors_.clear();

	blocks_.resize(mapData_.size());
	fires_.resize(mapData_.size());
	switches_.resize(mapData_.size());
	doors_.resize(mapData_.size());

	hasGoal_ = false;

	for (size_t row = 0; row < mapData_.size(); ++row) {
		blocks_[row].resize(mapData_[row].size());
		fires_[row].resize(mapData_[row].size());
		switches_[row].resize(mapData_[row].size());
		doors_[row].resize(mapData_[row].size());

		for (size_t column = 0; column < mapData_[row].size(); ++column) {
			// 終点が存在するかチェック
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
	// CSVの内容をファイルに書き込む
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
	// 範囲チェック
	if (column < 0 || column >= static_cast<int>(mapData_[row].size())) {
		return false;
	}
	// 値の範囲チェック
	bool isValidValue = (value >= 0 && value <= 6) || (value >= 7001 && value <= 7999) || (value >= 8001 && value <= 8999);

	if (!isValidValue) {
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

	// 変更したマスのブロックを更新
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

	// プレイヤーの現在位置と前フレーム位置
	Vector3 pos = player.GetPosition();
	const Vector3 prevPos = player.GetPreviousPosition();
	const Vector3 playerHalf = player.GetHalfSize();

	const ElementType playerElement = player.GetElement();

	// 接地フラグ
	bool grounded = false;
	// 垂直速度
	float newVerticalVelocity = player.GetVerticalVelocity();
	// 前フレームからのY方向の移動量
	const float moveY = pos.y - prevPos.y;
	// 網目の隙間に合わせた位置補正
	CorrectMeshGapPosition(pos, prevPos, playerHalf);
	// Blockとの衝突判定
	ResolveBlockCollision(pos, prevPos, playerHalf, playerElement, moveY, newVerticalVelocity, grounded);
	// Doorとの衝突判定
	ResolveDoorCollision(pos, prevPos, playerHalf, newVerticalVelocity, grounded);
	// 解決後の位置を反映
	player.SetPosition(pos);
	player.SetVerticalVelocity(newVerticalVelocity);
	player.SetGrounded(grounded);
}
// Playerと電気スイッチの当たリ判定
void MapChip::ResolveSwitchCollision(Player& player) {

	// 電気エレメント以外なら何もしない
	if (player.GetElement() != ElementType::Electric) {
		return;
	}

	const Vector3& playerPos = player.GetPosition();
	const Vector3& playerHalf = player.GetHalfSize();
	// 全Switchを調べる
	for (auto& row : switches_) {
		for (auto& sw : row) {

			if (!sw || sw->IsActive()) {
				continue;
			}
			const Vector3& switchPos = sw->GetPosition();
			const Vector3& switchHalf = sw->GetHalfSize();
			// AABBによる接触判定
			bool overlapX = playerPos.x - playerHalf.x < switchPos.x + switchHalf.x && playerPos.x + playerHalf.x > switchPos.x - switchHalf.x;
			bool overlapY = playerPos.y - playerHalf.y < switchPos.y + switchHalf.y && playerPos.y + playerHalf.y > switchPos.y - switchHalf.y;
			bool overlapZ = playerPos.z - playerHalf.z < switchPos.z + switchHalf.z && playerPos.z + playerHalf.z > switchPos.z - switchHalf.z;

			if (overlapX && overlapY && overlapZ) {
				// SwitchをONにする
				sw->Activate();
				// 同じグループIDのDoorを開く
				int groupId = sw->GetGroupId();

				for (auto& doorRow : doors_) {
					for (auto& door : doorRow) {

						if (!door) {
							continue;
						}
						// 同じグループIDのDoorを開く
						if (door->GetGroupId() == groupId) {
							door->Open();
						}
					}
				}
			}
		}
	}
}
// 網目ブロックの位置補正
void MapChip::CorrectMeshGapPosition(Vector3& pos, const Vector3& prevPos, const Vector3& playerHalf) {
	const float gapMoveX = pos.x - prevPos.x;
	const float gapMoveY = pos.y - prevPos.y;
	const float epsilon = 0.0001f;

	// 上下方向に移動していて、横方向には移動していない場合だけ
	if (std::abs(gapMoveX) <= epsilon && std::abs(gapMoveY) > epsilon) {
		// プレイヤーに最も近いマスの中心X座標
		const int candidateColumn = static_cast<int>(std::round(pos.x / blockSize_));
		// 左右にブロックが存在する列だけを対象にする
		if (candidateColumn > 0 && candidateColumn + 1 < static_cast<int>(blocks_[0].size())) {

			const float targetX = static_cast<float>(candidateColumn) * blockSize_;
			// マスの中心から半マス以内にいる場合だけ補正候補にする
			if (std::abs(pos.x - targetX) <= blockSize_ * 0.5f) {

				// 前フレームから現在までのプレイヤーの上下範囲
				const float playerTop = (pos.y > prevPos.y ? pos.y : prevPos.y) + playerHalf.y;
				const float playerBottom = (pos.y < prevPos.y ? pos.y : prevPos.y) - playerHalf.y;

				for (size_t row = 0; row < blocks_.size(); ++row) {

					// 隙間の中央にブロックがないことを確認
					Block* centerBlock = blocks_[row][candidateColumn].get();

					if (centerBlock != nullptr && centerBlock->IsActive() && !centerBlock->IsGoal()) {
						continue;
					}
					// 左右のブロックを取得
					Block* leftBlock = blocks_[row][candidateColumn - 1].get();
					Block* rightBlock = blocks_[row][candidateColumn + 1].get();
					// 左右両方に有効なブロックが必要
					const bool hasLeft = leftBlock != nullptr && leftBlock->IsActive() && !leftBlock->IsGoal();
					const bool hasRight = rightBlock != nullptr && rightBlock->IsActive() && !rightBlock->IsGoal();

					if (!hasLeft || !hasRight) {
						continue;
					}

					// このマスのY座標
					const float cellY = static_cast<float>(blocks_.size() - 1 - row) * blockSize_;
					const float cellHalf = blockSize_ * 0.5f;
					// プレイヤーが上下方向からこのマスに接近しているか
					const bool overlapsVertically = playerTop >= cellY - cellHalf && playerBottom <= cellY + cellHalf;

					if (overlapsVertically) {

						// 補正の強さ
						const float correctionStrength = 0.1f;
						// 隙間の中央へ少しだけ近づける
						pos.x += (targetX - pos.x) * correctionStrength;

						break;
					}
				}
			}
		}
	}
}
// PlayerとブロックのAABB衝突判定
void MapChip::ResolveBlockCollision(Vector3& pos, const Vector3& prevPos, const Vector3& playerHalf, ElementType playerElement, float moveY, float& newVerticalVelocity, bool& grounded) {
	const float epsilon = 0.0001f;
	// 各ブロックとの衝突判定
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
			// 現在位置での重なり判定
			const bool overlapX = std::abs(pos.x - blockPos.x) < (playerHalf.x + blockHalf.x);
			const bool overlapY = std::abs(pos.y - blockPos.y) < (playerHalf.y + blockHalf.y);
			const bool overlapZ = std::abs(pos.z - blockPos.z) < (playerHalf.z + blockHalf.z);

			if (!(overlapX && overlapY && overlapZ)) {
				continue;
			}
			// 火エレメントで燃えるブロックに触れた場合
			if (playerElement == ElementType::Fire && block->IsBurnable()) {

				block->Burn();
				continue;
			}
			// 水・気体エレメントなら網目ブロックを通過
			if (playerElement == ElementType::Gas && block->GetType() == BlockType::Mesh) {

				continue;
			}
			// 縦方向の隙間を通過中か確認
			const float moveX = pos.x - prevPos.x;

			if (std::abs(moveY) > epsilon && std::abs(moveX) <= epsilon) {

				const int candidateColumn = static_cast<int>(std::round(pos.x / blockSize_));

				if (candidateColumn > 0 && candidateColumn + 1 < static_cast<int>(blocks_[row].size())) {

					const float targetX = candidateColumn * blockSize_;

					// 隙間の中央付近にいる場合
					if (std::abs(pos.x - targetX) < blockSize_ * 0.5f) {

						Block* centerBlock = blocks_[row][candidateColumn].get();
						Block* leftBlock = blocks_[row][candidateColumn - 1].get();
						Block* rightBlock = blocks_[row][candidateColumn + 1].get();
						// 中央のブロックが空きマスか、ゴールなら左右の衝突を無視
						const bool centerIsEmpty = centerBlock == nullptr || !centerBlock->IsActive() || centerBlock->IsGoal();
						// 左右のブロックが有効か
						const bool leftIsSolid = leftBlock != nullptr && leftBlock->IsActive() && !leftBlock->IsGoal();
						const bool rightIsSolid = rightBlock != nullptr && rightBlock->IsActive() && !rightBlock->IsGoal();
						const bool isSideBlock = static_cast<int>(column) == candidateColumn - 1 || static_cast<int>(column) == candidateColumn + 1;
						// 左右に挟まれた空きマスなら衝突を無視
						if (centerIsEmpty && leftIsSolid && rightIsSolid && isSideBlock) {

							continue;
						}
					}
				}
			}

			// 衝突判定のAABBの上下座標
			const float blockTop = blockPos.y + blockHalf.y;
			const float blockBottom = blockPos.y - blockHalf.y;
			// 前フレームでY方向に重なっていたか
			const bool prevOverlapY = std::abs(prevPos.y - blockPos.y) < (playerHalf.y + blockHalf.y);
			// Y方向の衝突判定
			const bool hitCeiling = moveY > 0.0f && (prevPos.y + playerHalf.y <= blockBottom + epsilon) && (pos.y + playerHalf.y > blockBottom);
			// 上から落下してブロックの上面に着地
			const bool hitFloor = moveY < 0.0f && (prevPos.y - playerHalf.y >= blockTop - epsilon) && (pos.y - playerHalf.y < blockTop);
			// 頭打ち
			if (hitCeiling) {
				pos.y = blockBottom - playerHalf.y;
				newVerticalVelocity = 0.0f;
				continue;
			}
			// 着地
			if (hitFloor) {
				pos.y = blockTop + playerHalf.y;
				newVerticalVelocity = 0.0f;
				grounded = true;
				continue;
			}
			// X方向の衝突判定
			if (prevOverlapY) {

				// X軸で押し戻す
				if (pos.x > blockPos.x) {
					pos.x = blockPos.x + blockHalf.x + playerHalf.x;
				} else {
					pos.x = blockPos.x - blockHalf.x - playerHalf.x;
				}
			}
		}
	}
}
// PlayerとドアのAABB衝突判定
void MapChip::ResolveDoorCollision(Vector3& pos, const Vector3& prevPos, const Vector3& playerHalf, float& newVerticalVelocity, bool& grounded) {
	const float epsilon = 0.0001f;
	// Doorとの衝突判定
	for (size_t row = 0; row < doors_.size(); ++row) {
		for (size_t column = 0; column < doors_[row].size(); ++column) {

			Door* door = doors_[row][column].get();

			// Doorが存在しない、または開いている場合は無視
			if (door == nullptr || door->IsOpen()) {
				continue;
			}

			const Vector3 doorPos = door->GetPosition();
			const Vector3 doorHalf = door->GetHalfSize();

			// 現在位置での重なり判定
			const bool overlapX = std::abs(pos.x - doorPos.x) < (playerHalf.x + doorHalf.x);
			const bool overlapY = std::abs(pos.y - doorPos.y) < (playerHalf.y + doorHalf.y);
			const bool overlapZ = std::abs(pos.z - doorPos.z) < (playerHalf.z + doorHalf.z);

			if (!(overlapX && overlapY && overlapZ)) {
				continue;
			}

			// Doorの上下座標
			const float doorTop = doorPos.y + doorHalf.y;
			const float doorBottom = doorPos.y - doorHalf.y;
			// 前フレームでY方向に重なっていたか
			const bool prevOverlapY = std::abs(prevPos.y - doorPos.y) < (playerHalf.y + doorHalf.y);
			// 前フレームからのY方向の移動量
			const float doorMoveY = pos.y - prevPos.y;
			// 上方向への衝突（頭打ち）
			const bool hitCeiling = doorMoveY > 0.0f && (prevPos.y + playerHalf.y <= doorBottom + epsilon) && (pos.y + playerHalf.y > doorBottom);
			// 下方向への衝突（着地）
			const bool hitFloor = doorMoveY < 0.0f && (prevPos.y - playerHalf.y >= doorTop - epsilon) && (pos.y - playerHalf.y < doorTop);
			// 頭打ち
			if (hitCeiling) {
				pos.y = doorBottom - playerHalf.y;
				newVerticalVelocity = 0.0f;
				continue;
			}
			// 着地
			if (hitFloor) {
				pos.y = doorTop + playerHalf.y;
				newVerticalVelocity = 0.0f;
				grounded = true;
				continue;
			}
			// X方向・Z方向の衝突判定
			if (prevOverlapY) {

				// X軸で押し戻す
				if (pos.x > doorPos.x) {
					pos.x = doorPos.x + doorHalf.x + playerHalf.x;
				} else {
					pos.x = doorPos.x - doorHalf.x - playerHalf.x;
				}
			}
		}
	}
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
			// AABBによる重なり判定
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
	switches_[row][column].reset();
	doors_[row][column].reset();
	// 空きマスまたはプレイヤー開始位置
	if (value == 0 || value == 3) {
		return;
	}
	// マスの位置を計算
	Vector3 position{static_cast<float>(column) * blockSize_, static_cast<float>(static_cast<int>(mapData_.size()) - 1 - row) * blockSize_, 0.0f};
	// 火のギミックを生成
	if (value == 6) {
		fires_[row][column] = std::make_unique<Fire>();
		fires_[row][column]->Initialize(position, blockSize_);
		return;
	}
	// スイッチ
	if (value >= 7001 && value <= 7999) {
		int groupId = value - 7000;
		switches_[row][column] = std::make_unique<Switch>();
		switches_[row][column]->Initialize(position, groupId, blockSize_);
		return;
	}
	// ドア
	if (value >= 8001 && value <= 8999) {
		int groupId = value - 8000;
		doors_[row][column] = std::make_unique<Door>();
		doors_[row][column]->Initialize(position, groupId, blockSize_);
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
	blocks_[row][column]->Initialize(position, isGoal, type);
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