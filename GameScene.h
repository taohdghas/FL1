#pragma once
#include "MapChip.h"
#include "Scene.h"
#include <KamataEngine.h>

using namespace KamataEngine;

class GameScene : public Scene {
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

private:
	Camera camera_;
	MapChip mapChip_;
};