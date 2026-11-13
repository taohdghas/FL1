#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

#include <string>
#include <vector>

#include "ElementSystem.h"
#include "ElementUI.h"
#include "MapChip.h"
#include "Player.h"
#include "UI.h"
#include "Fire.h"

#include "Scene.h"

class GameScene : public Scene {
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

private:
	// リセット
	void Reset();
	// マップエディタの描画
	void DrawMapEditor();
	// カメラ移動(デバック用)
	void UpdateCamera();
	// ImGui更新
	void UpdateImGui();
	// プレイヤーのワールド座標をスクリーンへ変換
	Vector2 GetPlayerScreenPosition();
	// 指定ステージの読み込み
	void LoadStage(int index);
	// 終点到達チェック → 次ステージ or エンディング
	void CheckGoal();

private:
	Camera camera_;   // カメラ
	MapChip mapChip_; // マップチップ
	// UI
	std::unique_ptr<UI> ui_;
	Player player_;

	// ----- ステージ管理 -----
	std::vector<std::string> stagePaths_ = {
	    "Resources/mapdata/stage1.csv",
	    "Resources/mapdata/stage2.csv",
	    "Resources/mapdata/stage3.csv",
	};
	int currentStageIndex_ = 0;
	// クリア処理の多重発火防止
	bool isClearTriggered_ = false;

	// リセットゲージスプライト
	Sprite* resetMeterSprite_;
	// リセットゲージ座標
	Vector2 resetMeterPosition_ = {500.0f, 120.0f};
	// リセットゲージのサイズ
	Vector2 resetMeterSize_ = {300.0f, 30.0f};
	// 数字スプライト
	std::array<std::array<Sprite*, 10>, 3> resetPercentSprites_{};
	//%の位置
	Vector2 resetPercentPosition_ = {645.0f, 70.0f};
	// 数字1文字サイズ
	Vector2 resetPercentDigitSize_ = {24.0f, 32.0f};
	//%スプライト
	Sprite* resetPercentSymbolSprite_;
	//%記号のサイズ
	Vector2 resetPercentSymbolSize_ = {24.0f, 32.0f};
	// %スプライトの位置調整用オフセット
	Vector2 resetPercentSymbolOffset_ = {0.0f, 0.0f};
	// プレイヤーのスクリーン座標
	Vector2 playerScreenPosition_{};
	// エレメントシステム
	std::unique_ptr<ElementSystem> elementSystem_;
	// エレメントUI
	std::unique_ptr<ElementUI> elementUI_;
	// 数字同士の間隔
	float resetPercentSpacing_ = 4.0f;
	// リセット進行率
	int resetPercent_ = 0;
	// マップエディタ用
	int selectedCellType_ = 1;
	// CSVファイルのパス
	std::string csvPath_ = "Resources/mapdata/stage1.csv";
	char csvPathBuffer_[256]{};
	// 編集結果やエラーの表示
	std::string message_;
	// マップ編集モード
	enum class EditMode {
		Place, // 1 を配置
		Erase, // 0 に戻す
		Goal   // 2（終点）を配置
	};
	EditMode editMode_ = EditMode::Place;
	// リセットキー長押し時間
	float resetHoldTime_ = 0.0f;
	// リセット必要長押し時間
	const float resetRequiredTime_ = 1.5f;
	// リセット処理重複防止
	bool isResetRequested_ = false;
	// 火エレメントの効果時間
	const float fireDuration_ = 5.0f;
};