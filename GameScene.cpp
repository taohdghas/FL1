
#include "GameScene.h"

#include <cstring>
#include <imgui.h>

void GameScene::Initialize() {
	// カメラの初期化
	camera_.Initialize();
	camera_.translation_ = {20.0f, 10.0f, -30.0f};

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
	// マップチップの更新
	mapChip_.Update();
	// マップエディタ
	DrawMapEditor();

	#ifdef _DEBUG
	// カメラ移動(デバック用)
	UpdateCamera();
	// ImGui更新
	UpdateImGui();
#endif

	// カメラ行列を更新
	camera_.UpdateMatrix();
	camera_.TransferMatrix();
}

void GameScene::Draw() {
	Model::PreDraw();

	mapChip_.Draw(camera_);

	Model::PostDraw();
}

void GameScene::DrawMapEditor() {
#ifdef _DEBUG
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
// カメラ移動(デバック用)
void GameScene::UpdateCamera() {
	// マウス中ボタンを押しながらドラッグするとカメラを移動
	if (Input::GetInstance()->IsPressMouse(2)) {
		Input::MouseMove mouseMove = Input::GetInstance()->GetMouseMove();

		const float cameraMoveSpeed = 0.02f;

		camera_.translation_.x += static_cast<float>(mouseMove.lX) * cameraMoveSpeed;
		camera_.translation_.y -= static_cast<float>(mouseMove.lY) * cameraMoveSpeed;
	}

	// マウスホイールでカメラのZ座標を移動
	const int32_t wheel = Input::GetInstance()->GetWheel();
	const float zoomSpeed = 0.01f;

	camera_.translation_.z += static_cast<float>(wheel) * zoomSpeed;

	// カメラ行列を更新
	camera_.UpdateMatrix();
	camera_.TransferMatrix();
}
//ImGui更新
void GameScene::UpdateImGui() { 
	//カメラ座標表示
	ImGui::Begin("Camera Info");
	ImGui::Text("Camera Position");
	ImGui::Separator();

	// 現在値表示
	ImGui::Text("X: %.2f", camera_.translation_.x);
	ImGui::Text("Y: %.2f", camera_.translation_.y);
	ImGui::Text("Z: %.2f", camera_.translation_.z);

	ImGui::Separator();
		
	// カメラ移動用
	bool changed = false;
	if (ImGui::DragFloat3("Translation", &camera_.translation_.x, 0.1f, -1000.0f, 1000.0f, "%.2f")) changed = true;

	// スライダーで変化があればカメラ行列を即時更新
	if (changed) {
		camera_.UpdateMatrix();
		camera_.TransferMatrix();
	}

	ImGui::End();

}