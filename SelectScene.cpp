#include "SelectScene.h"

using namespace KamataEngine;

float SelectScene::getDeltaTime() {
	using clock = std::chrono::high_resolution_clock;

	auto now = clock::now();

	std::chrono::duration<float> dt = now - prevTime_;
	prevTime_ = now;

	return dt.count();
}

// 初期化
void SelectScene::Initialize() {
	prevTime_ = std::chrono::high_resolution_clock::now();
	sample_ = Sprite::Create(sampleTexture_, {0.0f, 0.0f}, {1, 1, 1, 1});
	fader.fadeIn(1.0f);

	isPush = false;
	isPushTitle = false;
	isPushGame = false;
}

// 終了
void SelectScene::Finalize() {}

// 更新
void SelectScene::Update() {
	float dt = getDeltaTime();
	fader.update(dt);

	// フェード中なら操作不可
	if (!fader.isFinished()) {
		return;
	}

	// SPACEキーでゲーム画面へ                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
	if (!isPush && Input::GetInstance()->PushKey(DIK_SPACE)) {
		isPush = true;
		isPushGame = true;
		fader.fadeOut(1.0f);
	}

	// ENTERキーでタイトル画面へ
	if (!isPush && Input::GetInstance()->PushKey(DIK_RETURN)) {
		isPush = true;
		isPushTitle = true;
		fader.fadeOut(1.0f);
	}

	// フェードアウト完了後にシーン遷移
	if (fader.isFinished() && fader.getAlpha() <= 0.0f) {
		if (isPushTitle) {
			nextScene_ = SceneType::Title;
		}

		if (isPushGame) {
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

	sample_->SetColor({1, 1, 1, fade});
	sample_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
}