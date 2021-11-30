#pragma once
#include "GameObject.h"
#include "Animation.h"

enum CheckpointState
{
	IDLE_NO_FLAG,
	GET_CHECKPOINT,
	IDLE_FLAG
};

class CheckPoint : public GameObject
{
private:
	CheckpointState currentState;

	Animation getCheckpoint;
	Animation idleFlag;

public:
	CheckPoint(iPoint pos, std::string name, std::string tag, Application* app);

	void PostUpdate() override;

	void DrawCurrentState(int i);

	void OnCollisionEnter(PhysBody* col)override;
	void OnCollisionExit(PhysBody* col) override;
};

