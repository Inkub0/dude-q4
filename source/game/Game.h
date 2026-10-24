#ifndef __Q4_GAME_H__
#define __Q4_GAME_H__

// (not __GAME_H__: that is DUDE's framework/Game.h guard, included below)

/*
	dude-quake4-game: Raven's game/Game.h, split (the original is in the first commit).

	The engine <-> game interface Raven declared here - gameReturn_t, allowReply_t, escReply_t,
	idGame, refSound_t, idGameEdit, gameImport_t, gameExport_t, GetGameAPI_t, and the types
	their calls take (demoState_t, ClientStats_t, userOrigin_t, debugInfoProc_t, saveType_t) -
	is DUDE's Quake 4 game interface, quake4/GameQ4.h; quake4/GameNamesQ4.h gives the types the
	names this code uses. What Raven kept here besides that is game-side and stays, unchanged.
*/

#include "quake4/GameNamesQ4.h"
#include "../idlib/RavenFramework.h"

// RAVEN BEGIN
// bgeisler: moved into scripts directory
// default scripts
#define SCRIPT_DEFAULTDEFS			"scripts/defs.script"
#define SCRIPT_DEFAULT				"scripts/main.script"
// RAVEN END
#define SCRIPT_DEFAULTFUNC			"doom_main"

enum demoReliableGameMessage_t {
	DEMO_RECORD_CLIENTNUM,
	DEMO_RECORD_EXCLUDE,
	DEMO_RECORD_COUNT
};

//
// these defines work for all startsounds from all entity types
// make sure to change script/doom_defs.script if you add any channels, or change their order
//
typedef enum {
	SND_CHANNEL_ANY = SCHANNEL_ANY,
	SND_CHANNEL_VOICE = SCHANNEL_ONE,
	SND_CHANNEL_VOICE2,
	SND_CHANNEL_BODY,
	SND_CHANNEL_BODY2,
	SND_CHANNEL_BODY3,
	SND_CHANNEL_WEAPON,
	SND_CHANNEL_ITEM,
	SND_CHANNEL_HEART,
	SND_CHANNEL_DEMONIC,
	SND_CHANNEL_RADIO,

	// internal use only.  not exposed to script or framecommands.
	SND_CHANNEL_AMBIENT,
	SND_CHANNEL_DAMAGE

// RAVEN BEGIN
// bdube: added custom to tell us where the end of the predefined list is
	,
	SND_CHANNEL_POWERUP,
	SND_CHANNEL_POWERUP_IDLE,
	SND_CHANNEL_MP_ANNOUNCER,
	SND_CHANNEL_CUSTOM
// RAVEN END
} gameSoundChannel_t;

// RAVEN BEGIN
// bdube: forward reference
class rvClientEffect;
// RAVEN END

// RAVEN BEGIN
// bdube: more forward declarations
class idProgram;
class idInterpreter;
class idThread;
// RAVEN END

// RAVEN BEGIN
// bdube: game logging
/*
===============================================================================

	Game Log.

===============================================================================
*/
class rvGameLog {
public:
	virtual				~rvGameLog( void ) {}

	virtual void		Init		( void ) = 0;
	virtual void		Shutdown	( void ) = 0;

	virtual void		BeginFrame	( int time ) = 0;
	virtual void		EndFrame	( void ) = 0;

	virtual	void		Set			( const char* keyword, int value ) = 0;
	virtual void		Set			( const char* keyword, float value ) = 0;
	virtual void		Set			( const char* keyword, const char* value ) = 0;
	virtual void		Set			( const char* keyword, bool value ) = 0;

	virtual void		Add			( const char* keyword, int value ) = 0;
	virtual void		Add			( const char* keyword, float value ) = 0;
};

extern rvGameLog *				gameLog;

#define GAMELOG_SET(x,y)		{if(g_gamelog.GetBool())gameLog->Set ( x, y );}
#define GAMELOG_ADD(x,y)		{if(g_gamelog.GetBool())gameLog->Add ( x, y );}

#define GAMELOG_SET_IF(x,y,z)	{if(g_gamelog.GetBool()&&(z))gameLog->Set ( x, y );}
#define GAMELOG_ADD_IF(x,y,z)	{if(g_gamelog.GetBool()&&(z))gameLog->Add ( x, y );}

// RAVEN END

#endif /* !__Q4_GAME_H__ */
