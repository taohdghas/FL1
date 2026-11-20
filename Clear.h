#pragma once
#include "Scene.h"
#include "Fader.h"
#include "KamataEngine.h"

using namespace KamataEngine;

class Clear : public Scene {
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	// 次のシーンへ進む
	SceneType GetNextScene() override;

private:
	SceneType nextScene_ = SceneType::None;
	// 遷移のフラグ
	bool isPush;
	bool isPushTitle;
	bool isPushSelect;
	bool isPushRetry;

	// フェード
	Fader fader;
	Fader starFader[3];
	std::chrono::high_resolution_clock::time_point prevTime_;
	float getDeltaTime();

	Sprite* clear_;
	uint32_t clearTexture_ = TextureManager::Load("uvChecker.png");


	// 使ったエレメント数(仮)
	int elementNum;

	// 評価4段階想定(星0、１，２，３)
	Sprite* star_[3];
	uint32_t starTexture_ = TextureManager::Load("star.png");

	// 評価の画像への演出
	enum class BounceState {
		Shrink,  // 縮小
		Expand,  // 拡大
		Settle,  // 1より少し小さめに
		Rebound, // 1に戻す
		Finish   // 終わり
	};

	BounceState bounceState_[3];

	float starWaitTimer = 0.0f;
	Vector2 starBaseSize_[3];
	float starScale_[3];
	bool isStarFade[3];
	
	bool canInput_;
	bool isStarDraw[3];
};