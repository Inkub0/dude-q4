// dude-quake4-game: the parts of Raven's idlib the game uses that DUDE's idlib does not have.
//
// The game compiles against DUDE's idlib (precompiled.h), whose types cross into the engine.
// Raven's own types and switches that never cross the boundary come from here, as Raven wrote
// them in its idlib (Heap.h, rvMemSys.h, math/Math.h + Math.cpp, math/Vector.cpp, Str.h + Str.cpp,
// containers/List.h, Pair.h, Queue.h, math/Interpolate.h, BitMsg.h + BitMsg.cpp), on Raven's PC
// build: no tracking memory system (_RV_MEM_SYS_SUPPORT off), so the heap and tag macros compile
// to nothing and DUDE's allocator does the work.
//
// Methods Raven added to id's classes (idVec3::Dist, idBitMsg::WriteLong, ...) are DUDE's: its
// idlib has them, written for DUDE, without changing a layout. The few whose Raven behaviour has
// quirks the game was shipped with - the idList heap, idStr::Split, the escape-code functions -
// are kept here as Raven wrote them, as free functions, and the game calls them by these names.

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
// math (math/Math.h, math/Vector.cpp)
// ---------------------------------------------------------------------------------------------
// Sign() with zero counted as positive
template<class T> ID_INLINE T	SignZero( T f ) { return ( f > 0 ) ? 1 : ( ( f < 0 ) ? -1 : 1 ); }

extern idVec4 vec4_one;

// ---------------------------------------------------------------------------------------------
// string colours, icons and escape codes (Str.h, Str.cpp)
// ---------------------------------------------------------------------------------------------
// RAVEN BEGIN
// bdube: added
const int C_COLOR_CONSOLE			= ':';
// RAVEN END

// RAVEN BEGIN
// bdube: added
#define S_COLOR_CONSOLE				"^:"
// ddynerman: team colors
#define S_COLOR_MARINE				"^c683"
#define S_COLOR_STROGG				"^c950"
#define S_COLOR_ALERT				"^c920"
// ddynerman: MP icons
#define I_VOICE_ENABLED				"^ivce"
#define I_VOICE_DISABLED			"^ivcd"
#define I_FRIEND_ENABLED			"^ifde"
#define I_FRIEND_DISABLED			"^ifdd"
#define I_FLAG_MARINE				"^iflm"
#define I_FLAG_STROGG				"^ifls"
#define I_READY						"^iyrd"
#define I_NOT_READY					"^inrd"
// shouchard:  server browser stuff
#define I_SERVER_DEDICATED			"^ids0"
#define I_SERVER_DEDICATED_PB		"^idsp"
#define I_SERVER_LOCKED				"^isl0"
#define I_SERVER_FAVORITE			"^isf0"
// RAVEN END

// RAVEN BEGIN
// bdube: string escape codes
#define S_ESCAPE_UNKNOWN			BIT(0)
#define S_ESCAPE_COLOR				BIT(1)
#define S_ESCAPE_COLORINDEX			BIT(2)
#define S_ESCAPE_ICON				BIT(3)
#define S_ESCAPE_COMMAND			BIT(4)
#define	S_ESCAPE_ALL				( S_ESCAPE_COLOR | S_ESCAPE_COLORINDEX | S_ESCAPE_ICON | S_ESCAPE_COMMAND )
// RAVEN END

// Raven's idStr escape-code functions and Split (static members of its idStr), here as rvStr::
// statics; the game's idStr::IcmpNoEscape( a, b ), s.RemoveEscapes(), idStr::Split( src, ... )
// and s.Split( ... ) call these.
class rvStr {
public:
	// length of the escape code at s (0: none), its S_ESCAPE_* type in *type
	static int				IsEscape( const char *s, int *type = NULL );
	static int				LengthWithoutEscapes( const char *s );
	static int				IcmpNoEscape( const char *s1, const char *s2 );
	static char *			RemoveEscapes( char *s, int escapes = S_ESCAPE_ALL );
	static idStr &			RemoveEscapes( idStr &s, int escapes = S_ESCAPE_ALL );
	static void				Split( const char *source, idList<idStr> &list, const char delimiter = ',', const char groupDelimiter = '\'' );
};

ID_INLINE idStr &rvStr::RemoveEscapes( idStr &s, int escapes ) {
	if ( s.Length() ) {
		RemoveEscapes( &s[0], escapes );		// in place, never longer
		s.CapLength( idStr::Length( s.c_str() ) );
	}
	return s;
}

// ---------------------------------------------------------------------------------------------
// idList as a heap (containers/List.h: cdr's HeapAdd / HeapPop members), here as free functions;
// the game's list.HeapAdd( obj ) and list.HeapPop() call these
// ---------------------------------------------------------------------------------------------
/*
================
idList<type>::HeapAdd

Adds a value to the list as if the list was a heap by doing a bubble swap sort
First it appends the object, then swaps up the heap at successive parent positions
as needed

Complexity: O[n log n]
================
*/
template< class type >
ID_INLINE void rvListHeapAdd( idList<type> &list, const type & obj ) {

	int pos = list.Append( obj );

	while (pos && list[((pos-1)/2)]<list[pos])
	{
		idSwap(list[((pos-1)/2)], list[pos]);
		pos = ((pos-1)/2);
	}
}

