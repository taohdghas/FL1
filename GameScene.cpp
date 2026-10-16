#include "GameScene.h"

// 初期化
void GameScene::Initialize() {
	
	// カメラの位置を設定
	camera_.translation_ = {0.0f, 0.0f, -40.0f};
	// カメラの初期化
	camera_.Initialize();

	// CSVを読み込んでマップを生成
	if (!mapChip_.Initialize("Resources/mapdata/stage1.csv", 2.0f)) {
		OutputDebugStringA("GameScene: マップの読み込みに失敗しました。\n");
	}
}

// 終了
void GameScene::Finalize() {
	// マップ内のブロックを解放
	mapChip_.Finalize();
}

// 更新
void GameScene::Update() { 
	// 移動

	if (Input::GetInstance()->PushKey(DIK_W)) {

		camera_.translation_.y += 0.1f;
	}

	if (Input::GetInstance()->PushKey(DIK_S)) {

		camera_.translation_.y -= 0.1f;
	}

	if (Input::GetInstance()->PushKey(DIK_A)) {

		camera_.translation_.x -= 0.1f;
	}

	if (Input::GetInstance()->PushKey(DIK_D)) {

		camera_.translation_.x += 0.1f;
	}

	// 行列を更新
	camera_.UpdateMatrix();
	// 行列を転送
	camera_.TransferMatrix();


	mapChip_.Update();
}

// 描画
void GameScene::Draw() {
	// モデル描画開始
	Model::PreDraw();

	// マップを描画
	mapChip_.Draw(camera_);

	// モデル描画終了
	Model::PostDraw();
}