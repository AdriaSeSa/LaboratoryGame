#pragma once
#include "GameObject.h"
#include "GroundSensor.h"
#include "Animation.h"
#include "HitboxSensor.h"

enum PlayerState
{
	IDLE,
	RUN,
	JUMP,
	DOUBLE_JUMP,
	FALL
};

class Player : public GameObject
{
private:
	Animation idle;
	Animation run;
	Animation doubleJump;
	Animation appearing;
	PlayerState playerCurrentState;

	bool isAppear = false;
	bool isLookingLeft;
	bool jumpBlock = false;

	bool godMod = false;

	float fallDetection = 0.1f;

public:
	Player(iPoint pos, std::string name, std::string tag, Application* app);
	~Player();
	void Update() override;

	void UpdatePlayerState();

	void OnCollisionEnter(PhysBody* col) override;
	void OnCollisionExit(PhysBody* col) override;
	void OnTriggerEnter(PhysBody* col) override;

	void Start() override;

	void PreUpdate() override;

	void PostUpdate() override;

	void CleanUp() override;

	void PlayerAppear();

	void Die();

	bool appliedFallForce = false;
	bool isFalling;
	bool isOnGround;
	bool isDead = false;

	GroundSensor* groundSensor;
	GroundSensor* openPlatformSensor;
	GroundSensor* closePlatformSensor;
	HitboxSensor* hitBoxSensor;
	iPoint groundSensorOffset = { 0, 8 };
	iPoint platformSensorOffset = { 0, -5 };
	
	int lifes = 3;

	int gravityScale = 2;

	int jumpCount = 2;

	float jumpForce = 6.7f;

	float speed = 5;
};
