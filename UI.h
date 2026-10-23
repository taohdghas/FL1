#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

class UI {
public:
	// ポーズメニューの項目
	enum class PauseMenuAction { None, ShowControls, ToSelect, ToTitle };

	// 初期化
	void Initialize();
	// 終了
	void Finalize();
	// 更新
	void Update();
	// 描画
	void Draw();
	// デバック
	void Debug();
	// スプライトがクリックされたか
	bool IsSpriteClicked(Sprite* sprite);

	// 選択結果リセット
	void ResetPauseMenuAction() { pauseMenuAction_ = PauseMenuAction::None; }
	// ポーズメニューが表示されているか
	bool IsPauseMenuActive() { return isPauseMenuActive_; }
	// 選択された項目を取得
	PauseMenuAction GetPauseMenuAction() { return pauseMenuAction_; }

private:
	// ボタンの拡大率を更新
	void UpdateButtonScale(Sprite* sprite, const Vector2& baseSize);
	// マウスがスプライトの範囲内か判定
	bool IsMouseOverSprite(Sprite* sprite);
private:
	// スプライトテクスチャ
	uint32_t pausemenuTexture_;
	uint32_t controlsButtonTexture_;
	uint32_t selectButtonTexture_;
	uint32_t titleButtonTexture_;
	uint32_t controlGuideTexture_;
	// ポーズメニュースプライト
	Sprite* pauseMenu_;
	// 操作説明項目スプライト
	Sprite* controlsButton_;
	// セレクトスプライト
	Sprite* selectButton_;
	// タイトルスプライト
	Sprite* titleButton_;
	// 操作説明スプライト
	Sprite* controlGuide_;
	// 選択された項目
	PauseMenuAction pauseMenuAction_ = PauseMenuAction::None;
	// 各ボタンの通常サイズ
	Vector2 controlsButtonBaseSize_;
	Vector2 selectButtonBaseSize_;
	Vector2 titleButtonBaseSize_;
	// 拡大率
	const float hoverScale_ = 1.1f;
	// ポーズメニュー描画フラグ
	bool isPauseMenuActive_ = false;
	// 操作説明描画フラグ
	bool isControlGuideActive_ = false;
};