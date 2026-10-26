#include "GameScene.h"
#include "SceneManager.h"
#include <cstring>
#include <imgui.h>

void GameScene::Initialize() {
	// カメラの初期化
	camera_.translation_ = {20.0f, 10.0f, -30.0f};
	camera_.Initialize();

	// CSVパスを入力欄に設定
	strncpy_s(csvPathBuffer_, sizeof(csvPathBuffer_), csvPath_.c_str(), _TRUNCATE);

	// プレイヤーを初期化（仮の地面の高さはY=0.5）
	player_.Initialize({0.0f, 0.5f, 0.0f});

	// マップを読み込む
	if (mapChip_.Initialize(csvPath_, 2.0f)) {
		message_ = "Map loaded.";
	} else {
		message_ = "Failed to load map.";
	}

	// ポーズメニュー
	ui_ = std::make_unique<UI>();
	ui_->Initialize();
	// リセットゲージスプライト
	resetMeterSprite_ = Sprite::Create(TextureManager::Load("resetmeter.png"), resetMeterPosition_);
	resetMeterSprite_->SetSize(resetMeterSize_);
	// 数字スプライト
	//   リセット進行率の数字スプライトを作成
	for (int digitPosition = 0; digitPosition < 3; ++digitPosition) {
		for (int number = 0; number < 10; ++number) {
			std::string texturePath = "0.png";
			texturePath = std::to_string(number) + ".png";
			resetPercentSprites_[digitPosition][number] = Sprite::Create(TextureManager::Load(texturePath), resetPercentPosition_);
			resetPercentSprites_[digitPosition][number]->SetSize(resetPercentDigitSize_);
		}
	}
	//%記号
	resetPercentSymbolSprite_ = Sprite::Create(TextureManager::Load("percent.png"), resetPercentPosition_);
	resetPercentSymbolSprite_->SetSize(resetPercentSymbolSize_);
}
// 終了
void GameScene::Finalize() {
	mapChip_.Finalize();      // マップチップ
	ui_->Finalize();          // ポーズメニュー
	delete resetMeterSprite_; // リセットゲージスプライト
	// 数字スプライトの削除
	for (int digitPosition = 0; digitPosition < 3; ++digitPosition) {
		for (int number = 0; number < 10; ++number) {
			delete resetPercentSprites_[digitPosition][number];
			resetPercentSprites_[digitPosition][number] = nullptr;
		}
	}
	delete resetPercentSymbolSprite_; //%スプライト
}
//更新
void GameScene::Update() {
	// マップチップの更新
	mapChip_.Update();
	player_.Update();

	// マップエディタ
	DrawMapEditor();
	// UI更新
	ui_->Update();

	switch (ui_->GetPauseMenuAction()) {
	case UI::PauseMenuAction::ShowControls:
		message_ = "Show Controls selected.";
		break;
	case UI::PauseMenuAction::ToSelect:
		sceneManager_->ChangeScene(SceneType::Select);
		break;
	case UI::PauseMenuAction::ToTitle:
		sceneManager_->ChangeScene(SceneType::Title);
		break;
	case UI::PauseMenuAction::None:
		break;
	default:
		break;
	}

#ifdef _DEBUG
	// UIデバック
	ui_->Debug();
	// カメラ移動(デバック用)
	UpdateCamera();
	// ImGui更新
	UpdateImGui();
#endif

	// ポーズメニューが表示されているならスキップ
	if (ui_->IsPauseMenuActive()) {
		return;
	}
	// リセット
	Reset();
	// カメラ行列を更新
	camera_.UpdateMatrix();
	camera_.TransferMatrix();
}

