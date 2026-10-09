#include <Windows.h>
#include "KamataEngine.h"

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	
	//エンジン初期化
	KamataEngine::Initialize(L"4345");

	// DirectXCommonインスタンス取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//メインループ
	while (true) {
		//エンジン更新
		if (KamataEngine::Update()) {
			break;
		}

		//描画開始
		dxCommon->PreDraw();

		// 描画終了
		dxCommon->PostDraw();
	}

	//エンジン終了
	KamataEngine::Finalize();

	return 0;
}
