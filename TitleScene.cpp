#include "TitleScene.h"
using namespace KamataEngine;

// 初期化
void TitleScene::Initialize() {
	
}
// 終了
void TitleScene::Finalize() {

}

// 更新
void TitleScene::Update() {
	// SPACEキーでセレクト画面へ
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		nextScene_ = SceneType::Select;
	}
}

SceneType TitleScene::GetNextScene() {
	return nextScene_;
}

// 描画
void TitleScene::Draw() {}
