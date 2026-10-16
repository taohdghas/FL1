
#include "GameScene.h"

#include <cstring>
#include <imgui.h>

void GameScene::Initialize() {
	// カメラの初期化
	camera_.Initialize();
	camera_.translation_ = {0.0f, 0.0f, -10.0f};
	camera_.UpdateMatrix();

	// CSVパスを入力欄に設定
	strncpy_s(csvPathBuffer_, sizeof(csvPathBuffer_), csvPath_.c_str(), _TRUNCATE);

	// マップを読み込む
	if (mapChip_.Initialize(csvPath_, 2.0f)) {
		message_ = "Map loaded.";
	} else {
		message_ = "Failed to load map.";
	}
}

void GameScene::Finalize() { mapChip_.Finalize(); }

void GameScene::Update() {
	mapChip_.Update();

	// マップエディタ
	DrawMapEditor();
}

void GameScene::Draw() {
	Model::PreDraw();

	mapChip_.Draw(camera_);

	Model::PostDraw();
}

void GameScene::DrawMapEditor() {
#ifdef _DEBUG
	// ImGuiのフレーム開始・終了はmain.cpp側で行う
	ImGui::Begin("Map Editor");

	// CSVファイルのパス
	ImGui::Text("CSV File");
	ImGui::SetNextItemWidth(240.0f);

	if (ImGui::InputText("##CSVPath", csvPathBuffer_, sizeof(csvPathBuffer_))) {
		csvPath_ = csvPathBuffer_;
	}

	// CSV保存
	ImGui::SameLine();

	if (ImGui::Button("Save")) {
		csvPath_ = csvPathBuffer_;

		if (mapChip_.SaveCSV(csvPath_)) {
			message_ = "Saved successfully.";
		} else {
			message_ = "Failed to save CSV.";
		}
	}

	// CSV読み込み
	if (ImGui::Button("Load")) {
		csvPath_ = csvPathBuffer_;

		if (mapChip_.LoadCSV(csvPath_)) {
			message_ = "Loaded successfully.";
		} else {
			message_ = "Failed to load CSV.";
		}
	}

	ImGui::Separator();

	// 配置・削除モード
	ImGui::Text("Edit Mode");
	ImGui::SameLine();

	if (ImGui::RadioButton("Place", isPlaceMode_)) {
		isPlaceMode_ = true;
	}

	ImGui::SameLine();

	if (ImGui::RadioButton("Erase", !isPlaceMode_)) {
		isPlaceMode_ = false;
	}

	ImGui::Separator();

	// マップをマス目のボタンとして表示
	const int rowCount = mapChip_.GetRowCount();
	const int columnCount = mapChip_.GetColumnCount();

	for (int row = 0; row < rowCount; ++row) {
		for (int column = 0; column < columnCount; ++column) {
			const int cell = mapChip_.GetCell(row, column);

			// ブロックあり：[1]、空き：[0]
			const char* label = cell == 1 ? "[1]" : "[0]";

			// ボタンのIDが重複しないようにする
			ImGui::PushID(row);
			ImGui::PushID(column);

			if (ImGui::Button(label)) {
				const int newValue = isPlaceMode_ ? 1 : 0;

				if (mapChip_.SetCell(row, column, newValue)) {
					message_ = "Map edited.";
				}
			}

			ImGui::PopID();
			ImGui::PopID();

			if (column + 1 < columnCount) {
				ImGui::SameLine();
			}
		}
	}

	ImGui::Separator();
	ImGui::Text("%s", message_.c_str());

	ImGui::End();
#endif
}
