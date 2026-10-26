#pragma once
#include "SceneType.h"

class Scene {
public:
	virtual ~Scene() = default;

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// ★シーン遷移要求
	virtual SceneType GetNextScene() { return SceneType::None; }
};