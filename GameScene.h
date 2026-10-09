#pragma once
#include "Scene.h"

class GameScene : public Scene {
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};