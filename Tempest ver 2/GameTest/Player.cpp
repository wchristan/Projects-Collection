#include "stdafx.h"
#include "Player.h"
#include "App/app.h"
#include "Custom.h"
#include "GameState.h"
#include <memory>

#define HIGHSPEED 0.001745f
#define MEDSPEED 0.001745f/2.0f
#define LOWSPEED 0.001745f/4.0f

Player::Player()
{
	screenCenter = Vec2(APP_VIRTUAL_WIDTH / 2, APP_VIRTUAL_HEIGHT / 2);

	//Create ship sprite
	ship = App::CreateSprite(".\\TestData\\Ships.bmp", 2, 12);
	ship->SetFrame(2);
	ship->SetScale(0.7f);

	//Set position & rotation
	colliderRadius = ship->GetWidth() / 6 * ship->GetScale();
	startLocation = THREE_HALF_PI; //Ship starts at cardinal south

	//Basic Variables
	moveSpeed = LOWSPEED;
	pathRadius = 420.f;
	pathRotation = startLocation;
	
	//Player Stats
	
	health = 10;
	attack = 1;
	maxAttack = 10;
	fireRate = 2.0f; //Bullet per second
	maxFireRate = 50.0f; //Bullet per second
	bulletInitDistance = 40.0f;
	bulletSpeed = 200.0f;
	maxBulletSpeed = 600.0f;
	bulletLife = 2000.0f;
	maxBulletLife = 5000.0f;
	bulletRebound = 0;
	bulletSpawnPoints = 1;


	//Set FX variables
	dmgReactTime = 500.0f;
	defColour = Vec3(0.0f, 0.0f, 0.0f);
	dmgColour = Vec3(1.0f, 0.0f, 0.5f);
	
	//Misc settings
	timer.SetTimer(1000.0f / fireRate, FIRETIMER, true);
	timer.SetTimer(dmgReactTime, DMGTIMER, false);
	
	//Set the start location and facing angle
	MoveShip();

	gameState = nullptr;
}

Player::Player(GameState* gameState_) : Player()
{
	gameState = gameState_;
	gameState->Initialize(health);
}

Player::~Player()
{
	CleanUp();
}

void Player::CleanUp()
{
	while (bullets.size() > 0) {
		delete bullets.back();
		bullets.back() = nullptr;
		bullets.pop_back();
	}

	if (gameState != nullptr) {
		gameState = nullptr;
	}

	delete ship;
}

//Calculations
void Player::Update(float deltaTime_)
{
	timer.Update(deltaTime_);
	
	InputUpdate(deltaTime_);

	ship->Update(deltaTime_);

	//Update bullets
	if (bullets.size() > 0) {
		for (int i = (int)bullets.size() - 1; i >= 0; i--) {
			bullets[i]->Update(deltaTime_);
			
			if (bullets[i]->lifeSpan <= 0.0f) {
				DestroyBullet(i);
			}
		}
	}
}

//Graphical
void Player::Render()
{
	ship->Draw();

	//Render bullets
	if (bullets.size() > 0) {
		for (Bullet* &bullet : bullets) {
			bullet->Render();
		}

	}

	//Show FX when damaged
	if (!timer.GetTimeCheck(DMGTIMER)) {
		Custom::DrawPoly(colliderRadius, GetPosition(), 0, 20, dmgColour);
	}
	else {
		Custom::DrawPoly(colliderRadius, GetPosition(), 0, 20, defColour);
	}
}

void Player::InputUpdate(float deltaTime_)
{
	//Get mouse position
	App::GetMousePos(mousePosition.x, mousePosition.y);

	//Get joystick input
	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		pathRotation += moveSpeed * deltaTime_;

		MoveShip();
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		pathRotation -= moveSpeed * deltaTime_;
	
		MoveShip();
	}
	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		
	}
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		
	}

	//Ship Rotation
	RotateShip();

	//Shoot with spacebar
	if (App::IsKeyPressed(APP_PAD_EMUL_BUTTON_ALT_A) && timer.GetTimeCheck(FIRETIMER)) {
		Shoot();
	}
	if (App::IsKeyPressed(APP_PAD_EMUL_BUTTON_LEFT_SHOULDER)) {
		moveSpeed = MEDSPEED;
	}
	else {
		moveSpeed = LOWSPEED;
	}
}

Vec2 Player::CalcBulletSpawn(Vec2& spawn_, float angleDev_)
{
	float x, y;
	ship->GetPosition(x, y);
	float shipAngle = ship->GetAngle();

	//Adjust position from local to global
	spawn_.x = -sinf(shipAngle + (angleDev_ * RAD));
	spawn_.y = cosf(shipAngle + (angleDev_ * RAD));

	spawn_ *= bulletInitDistance;

	//Adjust position from local to global
	spawn_.x += x;
	spawn_.y += y;

	return Vec2(spawn_.x, spawn_.y);
}

void Player::IncreaseHealth(int value_)
{
	health += value_;
	gameState->UpdateHealth(health);
}

