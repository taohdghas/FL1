#include "SelectScene.h"

using namespace KamataEngine;

float SelectScene::getDeltaTime() {
	using clock = std::chrono::high_resolution_clock;

	auto now = clock::now();

	std::chrono::duration<float> dt = now - prevTime_;
	prevTime_ = now;

	return dt.count();
}

//初期化
void SelectScene::Initialize() {
	prevTime_ = std::chrono::high_resolution_clock::now();
	fader.fadeIn(1.0f);

	isPushTitle = false;
	isPushGame = false;
}

// 終了
void SelectScene::Finalize() {}

// 更新
void SelectScene::Update() {
	float dt = getDeltaTime();
	fader.update(dt);

	// SPACEキーでゲーム画面へ                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		isPushTitle = false;
		isPushGame = true;
		fader.fadeOut(1.0f);
	}

	// ENTERキーでタイトル画面へ
	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		isPushTitle = true;
		isPushGame = false;
		fader.fadeOut(1.0f);
	}

	// フェードアウト完了後にシーン遷移
	if (fader.isFinished() && fader.getAlpha() <= 0.0f) {
		if (isPushTitle && !isPushGame) {
			nextScene_ = SceneType::Title;
		}

		if (!isPushTitle && isPushGame) {
			nextScene_ = SceneType::Game;
		}
	}
}

SceneType SelectScene::GetNextScene() {
	return nextScene_;
}

// 描画
void SelectScene::Draw() {
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	float fade = fader.getAlpha();

	sample_ = Sprite::Create(sampleTexture_, {0.0f, 0.0f}, {1, 1, 1, fade});
	sample_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
}