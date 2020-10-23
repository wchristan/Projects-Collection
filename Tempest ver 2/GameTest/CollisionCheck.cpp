#include "stdafx.h"
#include "CollisionCheck.h"
#include "Player.h"
#include "Spawner.h"
#include "App/AppSettings.h"


CollisionCheck::CollisionCheck()
{
	player = nullptr;
	spawner = nullptr;
	timer = 50.0f;
	timeElapsed = 0.0f;
}

CollisionCheck::CollisionCheck(Player* player_, Spawner* spawn_)
{
	player = player_;
	spawner = spawn_;
	timer = 50.0f;
	timeElapsed = 0.0f;
}

CollisionCheck::~CollisionCheck()
{
	
}

void CollisionCheck::Update(float deltaTime_)
{
	timeElapsed -= deltaTime_;

	if (timeElapsed > 0.0f) {
		return;
	}

	Checks();

	timeElapsed = timer;
}

void CollisionCheck::Checks()
{
	//Obstacles-Bullets collision
	for (int i = (int)spawner->obstacles.size() - 1; i >= 0; i--) {
		for (int j = (int)player->bullets.size() - 1; j >= 0; j--) {

			Enemy* enemy = *&spawner->obstacles[i];
			Bullet* bullet = *&player->bullets[j];
			
			float dist = Math::MagSq(enemy->GetPosition() - bullet->GetPosition());

			float radDist = enemy->GetRadius() + bullet->GetRadius();
			radDist *= radDist;

			if (dist <= radDist) {
				if (bullet->rebound == 0) {
					player->DestroyBullet(j);

					break;
				}

				Vec2 hitDirection = bullet->GetPosition() - enemy->GetPosition();
				
				bullet->SetReflection(hitDirection);

				//Subtract rebound
				bullet->rebound--;
			}
		}
	}

	//Boss Enemies
	for (int i = (int)spawner->bossEnemies.size() - 1; i >= 0; i--) {
		for (int j = (int)player->bullets.size() - 1; j >= 0; j--) {

			float dist = Math::MagSq(spawner->bossEnemies[i]->GetPosition() - player->bullets[j]->GetPosition());
			
			float radDist = spawner->bossEnemies[i]->GetRadius() + player->bullets[j]->radius;
			radDist *= radDist;

			if (dist <= radDist) {
				player->OnCollision(PLAYERDAMAGE, spawner->bossEnemies[i]->GetValue());
				spawner->bossEnemies[i]->OnCollision(player->GetAttack());
				player->DestroyBullet(j);

				break;
			}
		}
	}

	//Bullet-Enemies collision
	for (int i = (int)player->bullets.size() - 1; i >= 0; i--) {
		for (int j = (int)spawner->enemies.size() - 1; j >= 0; j--) {

			float dist = Math::MagSq(player->bullets[i]->GetPosition() - spawner->enemies[j]->GetPosition());

			float radDist = player->bullets[i]->radius + spawner->enemies[j]->GetRadius();
			radDist *= radDist;

			//If there's a collision between a bullet & an enemy...
			if (dist <= radDist) {
				
				//Damage enemy
				bool isEnemyKilled = spawner->enemies[j]->OnCollision(player->GetAttack());	
				
				//Destroy bullet
				if (!isEnemyKilled || player->bullets[i]->rebound == 0) {
					player->DestroyBullet(i);
				}
				else {
					player->bullets[i]->rebound--;
				}

				//Break to update the containers' size
				break;
			}
		}
	}

	//Player-Obstacles Collision
	for (int i = (int)spawner->obstacles.size() - 1; i >= 0; i--) {

		float dist = Math::MagSq(player->GetPosition() - spawner->obstacles[i]->GetPosition());
		float radDist = player->GetRadius() + spawner->obstacles[i]->GetRadius();
		radDist *= radDist;

		//If obstacle hits player...
		if (dist <= radDist) {
			player->OnCollision(ENEMYDAMAGE, spawner->obstacles[i]->GetAttack());	//Damage player

			spawner->DestroyObstacle(i);

			break;
		}
	}

	//Player-Enemies Collision
	for (int i = (int)spawner->enemies.size() - 1; i >= 0; i--) {

		float dist = Math::MagSq(player->GetPosition() - spawner->enemies[i]->GetPosition());
		float radDist = player->GetRadius() + spawner->enemies[i]->GetRadius();
		radDist *= radDist;

		//If enemy hits player...
		if (dist <= radDist) {
			player->OnCollision(ENEMYDAMAGE, spawner->enemies[i]->GetAttack());	//Damage player

			spawner->DestroyEnemy(i);

			break;
		}
	}
}