void Player::BuffAttack()
{
	if (attack >= maxAttack) return;
	attack++;
	gameState->UpdateBuffs(EBuffType::Attack, attack - 1);
}

void Player::BuffFireRate()
{
	if (fireRate < maxFireRate) {
		fireRate++;
		timer.SetTime(FIRETIMER, 1000.0f / fireRate);

		gameState->UpdateBuffs(EBuffType::FireRate, static_cast<int>(fireRate - 2.0f));
	}
}

void Player::BuffBulletSpeed()
{
	if (bulletSpeed >= maxBulletSpeed) return;

	bulletSpeed += 20.0f;
	gameState->UpdateBuffs(EBuffType::BulletSpeed, static_cast<int>((bulletSpeed - 200) / 20));
}

void Player::BuffBulletRebound()
{
	bulletRebound++;
	gameState->UpdateBuffs(EBuffType::Rebound, bulletRebound);
}

void Player::BuffBulletLife()
{
	if (bulletLife >= maxBulletLife) return;
	bulletLife += 250.0f;
	gameState->UpdateBuffs(EBuffType::BulletLife, (int)(bulletLife - 200.0f)/250.0f);
}

void Player::BuffExtraFire()
{
	if (bulletSpawnPoints >= 3) return;
	bulletSpawnPoints++;
	gameState->UpdateBuffs(EBuffType::Turret, bulletSpawnPoints - 1);
}

void Player::MoveShip()
{
	//This will move the ship around a fixed radius from the center
	shipPosition.x = (APP_VIRTUAL_WIDTH / 2) + cosf(pathRotation) * pathRadius;
	shipPosition.y = (APP_VIRTUAL_HEIGHT / 2) + sinf(pathRotation) * pathRadius;

	//Reset/clamp path rotation
	if (pathRotation > DOUBLE_PI) {
		pathRotation = 0.0f;
	}
	else if (pathRotation < 0.0f) {
		pathRotation = DOUBLE_PI;
	}

	ship->SetPosition(shipPosition.x, shipPosition.y);
}

void Player::RotateShip() 
{
	Vec2 mouseDir = mousePosition - shipPosition;

	float angleToMouse = atan2f(mouseDir.x, mouseDir.y);
	
	ship->SetAngle(-angleToMouse);
}

int Player::GetHealth()
{
	return health;
}

int Player::GetAttack()
{
	return attack;
}

Vec2 Player::GetPosition()
{
	return shipPosition;
}

float Player::GetRadius()
{
	return colliderRadius;
}

float Player::GetBulletSpeed()
{
	return bulletSpeed;
}

void Player::Shoot()
{
	float x, y;
	ship->GetPosition(x, y);

	float shipAngle = ship->GetAngle();

	Vec2 spawn;
	Vec2 bulletVel;

	//Assume magnitude of one, find forward vector
	bulletVel.x = -sinf(shipAngle);
	bulletVel.y = cosf(shipAngle);

	//Bullet velocityMult
	bulletVel *= bulletSpeed;

	CalcBulletSpawn(spawn, 0.0f);

	auto InstBullet = [&](Bullet*& bullet_, Vec2 position_, Vec2 velocity_, float rotation_, unsigned int rebound_, float lifeSpan_) {
		bullet_ = new Bullet(position_, velocity_, rotation_, rebound_, lifeSpan_);
		bullets.push_back(bullet_);
	};
	
	Bullet* bullet = nullptr;

	switch (bulletSpawnPoints) {
	case 2:
		//Secondary bullet spawn
		CalcBulletSpawn(spawn, 30.0f);
		InstBullet(bullet, spawn, bulletVel, -shipAngle * DEG, bulletRebound, bulletLife);

		//Tertiary bullet spawn
		CalcBulletSpawn(spawn, -30.0f);
		InstBullet(bullet, spawn, bulletVel, -shipAngle * DEG, bulletRebound, bulletLife);
		break;
	case 3:
		//Create and add bullet
		InstBullet(bullet, spawn, bulletVel, -shipAngle * DEG, bulletRebound, bulletLife);

		//Secondary bullet spawn
		CalcBulletSpawn(spawn, 30.0f);
		InstBullet(bullet, spawn, bulletVel, -shipAngle * DEG, bulletRebound, bulletLife);

		//Tertiary bullet spawn
		CalcBulletSpawn(spawn, -30.0f);
		InstBullet(bullet, spawn, bulletVel, -shipAngle * DEG, bulletRebound, bulletLife);
		break;
	case 1:
	default:
		//Create and add bullet
		InstBullet(bullet, spawn, bulletVel, -shipAngle * DEG, bulletRebound, bulletLife);
		break;
	}
}

void Player::DestroyBullet(int index_) {
	delete bullets[index_];
	bullets[index_] = nullptr;
	bullets.erase(bullets.begin() + index_);
}

void Player::OnCollision(int type_, int value_)
{
	if (type_ == PLAYERDAMAGE) {
		gameState->UpdateScore(value_);
	}
	if (type_ == ENEMYDAMAGE) {
		health -= value_;
		gameState->UpdateHealth(health);

		timer.ResetTimer(DMGTIMER);
	}
}