/*
================
idList<type>::HeapPop

Removes the top element from the heap and
First swaps the top element of the heap with the lowest
element, destroys the lowest element (wich was the top),
and then sorts the new top element down the heap as
needed

Complexity: O[n log n]
================
*/
template< class type >
ID_INLINE void rvListHeapPop( idList<type> &list ) {

	assert(list.Num()>0);
	if (list.Num()<=0)
	{
		return;
	}

	idSwap(list[0], list[list.Num()-1]);
	list.SetNum( list.Num() - 1, false );		// Raven: num--
	const int num = list.Num();

	int pos = 0;

	int largestChild = pos;
	if (((2*pos)+1)<num)
	{
		if (((2*pos)+2)<num)
		{
			largestChild = ( (list[((2*pos)+2)] < list[((2*pos)+1)]) ? (((2*pos)+1)) : (((2*pos)+2)) );
		}
		largestChild = ((2*pos)+1);
	}

	while (largestChild!=pos && list[pos]<list[largestChild])
	{
		idSwap(list[largestChild], list[pos]);
		pos = largestChild;

		largestChild = pos;
		if (((2*pos)+1)<num)
		{
			if (((2*pos)+2)<num)
			{
				largestChild = ( (list[((2*pos)+2)] < list[((2*pos)+1)]) ? (((2*pos)+1)) : (((2*pos)+2)) );
			}
			largestChild = ((2*pos)+1);
		}
	}
}

// ---------------------------------------------------------------------------------------------
// threads/AutoCrit.h - critical sections for Raven's threaded builds; on the PC they compile out
// ---------------------------------------------------------------------------------------------
template <typename t>
class AutoCrit {
public:
	AutoCrit() { }
	~AutoCrit() { }
};

template <typename t>
class ConditionalAutoCrit {
public:
	ConditionalAutoCrit() { }
	~ConditionalAutoCrit() { }
};

class ThreadedAlloc { };		// the tag of the allocation critical section

// ---------------------------------------------------------------------------------------------
// sys/sys_public.h - source location strings
// ---------------------------------------------------------------------------------------------
#define STRINGIZE( x )						#x
#define STRINGIZE_INDIRECT( F, X )			F( X )
#define __LINESTR__							STRINGIZE_INDIRECT( STRINGIZE, __LINE__ )
#define __FILELINEFUNC__					( __FILE__ " " __LINESTR__ " " __FUNCTION__ )
#define __FUNCLINE__						( __FUNCTION__ " " __LINESTR__ )

// ---------------------------------------------------------------------------------------------
// rvRandom (math/Math.h, Math.cpp) - jscott: fast and reliable random routines
// ---------------------------------------------------------------------------------------------
class rvRandom {
private:
	// DUDE: unsigned int, not Raven's unsigned long - 32 bits on its 32-bit build, 64 here,
	// where the generator's "mSeed >> 17" would leave the 0..32767 range it is written for
	static	unsigned int	mSeed;
public:
							rvRandom( void ) { mSeed = 0x89abcdef; }

	// for a non seed based init
	static	int				Init( void );

	// Init the seed to a unique number
	static	void			Init( unsigned long seed ) { mSeed = (unsigned int)seed; }

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
	return idMath::Sin( DEG2RAD( idMath::MidPointLerp( 0.0f, 60.0f, 90.0f, frac ) ) );
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


// ---------------------------------------------------------------------------------------------
// rvBits, rvPool, rvIndexPool (containers/Queue.h)
// ---------------------------------------------------------------------------------------------
/*
===============================================================================

	Raven Bit Vector Template

===============================================================================
*/
template<int CAPACITY>
class rvBits
{
	enum
	{
		BITS_SHIFT		= 5,										// 5.  Such A Nice Number
		BITS_INT_SIZE	= 32,										// Size Of A Single Word
		BITS_AND		= (BITS_INT_SIZE - 1),						// Used For And Operation
		ARRAY_SIZE		= ((CAPACITY + BITS_AND)/(BITS_INT_SIZE)),	// Num Words Used
		DATA_BYTE_SIZE	= (ARRAY_SIZE*sizeof(unsigned int)),		// Num Bytes Used
	};

