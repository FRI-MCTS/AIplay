#ifndef _TOM_PLAYER_ENGINE_
#define _TOM_PLAYER_ENGINE_

//#includes
#include "Game_Interface.hpp"

//defines
#define TOMPLAYER_PLAYER_NUMBER					0
#define TOMPLAYER_EXTERNAL_RESET_ENABLED		1
#define TOMPLAYER_FINAL_OUTPUT_ENABLED			0
#define TOMPLAYER_VISUALIZATION_DEPTH			0

//forward declarations
class Game_Engine;

//class definition
#pragma once
class Player_Engine
{

public:

	//constructor
	Player_Engine(Game_Engine* = NULL, int = TOMPLAYER_PLAYER_NUMBER);

	//virtual public procedures
	virtual void Reset()			{};
	virtual void New_Game()			{};
	virtual int  Get_Move()			= 0;
	virtual void Before_Move(int)	{};
	virtual void After_Move(int)	{};
	virtual void End_Game()			{};

	//virtual public procedures - debug and visualization
	virtual void Output()	{};

	//public procedures
	void Reset_Settings(Game_Engine* = NULL);

	//public variables - player settings
	Game_Engine* game;
	int player_number;
	bool external_reset_enabled;

	//public variables - player definition
	string player_name;

	//public variables - debug and visualization
	bool final_output_enabled;
	int visualization_depth;

protected:

	//virtual protected procedures
	virtual void Init_Settings()	{};
	virtual void Allocate_Memory()	{};
	virtual void Clear_Memory()		{};

	//protected procedures
	void Initialize();

	//protected variables - object state
	bool is_initialized;

};

class Player_Human : public Player_Engine
{

public:

	//public procedures
	Player_Human(Game_Engine* game);
	int Get_Move();

};

class Player_Random : public Player_Engine
{

public:

	//public procedures
	Player_Random(Game_Engine* game);
	int Get_Move();

};

class Player_Passive : public Player_Engine
{

public:

	//public procedures
	Player_Passive(Game_Engine* game);
	int Get_Move();

};

class Player_SameMove : public Player_Engine
{

public:

	//public procedures
	Player_SameMove(Game_Engine* game);
	int Get_Move();

	//public variables
	int move;

};

#include "Game_Engine.hpp"

#endif