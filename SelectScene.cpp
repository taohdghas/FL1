#include "SelectScene.h"
using namespace KamataEngine;

//初期化
void SelectScene::Initialize() {}

// 終了
void SelectScene::Finalize() {}

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
void SelectScene::Draw() {}