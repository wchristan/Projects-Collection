#pragma once

//Base scene class
class GameScene
{
public:
	GameScene();
	virtual ~GameScene();

	virtual void Initialize();

	virtual void Update(float deltaTime_);
	virtual void Render();

	virtual void CleanUp();

private:

};

