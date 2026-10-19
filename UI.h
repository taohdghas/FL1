#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

class UI {
public:
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

private:
	// ポーズメニューテクスチャ
	uint32_t pausemenuTexture_;
	// ポーズメニュースプライト
	Sprite* pauseMenu_;

	// ポーズメニュー描画フラグ
	bool isPauseMenuActive_ = false;
};