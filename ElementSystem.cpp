#include "ElementSystem.h"

//更新
void ElementSystem::Update(float deltaTime) { 
	if (!IsActive()) {
		return;
	}
	// 残り時間を減らす
	remainingTime_ -= deltaTime;
	//効果時間終了したら通常状態
	if (remainingTime_ <= 0.0f) {
		Clear();
	}
}

// 解除
void ElementSystem::Clear() { 
	currentElement_ = ElementType::None; 
	remainingTime_ = 0.0f;
}
// エレメント接続
bool ElementSystem::Connect(ElementType type, float duration) { 
	if (type == ElementType::None || duration <= 0.0f) {
		return false;
	}
	//接続したエレメントに切り替える
	currentElement_ = type;
	remainingTime_ = duration;
	return true;
}
//
ElementType ElementSystem::GetCurrentElement() { return currentElement_; }
// 残り時間を取得
float ElementSystem::GetRemainingTime() { return remainingTime_; }
// エレメントが有効かどうかを取得
bool ElementSystem::IsActive() { return currentElement_ != ElementType::None; }
