#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

#include "Block.h"
#include "Player.h"
#include "Fire.h"
#include <memory>
#include <string>
#include <vector>

class MapChip {
public:
	MapChip() = default;
	~MapChip();

	bool Initialize(const std::string& csvPath, float blockSize = 2.0f);
	void Update();
	void Draw(const Camera& camera);
	void Finalize();
	// Playerと各BlockのAABB衝突を解決する
	void ResolvePlayerCollision(Player& player) ;
	// プレイヤーが終点に到達したか（AABB重なり）
	bool HasPlayerReachedGoal(const Player& player) const;
	//火のギミックとの接触判定
	bool ResolveFireCollision(Player& player);
	// マップに終点が存在するか
	bool HasGoal() const { return hasGoal_; }
	// マップの1マスのサイズを取得
	bool SaveCSV(const std::string& csvPath) ;
	// CSV読み込み
	bool LoadCSV(const std::string& csvPath);
	// マスの値を変更
	bool SetCell(int row, int column, int value);
	// マスの値を取得
	int GetCell(int row, int column) ;
	// 行数を取得
	int GetRowCount() { return static_cast<int>(mapData_.size());}
	//	
	int GetColumnCount() ;
	// プレイヤーのスポーン位置を取得
	bool GetPlayerSpawnPosition(Vector3& position);
private:
	// 指定したマスのブロックだけを更新する
	void UpdateBlock(int row, int column);
	// CSVのプレイヤー開始位置を調べる
	void UpdatePlayerSpawnPosition();
private:
	// マップデータ
	std::vector<std::vector<int>> mapData_;
	// 各マスに対応するブロック
	// nullptrならブロックなし
	std::vector<std::vector<std::unique_ptr<Block>>> blocks_;
	//火のギミック管理
	std::vector<std::vector<std::unique_ptr<Fire>>> fires_;
	//ブロックの1マスのサイズ
	float blockSize_ = 2.0f;
	//プレイヤー開始位置
	Vector3 playerSpawnPosition_{};
	// プレイヤー開始位置が設定されているか
	bool hasPlayerSpawnPosition_ = false;
	// 終点が存在するか
	bool hasGoal_ = false;
};