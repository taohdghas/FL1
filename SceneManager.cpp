#include "SceneManager.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "GameScene.h"

//初期化
void SceneManager::Initialize() {
	currentScene_ = CreateScene(SceneType::Title); 
	 
	if (currentScene_) {
		currentScene_->Initialize();
	}
}
//終了
void SceneManager::Finalize() {
	if (currentScene_) {
		currentScene_->Finalize();
		currentScene_.reset();
	}
}
//更新
void SceneManager::Update() { if (currentScene_) { currentScene_->Update(); } }
// 描画
void SceneManager::Draw() { if (currentScene_) { currentScene_->Draw(); } }

// シーン生成0
std::unique_ptr<Scene> SceneManager::CreateScene(SceneType sceneType) {
	switch (sceneType) {
	case SceneType::Title:
		return std::make_unique<TitleScene>();

	case SceneType::Select:
		return std::make_unique<SelectScene>();

	case SceneType::Game:
		return std::make_unique<GameScene>();
	}

	return nullptr;
}
