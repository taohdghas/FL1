#pragma once
#include "Scene.h"
#include "KamataEngine.h"

class TitleScene : public Scene {
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	// 次のシーンへ進む
	SceneType GetNextScene() override;

private:
	SceneType nextScene_ = SceneType::None;
};