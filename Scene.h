#pragma once

class SceneManager;
class Scene {
public:
	virtual ~Scene() = default;

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager;}

protected:
	SceneManager* sceneManager_ = nullptr;
};