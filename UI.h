#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

class UI {
public:
	//ポーズメニューの項目
	enum class PauseMenuAction {
		None,
		ShowControls,
		ToSelect,
		ToTitle
	};

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
	// 選択結果リセット
	void ResetPauseMenuAction() { pauseMenuAction_ = PauseMenuAction::None; }
    //選択された項目を取得
	PauseMenuAction GetPauseMenuAction() { return pauseMenuAction_; }
	

private:
	// スプライトテクスチャ
	uint32_t pausemenuTexture_;
	uint32_t controlsButtonTexture_;
	uint32_t selectButtonTexture_;
	uint32_t titleButtonTexture_;
	// ポーズメニュースプライト
	Sprite* pauseMenu_;
	//操作説明スプライト
	Sprite* controlsButton_;
	//セレクトスプライト
	Sprite* selectButton_;
	//タイトルスプライト
	Sprite* titleButton_;
	//選択された項目
	PauseMenuAction pauseMenuAction_ = PauseMenuAction::None;

	// ポーズメニュー描画フラグ
	bool isPauseMenuActive_ = false;
};