// dude-quake4-game: the parts of Raven's idlib the game uses that DUDE's idlib does not have.
//
// The game compiles against DUDE's idlib (precompiled.h), whose types cross into the engine.
// Raven's own types and switches that never cross the boundary come from here, as Raven wrote
// them in its idlib (Heap.h, rvMemSys.h, math/Math.h + Math.cpp, containers/Pair.h,
// math/Interpolate.h), on Raven's PC build: no tracking memory system (_RV_MEM_SYS_SUPPORT
// off), so the heap and tag macros compile to nothing and DUDE's allocator does the work.

#ifndef __RAVEN_IDLIB_H__
#define __RAVEN_IDLIB_H__

// ---------------------------------------------------------------------------------------------
// allocation tags (Heap.h) - recorded by Raven's tracking allocator, ignored here
// ---------------------------------------------------------------------------------------------
// RAVEN BEGIN
// amccarthy:  tags for memory allocation tracking.
typedef enum {
	MA_NONE = 0,

	MA_OPNEW,
	MA_DEFAULT,
	MA_LEXER,
	MA_PARSER,
	MA_AAS,
	MA_CLASS,
	MA_SCRIPT,
	MA_CM,
	MA_CVAR,
	MA_DECL,
	MA_FILESYS,
	MA_IMAGES,
	MA_MATERIAL,
	MA_MODEL,
	MA_FONT,
	MA_RENDER,
	MA_VERTEX,
	MA_SOUND,
	MA_WINDOW,
	MA_EVENT,
	MA_MATH,
	MA_ANIM,
	MA_DYNAMICBLOCK,
	MA_STRING,
	MA_GUI,
	MA_EFFECT,
	MA_ENTITY,
	MA_PHYSICS,
	MA_AI,
	MA_NETWORK,

	MA_DO_NOT_USE,
	MA_MAX
} Mem_Alloc_Types_t;
// RAVEN END

#define MEM_SCOPED_TAG(var, tag)
#define MEM_SCOPED_TAG_SET(var, tag)

// Raven's allocation calls carry a tag; DUDE's allocator takes none
ID_INLINE void *	Mem_Alloc( const int size, byte tag ) { return Mem_Alloc( size ); }
ID_INLINE void *	Mem_ClearedAlloc( const int size, byte tag ) { return Mem_ClearedAlloc( size ); }
ID_INLINE void *	Mem_Alloc16( const int size, byte tag ) { return Mem_Alloc16( size ); }

// ---------------------------------------------------------------------------------------------
// heaps (rvMemSys.h) - the system heaps of Raven's memory system, not built on the PC
// ---------------------------------------------------------------------------------------------
typedef enum
{
	RV_HEAP_ID_DEFAULT,			// heap that exists on application startup
	RV_HEAP_ID_PERMANENT,		// heap for allocations that have permanent (application scope) lifetime
	RV_HEAP_ID_LEVEL,			// heap for allocations that have a level lifetime
	RV_HEAP_ID_MULTIPLE_FRAME,	// heap for run-time allocations that have a lifetime of multiple draw frames
	RV_HEAP_ID_SINGLE_FRAME,	// heap for run-time allocations that have a lifetime of a single draw frame
	RV_HEAP_ID_TEMPORARY,		// heap for objects that have a short lifetime (temporaries generally used for level loading)
	RV_HEAP_ID_IO_TEMP,			// heap for allocations that are temporaries used in I/O operations like level loading or writing out data
	rv_heap_ID_max_count		// just a count, not a valid type
}
Rv_Sys_Heap_ID_t;

#define RV_PUSH_SYS_HEAP_ID(sysHeapID)
#define RV_PUSH_SYS_HEAP_ID_AUTO(varName,sysHeapID)
#define RV_PUSH_HEAP_MEM(memPtr)
#define RV_PUSH_HEAP_MEM_AUTO(varName,memPtr)
#define RV_PUSH_HEAP_PTR(heapPtr)
#define RV_POP_HEAP()
#define RV_PUSH_SYS_HEAP_ENTER_CRIT_SECT(sysHeapID)
#define RV_PUSH_HEAP_ENTER_CRIT_SECT(heapPtr)
#define RV_POP_HEAP_EXIT_CRIT_SECT()

// ---------------------------------------------------------------------------------------------
// rvRandom (math/Math.h, Math.cpp) - jscott: fast and reliable random routines
// ---------------------------------------------------------------------------------------------
class rvRandom {
private:
	static	unsigned long	mSeed;
public:
							rvRandom( void ) { mSeed = 0x89abcdef; }

	// for a non seed based init
	static	int				Init( void );

	// Init the seed to a unique number
	static	void			Init( unsigned long seed ) { mSeed = seed; }

	// Returns a float min <= x < max (exclusive; will get max - 0.00001; but never max)
	static	float			flrand( float min, float max );

