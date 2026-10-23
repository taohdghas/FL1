#pragma once
#include "Scene.h"
#include "KamataEngine.h"

class SelectScene : public Scene {
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	
	// 次のシーンへ進む
	SceneType GetNextScene() override;

private:
	/*uint32_t BG1TextureHandle_ = 0;
	KamataEngine::Sprite* BG1Sprite = nullptr;*/

	SceneType nextScene_ = SceneType::None;
};