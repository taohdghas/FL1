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
	void Initialize();//初期化
	void Finalize();   //終了
	void Update();    //更新
	void Draw();     //描画
	// シーンを変更
	void ChangeScene(SceneType sceneType);

private:
	// シーンを生成する関数
	std::unique_ptr<Scene> CreateScene(SceneType sceneType);

private:
	std::unique_ptr<Scene> currentScene_;
	SceneType nextScene_ = SceneType::Game;
	bool isSceneChangeRequested_ = false;
};
