#pragma once
#include "Scene.h"

class SelectScene : public Scene {
public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};