void GameScene::Draw() {
	Model::PreDraw();

	mapChip_.Draw(camera_);
	player_.Draw(camera_);

	Model::PostDraw();

	Sprite::PreDraw();

	// ポーズメニュー
	ui_->Draw();
	// リセット
	if (resetHoldTime_ > 0.0f && resetMeterSprite_) {
		// リセットゲージ
		resetMeterSprite_->Draw();
		// 進行率を文字列に変換
		std::string percentText = std::to_string(resetPercent_);

		int digitCount = static_cast<int>(percentText.size());
		// 数字全体の幅を計算して中央揃え
		float totalWidth = digitCount * resetPercentDigitSize_.x + resetPercentSymbolSize_.x + digitCount * resetPercentSpacing_;

		float startX = resetPercentPosition_.x - totalWidth / 2.0f;

		// 各桁の数字を描画
		for (int i = 0; i < digitCount; ++i) {
			int number = percentText[i] - '0';

			Sprite* digitSprite = resetPercentSprites_[i][number];

			digitSprite->SetPosition({startX + i * (resetPercentDigitSize_.x + resetPercentSpacing_), resetPercentPosition_.y});
			digitSprite->SetSize(resetPercentDigitSize_);
			digitSprite->Draw();
		}
		//%を数字に右側に描画
		resetPercentSymbolSprite_->SetPosition({startX + digitCount * (resetPercentDigitSize_.x + resetPercentSpacing_), resetPercentPosition_.y});
		//%の描画
		resetPercentSymbolSprite_->SetSize(resetPercentSymbolSize_);
		resetPercentSymbolSprite_->Draw();
	}

	Sprite::PostDraw();
}
// リセット
void GameScene::Reset() {
	// Rキーの長押しでリセット
	if (Input::GetInstance()->PushKey(DIK_R)) {
		resetHoldTime_ += 1.0f / 60.0f;
		// 長押し時間を0～1の割合に変換
		float rate = resetHoldTime_ / resetRequiredTime_;
		// 進行率を0～1の範囲に制限
		if (rate > 1.0f) {
			rate = 1.0f;
		}
		// 進行率を整数に変換
		resetPercent_ = static_cast<int>(rate * 100.0f);

		// ゲージを左から右へ伸ばす
		resetMeterSprite_->SetSize({resetMeterSize_.x * rate, resetMeterSize_.y});
		// 必要時間まで長押ししたらリセット
		if (resetHoldTime_ >= resetRequiredTime_ && !isResetRequested_) {
			isResetRequested_ = true;
			sceneManager_->ChangeScene(SceneType::Game);
		}
	} else {
		// Rキーを離したらゲージをリセット
		resetHoldTime_ = 0.0f;
		resetPercent_ = 0;
		isResetRequested_ = false;

		resetMeterSprite_->SetSize({0.0f, resetMeterSize_.y});
	}
}
// マップエディタの描画
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
// ImGui更新
// ImGui更新
void GameScene::UpdateImGui() {
#ifdef _DEBUG
	// カメラ座標表示
	ImGui::Begin("Camera Info");

	ImGui::Text("Camera Position");
	ImGui::Separator();

	ImGui::Text("X: %.2f", camera_.translation_.x);
	ImGui::Text("Y: %.2f", camera_.translation_.y);
	ImGui::Text("Z: %.2f", camera_.translation_.z);

	ImGui::Separator();

	bool changed = false;

	if (ImGui::DragFloat3("Translation", &camera_.translation_.x, 0.1f, -1000.0f, 1000.0f, "%.2f")) {
		changed = true;
	}

	if (changed) {
		camera_.UpdateMatrix();
		camera_.TransferMatrix();
	}

	ImGui::End();

	// リセットゲージ調整
	ImGui::Begin("Reset Meter");

	ImGui::Text("Position");
	ImGui::DragFloat("Meter X", &resetMeterPosition_.x, 1.0f, 0.0f, 1280.0f, "%.1f");

	ImGui::DragFloat("Meter Y", &resetMeterPosition_.y, 1.0f, 0.0f, 720.0f, "%.1f");

	ImGui::Separator();
	ImGui::Text("Size");

	ImGui::DragFloat("Meter Width", &resetMeterSize_.x, 1.0f, 1.0f, 1280.0f, "%.1f");

	ImGui::DragFloat("Meter Height", &resetMeterSize_.y, 1.0f, 1.0f, 720.0f, "%.1f");

	if (resetMeterSprite_) {
		resetMeterSprite_->SetPosition(resetMeterPosition_);

		// 長押し中は進行率に合わせた幅を反映する
		float rate = resetHoldTime_ / resetRequiredTime_;
		if (rate < 0.0f)
			rate = 0.0f;
		if (rate > 1.0f)
			rate = 1.0f;

		resetMeterSprite_->SetSize({resetMeterSize_.x * rate, resetMeterSize_.y});
	}

	ImGui::End();

	// パーセント表示調整
	ImGui::Begin("Reset Percentage");

	// 数字全体の中心位置
	ImGui::Text("Digit Position");

	ImGui::DragFloat("Digit Center X", &resetPercentPosition_.x, 1.0f, 0.0f, 1280.0f, "%.1f");

	ImGui::DragFloat("Digit Center Y", &resetPercentPosition_.y, 1.0f, 0.0f, 720.0f, "%.1f");

	ImGui::Separator();
	ImGui::Text("Digit Size");

	ImGui::DragFloat("Digit Width", &resetPercentDigitSize_.x, 0.5f, 1.0f, 200.0f, "%.1f");

	ImGui::DragFloat("Digit Height", &resetPercentDigitSize_.y, 0.5f, 1.0f, 200.0f, "%.1f");

	ImGui::DragFloat("Digit Spacing", &resetPercentSpacing_, 0.5f, 0.0f, 100.0f, "%.1f");

	ImGui::Separator();
	ImGui::Text("Percent Symbol Offset");

	ImGui::DragFloat("Percent Offset X", &resetPercentSymbolOffset_.x, 0.5f, -200.0f, 200.0f, "%.1f");

	ImGui::DragFloat("Percent Offset Y", &resetPercentSymbolOffset_.y, 0.5f, -200.0f, 200.0f, "%.1f");

	ImGui::Separator();
	ImGui::Text("Percent Symbol Size");

	ImGui::DragFloat("Percent Width", &resetPercentSymbolSize_.x, 0.5f, 1.0f, 200.0f, "%.1f");

	ImGui::DragFloat("Percent Height", &resetPercentSymbolSize_.y, 0.5f, 1.0f, 200.0f, "%.1f");

	ImGui::End();
#endif
}