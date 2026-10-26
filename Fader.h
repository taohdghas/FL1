#pragma once

class Fader {
public:
    Fader();

    void fadeIn(float speed);
    void fadeOut(float speed);
    void update(float dt);

    float getAlpha() const;
	bool isFinished() const;

private:
    float alpha;
    float speed;
    bool fadingIn;
    bool fadingOut;
};