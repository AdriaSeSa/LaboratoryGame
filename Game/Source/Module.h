#ifndef _MODULE_H_
#define _MODULE_H_


#include "SString.h"


class Application;
class PhysBody;


class Module
{
private :
	bool enabled;

public:

	Application* App;

	SString name;

	Module(Application* parent, bool start_enabled = true) : App(parent), enabled(start_enabled)
	{}

	virtual ~Module()
	{}

	bool IsEnabled() const
	{
		return enabled;
	}

	void Enable()
	{
		if(enabled == false)
		{
			enabled = true;
			Start();
		}
	}

	void Disable()
	{
		if(enabled == true)
		{
			enabled = false;
			CleanUp();
		}
	}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual UpdateStatus PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual UpdateStatus Update()
	{
		return UPDATE_CONTINUE;
	}

	virtual UpdateStatus PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void OnCollision(PhysBody* body1, PhysBody* body2)
	{
	
	}
};

#endif