#include "Fail.h"

using namespace KamataEngine;

float Fail::getDeltaTime() {
	using clock = std::chrono::high_resolution_clock;

	auto now = clock::now();

	std::chrono::duration<float> dt = now - prevTime_;
	prevTime_ = now;

	return dt.count();
}

// 初期化
void Fail::Initialize() {
	prevTime_ = std::chrono::high_resolution_clock::now();
	failed_ = Sprite::Create(failedTexture_, {0.0f, 0.0f}, {1, 1, 1, 1});
	fader.setAlpha(1.0f);

	isPush = false;
	isPushTitle = false;
	isPushSelect = false;
	isPushRetry = false;
}

// 終了
void Fail::Finalize() {}

// 更新
void Fail::Update() {
	float dt = getDeltaTime();
	fader.update(dt);

	// フェード中なら操作不可
	if (!fader.isFinished()) {
		return;
	} 

	// Aキーでタイトル画面へ
	if (!isPush && Input::GetInstance()->PushKey(DIK_A)) {
		isPush = true;
		isPushTitle = true;
		fader.fadeOut(1.5f);
	}

	// Sキーでセレクト画面へ
	if (!isPush && Input::GetInstance()->PushKey(DIK_S)) {
		isPush = true;
		isPushSelect = true;
		fader.fadeOut(1.5f);
	}

	// Dキーでリトライ
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

SceneType Fail::GetNextScene() { return nextScene_; }

// 描画
void Fail::Draw() {
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	float fade = fader.getAlpha();

	failed_->SetColor({1, 1, 1, fade});
	failed_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
}