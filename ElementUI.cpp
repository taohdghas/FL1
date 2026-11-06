#include "ElementUI.h"
#include "ElementSystem.h"
#include <cmath>
#include <imgui.h>

// 初期化
void ElementUI::Initialize() {
	Finalize();

	isInitialized_ = !panels_.empty();
}
// 終了
void ElementUI::Finalize() {
	for (ElementPanel& panel : panels_) {
		delete panel.panelSprite;
		delete panel.dragSprite;

		panel.panelSprite = nullptr;
		panel.dragSprite = nullptr;
	}

	panels_.clear();

	draggingPanelIndex_ = -1;
	isDragging_ = false;
	isPlayerHovered_ = false;
	isInitialized_ = false;
}
// 描画
void ElementUI::Draw() {
	if (!isInitialized_) {
		return;
	}

	for (int i = 0; i < static_cast<int>(panels_.size()); ++i) {
		ElementPanel& panel = panels_[i];

		// 通常のパネル
		panel.panelSprite->Draw();

		// ドラッグ中のパネル
		if (isDragging_ && draggingPanelIndex_ == i) {
			panel.dragSprite->Draw();
		}
	}
}
#pragma region デバック
#ifdef _DEBUG
void ElementUI::Debug( ElementSystem& elementSystem, Vector2& playerScreenPosition) {
	ImGui::Begin("Element Debug");

	// 現在のエレメント
	ImGui::Text("Current Element");
	ImGui::Text("%s", GetElementName(elementSystem.GetCurrentElement()));

	if (elementSystem.IsActive()) {
		ImGui::Text("Remaining Time: %.2f sec", elementSystem.GetRemainingTime());
	}

	ImGui::Separator();

	// 登録パネルの調整
	ImGui::Text("Panel Settings");

	for (int i = 0; i < static_cast<int>(panels_.size()); ++i) {
		ElementPanel& panel = panels_[i];

		ImGui::PushID(i);

		ImGui::Text("%s", GetElementName(panel.type));

		if (ImGui::DragFloat2("Position", &panel.position.x, 1.0f)) {
			panel.panelSprite->SetPosition(panel.position);

			if (!isDragging_ || draggingPanelIndex_ != i) {
				panel.dragSprite->SetPosition(panel.position);
			}
		}

		if (ImGui::DragFloat2("Size", &panel.size.x, 1.0f, 1.0f, 300.0f)) {
			panel.panelSprite->SetSize(panel.size);
			panel.dragSprite->SetSize(panel.size);
		}

		ImGui::Separator();

		ImGui::PopID();
	}

	// ドラッグ状態
	ImGui::Text("Drag Status");

	ImGui::Text("%s", isDragging_ ? "DRAGGING" : "NOT DRAGGING");

	// 接続判定
	ImGui::Text("Player Connection Range");

	if (isDragging_) {
		ImGui::Text("%s", isPlayerHovered_ ? "IN RANGE" : "OUT OF RANGE");
	} else {
		ImGui::Text("WAITING");
	}

	ImGui::Separator();

	// 接続判定範囲の調整
	ImGui::Text("Connection Range");

	ImGui::DragFloat2("Drop Half Size", &playerDropHalfSize_.x, 1.0f, 1.0f, 300.0f);

	// プレイヤー座標
	ImGui::Separator();

	ImGui::Text("Player Screen: X %.1f / Y %.1f", playerScreenPosition.x, playerScreenPosition.y);

	// マウス座標
	Vector2 mousePosition = Input::GetInstance()->GetMousePosition();

	ImGui::Text("Mouse: X %.1f / Y %.1f", mousePosition.x, mousePosition.y);

	ImGui::End();

}
#endif
#pragma endregion
// 更新
ElementType ElementUI::Update(const Vector2& playerScreenPosition) {
	if (!isInitialized_) {
		return ElementType::None;
	}

	const Vector2 mousePosition = Input::GetInstance()->GetMousePosition();

	// ドラッグ開始
	if (!isDragging_ && Input::GetInstance()->IsTriggerMouse(0)) {

		for (int i = 0; i < static_cast<int>(panels_.size()); ++i) {
			ElementPanel& panel = panels_[i];

			if (IsInside(mousePosition, panel.position, {panel.size.x * 0.5f, panel.size.y * 0.5f})) {

				isDragging_ = true;
				draggingPanelIndex_ = i;

				break;
			}
		}
	}

	if (!isDragging_) {
		isPlayerHovered_ = false;
		return ElementType::None;
	}

	ElementPanel& panel = panels_[draggingPanelIndex_];

	// ドラッグ中の画像をマウスに追従
	panel.dragSprite->SetPosition(mousePosition);

	// 接続範囲の確認
	isPlayerHovered_ = IsInside(mousePosition, playerScreenPosition, playerDropHalfSize_);

	// マウスを押している間は継続
	if (Input::GetInstance()->IsPressMouse(0)) {
		return ElementType::None;
	}

	// マウスを離したとき
	ElementType result = ElementType::None;

	if (isPlayerHovered_) {
		result = panel.type;
	}

	// ドラッグ状態を解除
	isDragging_ = false;
	isPlayerHovered_ = false;

	// 元の位置に戻す
	panel.dragSprite->SetPosition(panel.position);

	draggingPanelIndex_ = -1;

	return result;
}

// パネルを追加
bool ElementUI::AddPanel(ElementType type, const std::string& texturePath, const Vector2& position, const Vector2& size) {
	if (type == ElementType::None) {
		return false;
	}

	// 同じエレメントが登録済みなら追加しない
	for (const ElementPanel& panel : panels_) {
		if (panel.type == type) {
			return false;
		}
	}

	ElementPanel panel;

	panel.type = type;
	panel.position = position;
	panel.size = size;

	// テクスチャを読み込んでスプライトを作成
	panel.panelSprite = Sprite::Create(TextureManager::Load(texturePath), position);

	panel.dragSprite = Sprite::Create(TextureManager::Load(texturePath), position);

	if (!panel.panelSprite || !panel.dragSprite) {
		delete panel.panelSprite;
		delete panel.dragSprite;
		return false;
	}

	panel.panelSprite->SetAnchorPoint({0.5f, 0.5f});
	panel.panelSprite->SetSize(size);

	panel.dragSprite->SetAnchorPoint({0.5f, 0.5f});
	panel.dragSprite->SetSize(size);

	panels_.push_back(panel);

	isInitialized_ = true;

	return true;
}
// ドラッグ中かどうか
bool ElementUI::IsInside(const Vector2& point, const Vector2& center, const Vector2& halfSize) { return std::abs(point.x - center.x) <= halfSize.x && std::abs(point.y - center.y) <= halfSize.y; }
// エレメント名を取得
const char* ElementUI::GetElementName(ElementType type) {
	switch (type) {
	case ElementType::None:
		return "None";
	case ElementType::Fire:
		return "Fire";
	case ElementType::Water:
		return "Water";
	case ElementType::Stone:
		return "Stone";
	case ElementType::Gas:
		return "Gas";
	case ElementType::Electric:
		return "Electric";
	default:
		return "Unknown";
	}
}