#include "UI.h"

#include <imgui.h>

// 初期化
void UI::Initialize() {

	// テクスチャの読み込み
	pausemenuTexture_ = TextureManager::Load("uvChecker.png");
	controlsButtonTexture_ = TextureManager::Load("control.png");
	selectButtonTexture_ = TextureManager::Load("select.png");
	titleButtonTexture_ = TextureManager::Load("title.png");
	controlGuideTexture_ = TextureManager::Load("controlguide.png");
	// ポーズメニュースプライトの生成
	pauseMenu_ = Sprite::Create(pausemenuTexture_, {640.0f, 360.0f},{1,1,1,1},{0.5f,0.5f});
	// 操作説明項目スプライトの生成
	controlsButton_ = Sprite::Create(controlsButtonTexture_, {640.0f, 190.0f},{1,1,1,1},{0.5f, 0.5f});
	controlsButtonBaseSize_ = controlsButton_->GetSize();//サイズの取得
	// セレクトスプライトの生成
	selectButton_ = Sprite::Create(selectButtonTexture_, {640.0f, 360.0f},{1,1,1,1},{0.5f,0.5f});
	selectButtonBaseSize_ = selectButton_->GetSize(); //サイズの取得
	// タイトルスプライトの生成
	titleButton_ = Sprite::Create(titleButtonTexture_, {640.0f, 530.0f},{1,1,1,1},{0.5f,0.5f});
	titleButtonBaseSize_ = titleButton_->GetSize(); //サイズの取得
	// 操作説明スプライトの生成
	controlGuide_ = Sprite::Create(controlGuideTexture_, {640.0f, 360.0f}, {1,1,1,1},{0.5f,0.5f});
}
// 終了
void UI::Finalize() {
	// スプライトの削除
	delete pauseMenu_;
	delete controlsButton_;
	delete selectButton_;
	delete titleButton_;
	delete controlGuide_;
}

// 更新
void UI::Update() {
	// 操作説明画面表示中
	if (isControlGuideActive_) {
		if (Input::GetInstance()->TriggerKey(DIK_Q)) {
			isControlGuideActive_ = false;
			pauseMenuAction_ = PauseMenuAction::None;
		}
		return;
	}

	// ポーズメニューの表示
	if (Input::GetInstance()->TriggerKey(DIK_Q)) {
		isPauseMenuActive_ = !isPauseMenuActive_;
		pauseMenuAction_ = PauseMenuAction::None;
	}
	// ポーズメニューが表示されていないときは処理を終了
	if (!isPauseMenuActive_) {
		return;
	}
	// ポーズメニューが表示とき場合の処理
	if (isPauseMenuActive_ && !isControlGuideActive_) {
		UpdateButtonScale(controlsButton_, controlsButtonBaseSize_);

		UpdateButtonScale(selectButton_, selectButtonBaseSize_);

		UpdateButtonScale(titleButton_, titleButtonBaseSize_);
	}

	// 操作説明
	if (IsSpriteClicked(controlsButton_)) {
		isControlGuideActive_ = true;
		pauseMenuAction_ = PauseMenuAction::ShowControls;
	}
	// セレクトへ
	else if (IsSpriteClicked(selectButton_)) {
		pauseMenuAction_ = PauseMenuAction::ToSelect;
	}
	// タイトルへ
	else if (IsSpriteClicked(titleButton_)) {
		pauseMenuAction_ = PauseMenuAction::ToTitle;
	}
}

