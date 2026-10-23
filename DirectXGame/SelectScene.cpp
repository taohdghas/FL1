#include "SelectScene.h"
using namespace KamataEngine;

//初期化
void SelectScene::Initialize() {
	/*BG1TextureHandle_ = KamataEngine::TextureManager::Load("uvChecker.png");
	BG1Sprite = Sprite::Create(BG1TextureHandle_, { 0, 0 });*/
}

// 終了
void SelectScene::Finalize() {
	/*delete BG1Sprite;
	BG1Sprite = nullptr;*/
}

// 更新
void SelectScene::Update() {
	// ENTERキーでゲーム画面へ
	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		nextScene_ = SceneType::Game;
	}
}

SceneType SelectScene::GetNextScene() {
	return nextScene_;
}

// 描画
void SelectScene::Draw() {
	//BG1Sprite->Draw();
}