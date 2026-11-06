#include "ElementUI.h"
#include "ElementSystem.h"
#include <imgui.h>

// 初期化
void ElementUI::Initialize(const std::string& fireTexturePath) {
	Finalize();

	// 通常表示用の火パネル
	firePanelSprite_ = Sprite::Create(TextureManager::Load(fireTexturePath), firePanelPosition_);

	// ドラッグ中にカーソルへ追従する画像
	dragSprite_ = Sprite::Create(TextureManager::Load(fireTexturePath), firePanelPosition_);

	if (firePanelSprite_) {
		firePanelSprite_->SetAnchorPoint({0.5f, 0.5f});
		firePanelSprite_->SetSize(firePanelSize_);
	}

	if (dragSprite_) {
		dragSprite_->SetAnchorPoint({0.5f, 0.5f});
		dragSprite_->SetSize(firePanelSize_);
	}

	isDragging_ = false;
	isInitialized_ = firePanelSprite_ != nullptr && dragSprite_ != nullptr;
}
// 終了
void ElementUI::Finalize() {
	delete firePanelSprite_;
	firePanelSprite_ = nullptr;
	delete dragSprite_;
	dragSprite_ = nullptr;
	isDragging_ = false;
	isInitialized_ = false;
}
// 描画
void ElementUI::Draw() {
	if (!isInitialized_) {
		return;
	}

	// 通常のパネルは常に表示
	firePanelSprite_->Draw();

	// ドラッグ中だけカーソル追従画像を表示
	if (isDragging_) {
		dragSprite_->Draw();
	}
}
// デバック
#ifdef _DEBUG
void ElementUI::Debug( ElementSystem& elementSystem, Vector2& playerScreenPosition) {

	ImGui::Begin("Element Debug");

	// パネルの位置・サイズ調整
	if (isInitialized_) {

		ImGui::Text("Fire Panel");
		ImGui::Separator();

		if (ImGui::DragFloat2("Fire Position", &firePanelPosition_.x, 1.0f)) {

			firePanelSprite_->SetPosition(firePanelPosition_);

			if (!isDragging_) {
				dragSprite_->SetPosition(firePanelPosition_);
			}
		}

		if (ImGui::DragFloat2("Fire Size", &firePanelSize_.x, 1.0f)) {

			firePanelSprite_->SetSize(firePanelSize_);
			dragSprite_->SetSize(firePanelSize_);
		}
	}

	ImGui::Separator();

	// 現在のエレメント
	ImGui::Text("Current Element");

	switch (elementSystem.GetCurrentElement()) {

	case ElementType::None:
		ImGui::Text("NONE (Not Connected)");
		break;

	case ElementType::Fire:
		ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.1f, 1.0f), "FIRE");

		ImGui::Text("Remaining Time: %.2f sec", elementSystem.GetRemainingTime());
		break;

	default:
		ImGui::Text("Other Element");
		break;
	}

	ImGui::Separator();

	// ドラッグ状態
	ImGui::Text("Drag Status");

	if (isDragging_) {
		ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "DRAGGING");
	} else {
		ImGui::Text("NOT DRAGGING");
	}

	ImGui::Separator();

	// プレイヤーとの接続判定
	ImGui::Text("Player Connection Range");

	if (isDragging_) {
		if (isPlayerHovered_) {
			ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "IN RANGE");
		} else {
			ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "OUT OF RANGE");
		}
	} else {
		ImGui::Text("WAITING");
	}

	ImGui::Separator();

	// プレイヤーのスクリーン座標
	ImGui::Text("Player Screen Position");
	ImGui::Text("X: %.1f  Y: %.1f", playerScreenPosition.x, playerScreenPosition.y);

	// マウスの座標
	Vector2 mousePosition = Input::GetInstance()->GetMousePosition();

	ImGui::Text("Mouse Position");
	ImGui::Text("X: %.1f  Y: %.1f", mousePosition.x, mousePosition.y);

	ImGui::End();
}
#endif
// 更新
bool ElementUI::Update(const Vector2& playerScreenPosition) {
	if (!isInitialized_) {
		return false;
	}

	const Vector2 mousePosition = Input::GetInstance()->GetMousePosition();

	// ドラッグしていない場合
	if (!isDragging_) {

		isPlayerHovered_ = false;

		// 火パネルを押したらドラッグ開始
		if (Input::GetInstance()->IsTriggerMouse(0) && IsInside(mousePosition, firePanelPosition_, {firePanelSize_.x * 0.5f, firePanelSize_.y * 0.5f})) {

			isDragging_ = true;
		}
	}

	if (!isDragging_) {
		return false;
	}

	// ドラッグ画像をカーソルに追従させる
	dragSprite_->SetPosition(mousePosition);

	// プレイヤーの接続範囲内か確認
	isPlayerHovered_ = IsInside(mousePosition, playerScreenPosition, playerDropHalfSize_);

	// マウスを押している間はドラッグ継続
	if (Input::GetInstance()->IsPressMouse(0)) {
		return false;
	}

	// マウスを離したときに接続判定
	const bool connected = isPlayerHovered_;

	isDragging_ = false;
	// ドラッグ画像を元の位置に戻す
	dragSprite_->SetPosition(firePanelPosition_);

	return connected;
}
// ドラッグ中かどうか
bool ElementUI::IsInside(const Vector2& point, const Vector2& center, const Vector2& halfSize) { return std::abs(point.x - center.x) <= halfSize.x && std::abs(point.y - center.y) <= halfSize.y; }
