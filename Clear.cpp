#include "Clear.h"

using namespace KamataEngine;

float Clear::getDeltaTime() {
	using clock = std::chrono::high_resolution_clock;

	auto now = clock::now();

	std::chrono::duration<float> dt = now - prevTime_;
	prevTime_ = now;

	return dt.count();
}

// 初期化
void Clear::Initialize() {
	prevTime_ = std::chrono::high_resolution_clock::now();
	fader.fadeIn(1.0f);

	isPush = false;
	isPushTitle = false;
	isPushSelect = false;
	isPushRetry = false;
}

// 終了
void Clear::Finalize() {}

// 更新
void Clear::Update() {
	float dt = getDeltaTime();
	fader.update(dt);

	// Aキーでタイトル画面へ
	if (!isPush && Input::GetInstance()->PushKey(DIK_A)) {
		isPush = true;
		isPushTitle = true;
		fader.fadeOut(1.0f);
	}

	// Sキーでセレクト画面へ
	if (!isPush && Input::GetInstance()->PushKey(DIK_S)) {
		isPush = true;
		isPushSelect = true;
		fader.fadeOut(1.0f);
	}

	// Dキーでゲーム画面へ
	if (!isPush && Input::GetInstance()->PushKey(DIK_D)) {
		isPush = true;
		isPushRetry = true;
		fader.fadeOut(1.0f);
	}

	// フェードアウト完了後にシーン遷移
	if (fader.isFinished() && fader.getAlpha() <= 0.0f) {
		if (isPushTitle) {
			nextScene_ = SceneType::Title;
		}

		if (isPushSelect) {
			nextScene_ = SceneType::Select;
		}

		if (isPushRetry) {
			nextScene_ = SceneType::Game;
		}
	}
}

SceneType Clear::GetNextScene() { return nextScene_; }

// 描画
void Clear::Draw() {
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	float fade = fader.getAlpha();

	clear_ = Sprite::Create(clearTexture_, {0.0f, 0.0f}, {1, 1, 1, fade});
	clear_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
}