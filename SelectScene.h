#pragma once
#include "Scene.h"
#include "Fader.h"
#include "KamataEngine.h"

using namespace KamataEngine;

class SelectScene : public Scene {
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	
	// 次のシーンへ進む
	SceneType GetNextScene() override;

private:
	SceneType nextScene_ = SceneType::None;
	bool isPushTitle;
	bool isPushGame;

	Fader fader;
	std::chrono::high_resolution_clock::time_point prevTime_;
	float getDeltaTime();

	Sprite* sample_;
	uint32_t sampleTexture_ = TextureManager::Load("sample.png");
};