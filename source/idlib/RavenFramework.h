// dude-quake4-game: what Raven's engine headers declared that the game owns or names itself -
// its licensee names, the cvar help list, the memory report, and the engine globals the game
// defines (Game_local.cpp) - included by game/Game.h after DUDE's Quake 4 interface
// (quake4/GameNamesQ4.h). Raven's own code, from its framework/licensee.h, Common.h,
// CVarSystem.h, sound/sound.h, cm/CollisionModel.h, bse/BSEInterface.h and game/Game.h.

#ifndef __RAVEN_FRAMEWORK_H__
#define __RAVEN_FRAMEWORK_H__

// ---------------------------------------------------------------------------------------------
// framework/licensee.h, sys/sys_public.h
// ---------------------------------------------------------------------------------------------
#define	CD_BASEDIR						"Quake4"
#define GAME_BUILD_TYPE					""					// a release build
#ifndef BUILD_STRING
#define BUILD_STRING					D3_OSTYPE "-" D3_ARCH	// DUDE's build names them
#endif

// DUDE: the largest render entity extent SaveGame's asserts accept - the name is Raven's, its
// value was in an engine header the SDK did not ship: the size of the world
#define MAX_BOUND_SIZE					( 256.0f * 1024.0f )

// ---------------------------------------------------------------------------------------------
// framework/CVarSystem.h - rjohnson: new help system for cvar ui. Raven's engine listed these in
// a cvar menu; DUDE has none, so the helps are kept (and found by the game) but not registered.
// ---------------------------------------------------------------------------------------------
typedef enum {
	CVARHELP_ALL			= -1,		// all categories
	CVARHELP_GAME			= BIT(0),	// game menu
	CVARHELP_RENDERER		= BIT(1),	// renderer menu
	CVARHELP_PHYSICS		= BIT(2),	// physics menu
	CVARHELP_SOUND			= BIT(3),	// sound menu
	CVARHELP_AI				= BIT(4),	// AI menu
} cvarHelpCategory_t;

class idCVarHelp {
public:
								// Always use one of the following constructors.
								idCVarHelp( const char *cvarName, const char *friendlyName, const char *choices, const char *values, cvarHelpCategory_t category );

								idCVarHelp( const idCVarHelp &copy );

	const char *				GetCVarName( void ) const { return cvarName; }	
	const char *				GetFriendlyName( void ) const { return friendlyName; }	
	const char *				GetChoices( void ) const { return choices; }	
	const char *				GetValues( void ) const { return values; }	
	const cvarHelpCategory_t	GetCategory( void ) const { return category; }	
	const idCVarHelp *			GetNext( void ) const { return next; }

	void						SetNext( const idCVarHelp *value ) { next = value; }

	static void					RegisterStatics( void );

private:
	const char *				cvarName;				// the cvar this help belongs to
	const char *				friendlyName;			// a textual name for the cvar impaired
	const char *				choices;				// a textual list of choices for the cvar impaired
	const char *				values;					// the list of values that goes with the choices
	cvarHelpCategory_t			category;				// the category(s) this cvar should appear under
	const idCVarHelp *			next;					// next statically declared cvar helper

	static idCVarHelp *			staticCVarHelps;
	static idCVarHelp *			staticCVarHelpsTail;
};

ID_INLINE idCVarHelp::idCVarHelp( const idCVarHelp &copy ) {
	cvarName = copy.cvarName;
	friendlyName = copy.friendlyName;
	choices = copy.choices;
	values = copy.values;
	category = copy.category;
	next = NULL;
}

ID_INLINE idCVarHelp::idCVarHelp( const char *cvarName, const char *friendlyName, const char *choices, const char *values, cvarHelpCategory_t category ) {
	this->cvarName = cvarName;
	this->friendlyName = friendlyName;
	this->choices = choices;
	this->values = values;
	this->category = category;
	this->next = NULL;

	// DUDE: listed in declaration order, as Raven's statics were, with no engine to register with
	if ( !staticCVarHelpsTail ) {
		staticCVarHelps = this;
	} else {
		staticCVarHelpsTail->next = this;
	}
	staticCVarHelpsTail = this;
}

ID_INLINE void idCVarHelp::RegisterStatics( void ) {
}

// ---------------------------------------------------------------------------------------------
// framework/Common.h - the memory report (converted to a class so the idStr gets constructed)
// ---------------------------------------------------------------------------------------------
class MemInfo {
public:
					MemInfo( void ) {
						total = assetTotals = 0;
						imageAssetsTotal = modelAssetsTotal = soundAssetsTotal = 0;
						collAssetsTotal = animsAssetsTotal = aasAssetsTotal = 0;
						imageAssetsCount = modelAssetsCount = soundAssetsCount = 0;
						collAssetsCount = animsAssetsCount = aasAssetsCount = 0;
					}

	idStr			filebase;

	int				total;
	int				assetTotals;

	// asset totals
	int				imageAssetsTotal;
	int				modelAssetsTotal;
	int				soundAssetsTotal;
	int				collAssetsTotal;
	int				animsAssetsTotal;
	int				aasAssetsTotal;

	int				imageAssetsCount;
	int				modelAssetsCount;
	int				soundAssetsCount;
	int				collAssetsCount;
	int				animsAssetsCount;
	int				aasAssetsCount;
};

// ---------------------------------------------------------------------------------------------
// the engine interfaces the game library holds (defined in Game_local.cpp, from the import)
// ---------------------------------------------------------------------------------------------
extern idGame *						game;
extern idGameEdit *					gameEdit;
extern idSoundSystem *				soundSystem;
extern idCollisionModelManager *	collisionModelManager;
extern rvBSEManager *				bse;

#endif /* !__RAVEN_FRAMEWORK_H__ */