	unsigned int	mData[ARRAY_SIZE];

public:
	rvBits()									{clear();}
	void	clear()								{memset(mData, 0, (size_t)(ARRAY_SIZE * sizeof( unsigned int )));}
	bool	valid(const int i)					{return (i>=0 && i<CAPACITY);}
	void	unset(const int i)					{assert(valid(i));			mData[i>>BITS_SHIFT] &= ~(1<<(i&BITS_AND));}
	void	set(const int i)					{assert(valid(i));			mData[i>>BITS_SHIFT] |=  (1<<(i&BITS_AND));}
	bool	operator[](const int i)				{assert(valid(i));	return (mData[i>>BITS_SHIFT] &   (1<<(i&BITS_AND)))!=0;}
};

/*
===============================================================================

	Raven Pool

===============================================================================
*/
template<class TYPE, int CAPACITY>
class rvPool
{
	TYPE			mData[CAPACITY];
	TYPE*			mFree[CAPACITY];
	int				mSize;

public:
	rvPool()									{clear();}
	int		size()								{return mSize;}
	bool	empty()								{return mSize==0;}
	bool	full()								{return mSize>=CAPACITY;}
	void	clear()								{mSize=0; for (int i=0; i<CAPACITY; i++)				mFree[i]=&mData[i];}
	void	free(TYPE* t)						{assert(!empty());	mSize--;							mFree[mSize]=t;}
	TYPE*	alloc()								{assert(!full());	mSize++;							return mFree[mSize-1];}
};


/*
===============================================================================

	Raven Index Pool

===============================================================================
*/
template<class TYPE, int CAPACITY, int INDEXNUM>
class rvIndexPool
{
	TYPE			mData[CAPACITY];
	TYPE*			mFree[CAPACITY];
	TYPE*			mIndx[INDEXNUM];
	int				mSize;

public:
	rvIndexPool()								{clear();}
	int		size()								{return mSize;}
	bool	empty()								{return mSize==0;}
	bool	full()								{return mSize>=CAPACITY;}
	void	clear()								{mSize=0; for (int i=0; i<CAPACITY; i++) mFree[i]=&mData[i]; memset(mIndx, 0, sizeof(mIndx));}
	bool	valid(int i)						{return (i>=0 && i<INDEXNUM && mIndx[i]);}
	TYPE*	operator[](int i)					{assert(valid(i));	return mIndx[i];}
	TYPE*	alloc(int i)						{assert(!full());	mSize++;	mIndx[i]=mFree[mSize-1];	return mFree[mSize-1];}
	void	free(int i)							{assert(valid(i));	mSize--;	mFree[mSize]=mIndx[i];		mIndx[i]=NULL;}
};

// ---------------------------------------------------------------------------------------------
// idMsgQueue, idBitMsgQueue (BitMsg.h, BitMsg.cpp) - the game's message queues; DUDE's engine has
// its own idMsgQueue (framework/async/MsgChannel.h), which the game never includes and whose
// symbols the engine does not export
// ---------------------------------------------------------------------------------------------
#define MAX_MSG_QUEUE_SIZE				16384		// must be a power of 2

class idMsgQueue {
public:
					idMsgQueue();

	void			Init( int sequence );

	bool			Add( const byte *data, const int size, bool sequencing );
	// prepend to a message without the need to a memcopy and readjust
	bool			AddConcat( const byte *data1, const int size1, const byte *data2, const int size2, bool sequencing );
	bool			Get( byte *data, int dataSize, int &size, bool sequencing );
	int				GetTotalSize( void ) const;
	int				GetSpaceLeft( void ) const;
	int				GetFirst( void ) const { return first; }
	int				GetLast( void ) const { return last; }
	void			CopyToBuffer( byte *buf ) const;

	void			WriteTo( idBitMsg &msg );
	void			FlushTo( idBitMsg &msg );
	void			ReadFrom( const idBitMsg &msg );

	void			Save( idFile *file ) const;
	void			Restore( idFile *file );

private:
	byte			buffer[MAX_MSG_QUEUE_SIZE];
	int				first;			// sequence number of first message in queue
	int				last;			// sequence number of last message in queue
	int				startIndex;		// index pointing to the first byte of the first message
	int				endIndex;		// index pointing to the first byte after the last message

public:
	void			WriteByte( byte b );
	byte			ReadByte( void );
	void			WriteShort( int s );
	void			WriteUShort( int s );
	int				ReadShort( void );
	int				ReadUShort( void );
	void			WriteLong( int l );
	int				ReadLong( void );
	void			WriteData( const byte *data, const int size );
	void			ReadData( byte *data, const int size );
};

class idBitMsgQueue {
public:
					idBitMsgQueue();

	void			Init( void );

	void			Add( const idBitMsg &msg, const int timestamp );
	bool			Get( idBitMsg &msg, int &timestamp );
	bool			Get( idBitMsg &msg ) { int dummy; return Get( msg, dummy); }
	bool			GetTimestamp( int &timestamp );

private:
	int				nextTimestamp;
	bool			readTimestamp;
	idLinkList<idMsgQueue>	writeList, readList;
};

/*
===============================================================================

	DUDE: the game's asserts report and carry on. Retail Quake 4 shipped with asserts compiled
	out, and Raven's code trips some of them on paths that work (a savegame's objects created
	before the entities they attach to); idlib's AssertFailed stops the process. In a debug build
	each failing assert is logged once per site - the list to work through - and the game goes
	on as retail did. Only the game's own code uses this; DUDE's idlib and the engine keep theirs.

===============================================================================
*/

#ifdef _DEBUG
void rvAssertFailed( const char *file, int line, const char *expression );
#undef assert
#define assert( X )			(X) ? 1 : (rvAssertFailed( __FILE__, __LINE__, #X ), 0)
#endif

#endif /* !__RAVEN_IDLIB_H__ */
