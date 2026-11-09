#pragma once

class Fader {
public:
    Fader();

    void fadeIn(float second);
	void fadeOut(float second);
    void update(float dt);

    float getAlpha() const;
	void setAlpha(float alpha);

	bool isFinished() const;

private:
    float alpha;
    float speed;
    bool fadingIn;
    bool fadingOut;
};