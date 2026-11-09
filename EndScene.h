#pragma once
#include "Scene.h"
#include <KamataEngine.h>

using namespace KamataEngine;

// 全ステージクリア後のエンディング画面
class EndScene : public Scene {
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

private:
	Sprite* background_ = nullptr; // 背景（暗幕）
	Sprite* message_ = nullptr;    // 「Congratulations!」等の文字画像
	Sprite* backButton_ = nullptr; // タイトルへ戻るボタン

	Vector2 backButtonBaseSize_{};
	const float hoverScale_ = 1.1f;
};