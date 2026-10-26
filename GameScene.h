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
	// UI
	std::unique_ptr<UI> ui_;
	Player player_;
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
	//数字同士の間隔
	float resetPercentSpacing_ = 4.0f;
	//リセット進行率
	int resetPercent_ = 0;
	// CSVファイルのパス
	std::string csvPath_ = "Resources/mapdata/stage1.csv";
	char csvPathBuffer_[256]{};
	// 編集結果やエラーの表示
	std::string message_;
	// true：配置、false：削除
	bool isPlaceMode_ = true;
	//リセットキー長押し時間
	float resetHoldTime_ = 0.0f;
	//リセット必要長押し時間
	const float resetRequiredTime_ = 1.5f;
	//リセット処理重複防止
	bool isResetRequested_ = false;
};