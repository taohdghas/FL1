#include "Fader.h"

Fader::Fader() : alpha(0.0f), speed(1.0f), fadingIn(false), fadingOut(false) {}

// 秒数指定してフェードイン
void Fader::fadeIn(float second) {
	speed = 1.0f / second;
	fadingIn = true;
	fadingOut = false;
}

// 秒数指定してフェードアウト
void Fader::fadeOut(float second) {
	if (second <= 0.0f) {
		second = 0.01f;
	}

	speed = 1.0f / second;
	fadingOut = true;
	fadingIn = false;
}

// 更新
void Fader::update(float dt) {
	if (fadingIn) {
		alpha += speed * dt;
		if (alpha >= 1.0f) {
			alpha = 1.0f;
			fadingIn = false;
		}
	}

	if (fadingOut) {
		alpha -= speed * dt;
		if (alpha <= 0.0f) {
			alpha = 0.0f;
			fadingOut = false;
		}
	}
}

float Fader::getAlpha() const { return alpha; }

void Fader::setAlpha(float value) { alpha = value; }

bool Fader::isFinished() const { return !fadingIn && !fadingOut; }