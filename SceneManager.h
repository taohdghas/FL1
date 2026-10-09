#pragma once
#include "Scene.h"
#include <memory>

enum class SceneType {
	Title,
	Select,
	Game
};

class SceneManager {
public:
	void Initialize();
	void Finalize();
	void Update();
	void Draw();

	void ChangeScene(SceneType sceneType);

private:
	// シーンを生成する関数
	std::unique_ptr<Scene> CreateScene(SceneType sceneType);

private:
	std::unique_ptr<Scene> currentScene_;
};
