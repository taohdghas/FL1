#include "UI.h"

#include <imgui.h>

// 初期化
void UI::Initialize() {

	// テクスチャの読み込み
	pausemenuTexture_ = TextureManager::Load("uvChecker.png");
	// ポーズメニュースプライトの生成
	pauseMenu_ = Sprite::Create(pausemenuTexture_, {380.0f, 100.0f});
}
// 終了
void UI::Finalize() {
	// スプライトの削除
	delete pauseMenu_;
}

// 更新
void UI::Update() {
	// ポーズメニューの表示
	if (Input::GetInstance()->TriggerKey(DIK_Q)) {
		isPauseMenuActive_ = !isPauseMenuActive_;
	}
}

// 描画
void UI::Draw() {
	// ポーズメニューの描画
	if (isPauseMenuActive_) {
		pauseMenu_->Draw();
	}
}
// デバック
void UI::Debug() {
#ifdef _DEBUG
	ImGui::Begin("UI Debug");

	// ポーズメニューの位置
	Vector2 position = pauseMenu_->GetPosition();

	if (ImGui::DragFloat2("Pause Menu Position", &position.x, 1.0f)) {
		pauseMenu_->SetPosition(position);
	}
	// ポーズメニューのサイズ
	Vector2 size = pauseMenu_->GetSize();

	if (ImGui::DragFloat2("Pause Menu Size", &size.x, 1.0f)) {
		pauseMenu_->SetSize(size);
	}

	ImGui::End();
#endif
}