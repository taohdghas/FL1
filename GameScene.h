#pragma once

#include <KamataEngine.h>
using namespace KamataEngine;

#include <string>

#include "MapChip.h"
#include "Scene.h"

class GameScene : public Scene {
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

private:
	// マップエディタの描画
	void DrawMapEditor();
	//カメラ移動(デバック用)
	void UpdateCamera();
	//ImGui更新
	void UpdateImGui();

private:
	Camera camera_;
	MapChip mapChip_;

	// true：配置、false：削除
	bool isPlaceMode_ = true;

	// CSVファイルのパス
	std::string csvPath_ = "Resources/mapdata/stage1.csv";
	char csvPathBuffer_[256]{};

	// 編集結果やエラーの表示
	std::string message_;
};