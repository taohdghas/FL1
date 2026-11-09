#include "EndScene.h"
#include "SceneManager.h" // ★追加：SceneType / SceneManager を参照するため

void EndScene::Initialize() {
	// 背景（黒の全画面スプライト）
	background_ = Sprite::Create(TextureManager::Load("white.png"), {640.0f, 360.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.5f, 0.5f});
	if (background_) {
		background_->SetSize({1280.0f, 720.0f});
	}

	// メッセージ画像
	message_ = Sprite::Create(TextureManager::Load("clear.png"), {640.0f, 300.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});

	// 戻るボタン（既存の title.png を流用）
	backButton_ = Sprite::Create(TextureManager::Load("title.png"), {640.0f, 500.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});
	if (backButton_) {
		backButtonBaseSize_ = backButton_->GetSize();
	}
}

void EndScene::Finalize() {
	delete background_;
	background_ = nullptr;

	delete message_;
	message_ = nullptr;

	delete backButton_;
	backButton_ = nullptr;
}

void EndScene::Update() {
	if (backButton_ == nullptr) {
		return;
	}

	Vector2 mousePos = Input::GetInstance()->GetMousePosition();
	Vector2 pos = backButton_->GetPosition();
	Vector2 size = backButton_->GetSize();

	float left = pos.x - size.x * 0.5f;
	float right = pos.x + size.x * 0.5f;
	float top = pos.y - size.y * 0.5f;
	float bottom = pos.y + size.y * 0.5f;

	bool isHover = mousePos.x >= left && mousePos.x <= right && mousePos.y >= top && mousePos.y <= bottom;

	float scale = isHover ? hoverScale_ : 1.0f;
	backButton_->SetSize({backButtonBaseSize_.x * scale, backButtonBaseSize_.y * scale});

	if (isHover && Input::GetInstance()->IsTriggerMouse(0)) {
		if (sceneManager_) {
			sceneManager_->ChangeScene(SceneType::Title);
		}
	}
}

void EndScene::Draw() {
	Sprite::PreDraw();

	if (background_)
		background_->Draw();
	if (message_)
		message_->Draw();
	if (backButton_)
		backButton_->Draw();

	Sprite::PostDraw();
}