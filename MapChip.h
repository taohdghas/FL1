#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

#include "Block.h"
#include "Player.h"
#include "Fire.h"
#include "Switch.h"
#include "Door.h"
#include <memory>
#include <string>
#include <vector>
#include <array>

class MapChip {
public:
	MapChip() = default;
	~MapChip();
	//初期化
	bool Initialize(const std::string& csvPath, float blockSize = 2.0f);
	void Update();
	void Draw(const Camera& camera);
	void Finalize();
	// Playerと各BlockのAABB衝突判定
	void ResolvePlayerCollision(Player& player) ;
	//Playerと電気スイッチの当たリ判定
	void ResolveSwitchCollision(Player& player);
	//網目ブロックの位置補正
	void CorrectMeshGapPosition(Vector3&pos,const Vector3&prevPos,const Vector3&playerHalf);
	// PlayerとブロックのAABB衝突判定
	void ResolveBlockCollision(Vector3& pos, const Vector3& prevPos, const Vector3& playerHalf,
		ElementType playerElement, float moveY, float& newVerticalVelocity, bool& grounded);
	// PlayerとドアのAABB衝突判定
	void ResolveDoorCollision(Vector3&pos,const Vector3&prevPos,const Vector3&playerHalf,
		float&newVerticalVelocity,bool & grounded);
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
	// 小ブロックの配置・取得
	bool SetSmallBlock(int row, int column, int quadrant, bool place);
	bool HasSmallBlock(int row, int column, int quadrant) const;
	// マスの値を取得
	int GetCell(int row, int column) ;
	// 行数を取得
	int GetRowCount() { return static_cast<int>(mapData_.size());}
	// 列数を取得
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
	std::vector<std::vector<std::unique_ptr<Block>>> blocks_;
	//火のギミック管理
	std::vector<std::vector<std::unique_ptr<Fire>>> fires_;
	// スイッチ管理
	std::vector<std::vector<std::unique_ptr<Switch>>> switches_;
	// ドア管理
	std::vector<std::vector<std::unique_ptr<Door>>> doors_;
	//小ブロックの配置データ
	std::vector<std::vector<std::array<bool, 4>>> smallBlockData_;
	//小ブロックの実体
	std::vector<std::vector<std::array<std::unique_ptr<Block>, 4>>> smallBlocks_;
	//ブロックの1マスのサイズ
	float blockSize_ = 2.0f;
	//プレイヤー開始位置
	Vector3 playerSpawnPosition_{};
	// プレイヤー開始位置が設定されているか
	bool hasPlayerSpawnPosition_ = false;
	// 終点が存在するか
	bool hasGoal_ = false;
};