// 描画
void UI::Draw() {
	if (!isPauseMenuActive_) {
		return;
	}

	// ポーズメニュー
	pauseMenu_->Draw();
	// ポーズメニューの項目
	controlsButton_->Draw();
	selectButton_->Draw();
	titleButton_->Draw();
	// 操作説明画面
	if (isControlGuideActive_) {
		controlGuide_->Draw();
		return;
	}
}
// スプライトクリックされたか
bool UI::IsSpriteClicked(Sprite* sprite) {
	if (!sprite) {
		return false;
	}
	// マウス座標を取得
	Vector2 mousePosition = Input::GetInstance()->GetMousePosition();
	// スプライトの位置とサイズ
	Vector2 position = sprite->GetPosition();
	Vector2 size = sprite->GetSize();
	// 中央基準の当たり判定
	float left = position.x - size.x * 0.5f;
	float right = position.x + size.x * 0.5f;
	float top = position.y - size.y * 0.5f;
	float bottom = position.y + size.y * 0.5f;

	bool isInside = mousePosition.x >= left && mousePosition.x <= right && mousePosition.y >= top && mousePosition.y <= bottom;

	return isInside && Input::GetInstance()->IsTriggerMouse(0);
}
// ボタンの拡大率を更新
void UI::UpdateButtonScale(Sprite* sprite, const Vector2& baseSize) {
	if (sprite == nullptr) {
		return;
	}

	float scale = IsMouseOverSprite(sprite) ? hoverScale_ : 1.0f;

	sprite->SetSize({baseSize.x * scale, baseSize.y * scale});
}
// マウスがスプライトの範囲内か判定
bool UI::IsMouseOverSprite(Sprite* sprite) {
    if (sprite == nullptr) {
        return false;
    }

    // 使用中の KamataEngine で取得できるマウス座標を使う
    Vector2 mousePosition =Input::GetInstance()->GetMousePosition();

    Vector2 position = sprite->GetPosition();
    Vector2 size = sprite->GetSize();

	float left = position.x - size.x * 0.5f;
	float right = position.x + size.x * 0.5f;
	float top = position.y - size.y * 0.5f;
	float bottom = position.y + size.y * 0.5f;

    return
        mousePosition.x >= left &&
        mousePosition.x <= right &&
        mousePosition.y >= top &&
        mousePosition.y <= bottom;
}

// デバック
void UI::Debug() {
#ifdef _DEBUG
	ImGui::Begin("UI Debug");

	// 枠の位置
	Vector2 position = pauseMenu_->GetPosition();

	if (ImGui::DragFloat2("Pause Menu Position", &position.x, 1.0f)) {
		pauseMenu_->SetPosition(position);
	}

	// 枠のサイズ
	Vector2 size = pauseMenu_->GetSize();

	if (ImGui::DragFloat2("Pause Menu Size", &size.x, 1.0f)) {
		pauseMenu_->SetSize(size);
	}

	ImGui::Separator();
	ImGui::Text("Menu Buttons");

	// 操作説明ボタンの位置
	position = controlsButton_->GetPosition();

	if (ImGui::DragFloat2("Controls Position", &position.x, 1.0f)) {
		controlsButton_->SetPosition(position);
	}

	// 操作説明ボタンのサイズ
	size = controlsButton_->GetSize();

	if (ImGui::DragFloat2("Controls Size", &size.x, 1.0f)) {
		controlsButton_->SetSize(size);
	}

	ImGui::Separator();

	// セレクトへボタンの位置
	position = selectButton_->GetPosition();

	if (ImGui::DragFloat2("Select Position", &position.x, 1.0f)) {
		selectButton_->SetPosition(position);
	}

	// セレクトへボタンのサイズ
	size = selectButton_->GetSize();

	if (ImGui::DragFloat2("Select Size", &size.x, 1.0f)) {
		selectButton_->SetSize(size);
	}

	ImGui::Separator();

	// タイトルへボタンの位置
	position = titleButton_->GetPosition();

	if (ImGui::DragFloat2("Title Position", &position.x, 1.0f)) {
		titleButton_->SetPosition(position);
	}

	// タイトルへボタンのサイズ
	size = titleButton_->GetSize();

	if (ImGui::DragFloat2("Title Size", &size.x, 1.0f)) {
		titleButton_->SetSize(size);
	}

	ImGui::End();
#endif
}