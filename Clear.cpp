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
	clear_ = Sprite::Create(clearTexture_, {0.0f, 0.0f}, {1, 1, 1, 1});

	// フェードインはなし
	fader.setAlpha(1.0f);

	isPush = false;
	isPushTitle = false;
	isPushSelect = false;
	isPushRetry = false;


	// 画像横150px想定。大きさにより調整必要
	for (int i = 0; i < 3; i++) {
		star_[i] = Sprite::Create(starTexture_, {0.0f, 0.0f}, {1, 1, 1, 1});


		starBaseSize_[i] = {150.0f, 150.0f};
		star_[i]->SetSize(starBaseSize_[i]);
		starScale_[i] = 2.5f;

		starFader[i].setAlpha(0.0f);
		isStarFade[i] = false;
		bounceState_[i] = BounceState::Finish;

		isStarDraw[i] = false;
	}


	star_[0]->SetAnchorPoint({1.0f, 1.0f});
	star_[1]->SetAnchorPoint({0.5f, 1.0f});
	star_[2]->SetAnchorPoint({0.0f, 1.0f});
	star_[0]->SetPosition({500.0f, 400.0f});
	star_[1]->SetPosition({650.0f, 400.0f});
	star_[2]->SetPosition({800.0f, 400.0f});

	elementNum = 0;
	starWaitTimer = 0.0f;
	canInput_ = false;
}

// 終了
void Clear::Finalize() {}

// 更新
void Clear::Update() {
	float dt = getDeltaTime();
	fader.update(dt);
	starWaitTimer += dt;

	
	// 使ったエレメント数による評価
	if (elementNum >= 1 && elementNum <= 2) {
		isStarDraw[0] = true;
	}

	else if (elementNum >= 3 && elementNum <= 4) {
		isStarDraw[0] = true;
		isStarDraw[1] = true;
	}

	else if (elementNum == 5) {
		isStarDraw[0] = true;
		isStarDraw[1] = true;
		isStarDraw[2] = true;
	}

	// ズームアウト＋バウンド
	for (int i = 0; i < 3; i++) {
		starFader[i].update(dt);

		if (!isStarFade[i] || isPush) {
			continue;
		}

		switch (bounceState_[i]) {

		case BounceState::Shrink:

			starScale_[i] -= 6.0f * dt;

			if (starScale_[i] <= 0.5f) {

				starScale_[i] = 0.5f;
				bounceState_[i] = BounceState::Expand;
			}

			break;

		case BounceState::Expand:

			starScale_[i] += 5.0f * dt;

			if (starScale_[i] >= 1.3f) {

				starScale_[i] = 1.3f;
				bounceState_[i] = BounceState::Settle;
			}

			break;

		case BounceState::Settle:

			starScale_[i] -= 3.0f * dt;

			if (starScale_[i] <= 0.8f) {

				starScale_[i] = 0.8f;
				bounceState_[i] = BounceState::Rebound;
			}

			break;

		case BounceState::Rebound:

			starScale_[i] += 1.0f * dt;

			if (starScale_[i] >= 1.0f) {
				starScale_[i] = 1.0f;
				bounceState_[i] = BounceState::Finish;
			}

			break;

		case BounceState::Finish:

			break;
		}
	}

	// フェード
	for (int i = 0; i < 3; i++) {
		if (isStarDraw[i] && !isStarFade[i] && ((i == 0 && starWaitTimer >= 0.6f) || (i > 0 && starFader[i - 1].isFinished() && starFader[i - 1].getAlpha() >= 1.0f))) {
			starScale_[i] = 2.5f;
			bounceState_[i] = BounceState::Shrink;

			starFader[i].fadeIn(0.5f);
			isStarFade[i] = true;
		}
	}


	// 演出終わったかの判定
	canInput_ = true;

	for (int i = 0; i < 3; i++) {

		if (isStarDraw[i]) {

			if (!starFader[i].isFinished() || starFader[i].getAlpha() < 1.0f) {

				canInput_ = false;
				break;
			}
		}
	}

	// フェード中なら操作不可
	if (!canInput_) {
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

SceneType Clear::GetNextScene() { return nextScene_; }

// 描画
void Clear::Draw() {
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	float fade = fader.getAlpha();

	clear_->SetColor({1, 1, 1, fade});
	clear_->Draw();


	// elementNum == 0は星の描画なし
	for (int i = 0; i < 3; i++) {
		if (isStarDraw[i]) {
			float starFade;

			if (isPush) {
				starFade = fader.getAlpha();
			} else {
				starFade = starFader[i].getAlpha();
			}

			star_[i]->SetSize({starBaseSize_[i].x * starScale_[i], starBaseSize_[i].y * starScale_[i]});
			star_[i]->SetColor({1, 1, 1, starFade});
			star_[i]->Draw();
		}
	}

	// スプライト描画後処理
	Sprite::PostDraw();
}