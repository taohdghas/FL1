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
	bool isPush;
	bool isPushTitle;
	bool isPushSelect;
	bool isPushRetry;

	Fader fader;
	std::chrono::high_resolution_clock::time_point prevTime_;
	float getDeltaTime();

	Sprite* clear_;
	uint32_t clearTexture_ = TextureManager::Load("uvChecker.png");
};