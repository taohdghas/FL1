#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

#include <string>
#include "Player.h"
#include "MapChip.h"
#include "UI.h"

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

private:
	Camera camera_;
	MapChip mapChip_;
	// ポーズメニュー
	std::unique_ptr<UI> ui_;
	Player player_;
	// true：配置、false：削除
	bool isPlaceMode_ = true;
	// CSVファイルのパス
	std::string csvPath_ = "Resources/mapdata/stage1.csv";
	char csvPathBuffer_[256]{};
	// 編集結果やエラーの表示
	std::string message_;
	//リセットキー長押し時間
	float resetHoldTime_ = 0.0f;
	//リセット必要長押し時間
	const float resetRequiredTime_ = 1.0f;
	//リセット処理重複防止
	bool isResetRequired_ = false;
};