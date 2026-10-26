#include "TitleScene.h"

using namespace KamataEngine;

float TitleScene::getDeltaTime() {
	using clock = std::chrono::high_resolution_clock;

	auto now = clock::now();

	std::chrono::duration<float> dt = now - prevTime_;
	prevTime_ = now;

	return dt.count();
}

// 初期化
void TitleScene::Initialize() {
	prevTime_ = std::chrono::high_resolution_clock::now();
	fader.fadeIn(1.0f);
}

// 終了
void TitleScene::Finalize() {}

// 更新
void TitleScene::Update() {
	float dt = getDeltaTime();
	fader.update(dt);

	// SPACEキーでセレクト画面へ
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		fader.fadeOut(1.0f);
	}

	// フェードアウト完了後にシーン遷移
	if (fader.isFinished() && fader.getAlpha() <= 0.0f) {
		nextScene_ = SceneType::Select;
	}
}

SceneType TitleScene::GetNextScene() {
	return nextScene_;
}

// 描画
void TitleScene::Draw() {
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	float fade = fader.getAlpha();

	title_ = Sprite::Create(titleTexture_, {0.0f, 0.0f}, {1, 1, 1, fade});
	title_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
}
