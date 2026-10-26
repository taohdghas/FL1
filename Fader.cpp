#include "Fader.h"

Fader::Fader() : alpha(0.0f), speed(1.0f), fadingIn(false), fadingOut(false) {}

void Fader::fadeIn(float s) {
	speed = s;
	fadingIn = true;
	fadingOut = false;
}

void Fader::fadeOut(float s) {
	speed = s;
	fadingOut = true;
	fadingIn = false;
}

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

bool Fader::isFinished() const { return !fadingIn && !fadingOut; }