#pragma once

#include <vector>


//Class that helps with collision checks between objects
//Collisions are all circular colliders
class CollisionCheck
{
public:
	class Player* player;
	class Spawner* spawner;

	//Timer to limit the rate of collision checks per update
	float timer;
	float timeElapsed;
	
	CollisionCheck();
	CollisionCheck(class Player* player_, class Spawner* spawn_);

	~CollisionCheck();


	void Update(float deltaTime_);
	void Checks();

};

