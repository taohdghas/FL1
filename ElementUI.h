#pragma once
#include <KamataEngine.h>
#include <string>
#include <vector>
#include "ElementType.h"

using namespace KamataEngine;

class ElementSystem;
class ElementUI {
public:
	// 初期化
	void Initialize();
	// 終了
	void Finalize();
	// 描画
	void Draw();
	//デバック
	void Debug(ElementSystem& elementSystem,Vector2& playerScreenPosition);
	// 更新
	ElementType Update(const Vector2& playerScreenPosition);
	//パネルを追加
	bool AddPanel(ElementType type, const std::string& texturePath, const Vector2& position, const Vector2& size);

private:
	// ドラッグ中かどうか
	bool IsInside(const Vector2& point, const Vector2& center, const Vector2& halfSize);
	//エレメント名を取得
	const char* GetElementName(ElementType type);
	//パネル情報
	struct ElementPanel {
		ElementType type = ElementType::None;
		Sprite* panelSprite = nullptr;
		Sprite* dragSprite = nullptr;
		Vector2 position{};
		Vector2 size{};
	};

private:
	//エレメントパネル一覧
	std::vector<ElementPanel> panels_;
	//現在ドラッグ中のパネル
	int draggingPanelIndex_ = -1;
	// プレイヤーへの接続は安定範囲
	Vector2 playerDropHalfSize_ = {60.0f, 60.0f};
	bool isDragging_ = false;    // ドラッグ中かどうか
	bool isInitialized_ = false; // 初期化済みかどうか
	bool isPlayerHovered_ = false;
};