#pragma once
#include <KamataEngine.h>
#include <string>
using namespace KamataEngine;

class ElementSystem;
class ElementUI {
public:
	// 初期化
	void Initialize(const std::string& fireTexturePath);
	// 終了
	void Finalize();
	// 描画
	void Draw();
	//デバック
	void Debug(ElementSystem& elementSystem,Vector2& playerScreenPosition);
	// 更新
	bool Update(const Vector2& playerScreenPosition);

private:
	// ドラッグ中かどうか
	bool IsInside(const Vector2& point, const Vector2& center, const Vector2& halfSize);

private:
	// 火エレメントパネルのスプライト
	Sprite* firePanelSprite_ = nullptr;
	// ドラッグスプライト
	Sprite* dragSprite_ = nullptr;
	// パネルの表示位置とサイズ
	Vector2 firePanelPosition_ = {200.0f, 660.0f};
	Vector2 firePanelSize_ = {100.0f, 100.0f};
	// プレイヤーへの接続は安定範囲
	Vector2 playerDropHalfSize_ = {60.0f, 60.0f};
	bool isDragging_ = false;    // ドラッグ中かどうか
	bool isInitialized_ = false; // 初期化済みかどうか
	bool isPlayerHovered_ = false;
};