	// Returns a float min <= 0 < 1.0
	static	float			flrand();

	static	float			flrand( const idVec2& v );

	// Returns an integer min <= x <= max (ie inclusive)
	static	int				irand( int min, int max );
};

// ---------------------------------------------------------------------------------------------
// rvPair (containers/Pair.h)
// ---------------------------------------------------------------------------------------------
template< class type1, class type2 >
class rvPair {
public:
	rvPair() {};
	rvPair( const type1& T1, const type2& T2 ) { first = T1; second = T2; };

	const type1& First( void ) const { return first; };
	const type2& Second( void ) const { return second; };

	static int rvPairFirstCompare( const rvPair< type1, type2 > *a, const rvPair< type1, type2 > *b ) {
		return b->First() - a->First();
	}

	static int rvPairSecondCompare( const rvPair< type1, type2 > *a, const rvPair< type1, type2 > *b ) {
		return  b->Second() - a->Second();
	}

	static int rvPairFirstCompareDirect( const rvPair< type1, type2 > *a, const rvPair< type1, type2 > *b ) {
		if( b->First() - a->First() < 0.001f || b->First() - a->First() < -0.001f ) {
			return 0;
		}

		if( a->First() > b->First() ) {
			return -1;
		} else if( a->First() < b->First() ) {
			return 1;
		}
		return 0;
	}

	static int rvPairSecondCompareDirect( const rvPair< type1, type2 > *a, const rvPair< type1, type2 > *b ) {
		if( b->Second() - a->Second() < 0.001f || b->Second() - a->Second() < -0.001f ) {
			return 0;
		}

		if( a->Second() > b->Second() ) {
			return -1;
		} else if( a->Second() < b->Second() ) {
			return 1;
		}
		return 0;
	}

private:
	type1 first;
	type2 second;
};

// ---------------------------------------------------------------------------------------------
// rvSphericalInterpolate (math/Interpolate.h). Raven derived it from its idInterpolate, which
// exposes its members to subclasses; DUDE's idInterpolate keeps them private, so the same
// class stands alone here, with the members and behaviour it had.
// ---------------------------------------------------------------------------------------------
ID_INLINE float Linear( float frac ) {
	return frac;
}

ID_INLINE float SinusoidalMidPoint( float frac ) {
	// idMath::MidPointLerp( 0, 60, 90, frac ), as Raven's Math.cpp defines it
	const float degrees = ( frac < 0.5f ) ? 0.0f + ( 60.0f - 0.0f ) * ( 2.0f * frac )
										  : 60.0f + ( 90.0f - 60.0f ) * ( 2.0f * ( frac - 0.5f ) );
	return idMath::Sin( DEG2RAD( degrees ) );
}

typedef float (*TimeManipFunc) ( float );

class rvSphericalInterpolate {
public:
						rvSphericalInterpolate() : startTime( 0.0f ), duration( 0.0f ), currentTime( 0.0f ) {
							// as idInterpolate's constructor: all values zeroed
							currentValue.Set( 0.0f, 0.0f, 0.0f, 0.0f );
							startValue = endValue = currentValue;
							SetTimeFunction( SinusoidalMidPoint );
						}

	void				Init( const float startTime, const float duration, const idQuat &startValue, const idQuat &endValue ) {
							this->startTime = startTime;
							this->duration = duration;
							this->startValue = startValue;
							this->endValue = endValue;
							this->currentTime = startTime - 1;
							this->currentValue = startValue;
						}
	void				SetStartTime( float time ) { this->startTime = time; }
	void				SetDuration( float duration ) { this->duration = duration; }
	void				SetStartValue( const idQuat &startValue ) { this->startValue = startValue; }
	void				SetEndValue( const idQuat &endValue ) { this->endValue = endValue; }

	idQuat				GetCurrentValue( float time ) const {
							float deltaTime;

							deltaTime = time - startTime;
							if ( time != currentTime ) {
								currentTime = time;
								if( duration == 0.0f ) {
									currentValue = endValue;
								} else {
									currentValue.Slerp( startValue, endValue, timeFunc((float)deltaTime / duration) );
								}
							}
							return currentValue;
						}
	bool				IsDone( float time ) const { return ( time >= startTime + duration ); }

	float				GetStartTime( void ) const { return startTime; }
	float				GetEndTime( void ) const { return startTime + duration; }
	float				GetDuration( void ) const { return duration; }
	const idQuat &		GetStartValue( void ) const { return startValue; }
	const idQuat &		GetEndValue( void ) const { return endValue; }

	void				SetTimeFunction( TimeManipFunc func ) { timeFunc = func; }

protected:
	float				startTime;
	float				duration;
	idQuat				startValue;
	idQuat				endValue;
	mutable float		currentTime;
	mutable idQuat		currentValue;
	TimeManipFunc		timeFunc;
};

#endif /* !__RAVEN_IDLIB_H__ */
