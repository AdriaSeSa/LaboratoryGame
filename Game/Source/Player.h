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
	PlayerState playerCurrentState = IDLE;

	bool isAppear = false;
	bool isLookingLeft;
	bool jumpBlock = false;

	bool godMod = false;

	float fallDetection = 0.1f;

	bool appliedFallForce = false;
	bool isFalling = false;
	bool isOnGround = false;

	float totalSkillCoolDown = 20;

	GroundSensor* groundSensor = nullptr;
	GroundSensor* openPlatformSensor = nullptr;
	GroundSensor* closePlatformSensor = nullptr;
	HitboxSensor* hitBoxSensor = nullptr;
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

	bool usingSkill; // De momento esto solo funciona con la habilidad de bajar rapido. Si se añaden habilidades se tiene que cambiar el sistema
						// Ahora mismo el enemigo comprueba esta variable para morir de golpe o no. Mas adelante deber?tener en cuenta si esta habilidad
						// es la de bajar rapido o es cualquier otra
	float skillCoolDown = 20;


public:

	Player(iPoint pos, std::string name, std::string tag, Application* app);
	~Player();
	void Update() override;

	void UpdatePlayerState();

	void OnCollisionEnter(PhysBody* col) override;
	void OnCollisionExit(PhysBody* col) override;
	void OnTriggerEnter(PhysBody* trigger, PhysBody* col) override;
	void OnTriggerStay(PhysBody* trigger, PhysBody* col) override;

	void Start() override;

	void PreUpdate() override;

	void PostUpdate() override;

	void CleanUp() override;

	void ResetJumpCount(int count = 2);

	void Jump(bool playSFX = true);

	void PlayerAppear();

	void Die();

private:

	void SetUpAnimations();
};