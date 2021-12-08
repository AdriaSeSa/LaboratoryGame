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

	bool appliedFallForce = false;
	bool isFalling;
	bool isOnGround;

	GroundSensor* groundSensor;
	GroundSensor* openPlatformSensor;
	GroundSensor* closePlatformSensor;
	HitboxSensor* hitBoxSensor;
	iPoint groundSensorOffset = { 5, 14};
	iPoint platformSensorOffset = { 5, 1};

	// Para plataforma H
	float relativeVelocity_X = 0;
	PhysBody* parent = nullptr;

public:

	bool isDead = false;

	int gravityScale = 2;

	int jumpCount = 2;

	float jumpForce = 7.0f;

	float speed = 5;

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

	void ResetJumpCount(int count = 2);

	void Jump();

	void PlayerAppear();

	void Die();

private:

	void SetUpAnimations();
};