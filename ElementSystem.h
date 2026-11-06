#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

enum class ElementType {
	None,
	Fire,
	Water,
	Stone,
	Gas,
	Electric,
};

class ElementSystem {
public:
    //更新
	void Update(float deltaTime);
	//解除
	void Clear();
	//エレメント接続
	bool Connect(ElementType type, float duration);
	// 現在のエレメントを取得
	ElementType GetCurrentElement();
	// 残り時間を取得
	float GetRemainingTime();
	// エレメントが有効かどうかを取得
	bool IsActive();

private:
	ElementType currentElement_ = ElementType::None;
	float remainingTime_ = 0.0f;

};
