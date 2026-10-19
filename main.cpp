#include <Windows.h>
#include "KamataEngine.h"
#include "SceneManager.h"

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	//エンジン初期化
	KamataEngine::Initialize(L"4345");

	// ImGuiManagerの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

	// DirectXCommonインスタンス取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//SceneManager
	SceneManager sceneManager;
	sceneManager.Initialize();

	//メインループ
	while (true) {
		//エンジン更新
		if (KamataEngine::Update()) {
			break;
		}
		// ImGui受付開始
		imguiManager->Begin();
		// シーン更新
		sceneManager.Update();
		// ImGui受付終了
		imguiManager->End();

		//描画開始
		dxCommon->PreDraw();
		//シーン描画
		sceneManager.Draw();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// シーン終了
	sceneManager.Finalize();
	// エンジン終了
	KamataEngine::Finalize();

	return 0;
}
