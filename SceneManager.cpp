#include "SceneManager.h"
#include "EndScene.h"
#include "GameScene.h"
#include "SelectScene.h"
#include "TitleScene.h"

// 初期化
void SceneManager::Initialize() {
	currentScene_ = CreateScene(SceneType::Game);

	if (currentScene_) {
		currentScene_->SetSceneManager(this);
		currentScene_->Initialize();
	}
}
// 終了
void SceneManager::Finalize() {
	if (currentScene_) {
		currentScene_->Finalize();
		currentScene_.reset();
	}
}
// 更新
void SceneManager::Update() {
	if (currentScene_) {
		currentScene_->Update();
	}
	// Updateが終わってからシーンを切り替える
	if (isSceneChangeRequested_) {
		isSceneChangeRequested_ = false;

		if (currentScene_) {
			currentScene_->Finalize();
			currentScene_.reset();
		}

		currentScene_ = CreateScene(nextScene_);

		if (currentScene_) {
			currentScene_->SetSceneManager(this);
			currentScene_->Initialize();
		}
	}
}
// 描画
void SceneManager::Draw() {
	if (currentScene_) {
		currentScene_->Draw();
	}
}
// シーン変更
void SceneManager::ChangeScene(SceneType sceneType) {
	nextScene_ = sceneType;
	isSceneChangeRequested_ = true;
}

// シーン生成
std::unique_ptr<Scene> SceneManager::CreateScene(SceneType sceneType) {
	switch (sceneType) {
	case SceneType::Title:
		return std::make_unique<TitleScene>();

	case SceneType::Select:
		return std::make_unique<SelectScene>();

	case SceneType::Game:
		return std::make_unique<GameScene>();

	case SceneType::End:
		return std::make_unique<EndScene>();
	}

	return nullptr;
}