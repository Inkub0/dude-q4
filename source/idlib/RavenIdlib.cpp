// dude-quake4-game: the definitions behind RavenIdlib.h, as Raven's idlib had them.

#include "precompiled.h"

// ================================================================================================
// jscott: fast and reliable random routines (math/Math.cpp)
// ================================================================================================

unsigned long rvRandom::mSeed;

float rvRandom::flrand( float min, float max )
{
	float	result;

	mSeed = ( mSeed * 214013L ) + 2531011;
	// Note: the shift and divide cannot be combined as this breaks the routine
	result = ( float )( mSeed >> 17 );						// 0 - 32767 range
	result = ( ( result * ( max - min ) ) * ( 1.0f / 32768.0f ) ) + min;
	return( result );
}

float rvRandom::flrand() {
	return flrand( 0.0f, 1.0f );
}

float rvRandom::flrand( const idVec2& v ) {
	return flrand( v[0], v[1] );
}

int rvRandom::irand( int min, int max )
{
	int		result;

	max++;
	mSeed = ( mSeed * 214013L ) + 2531011;
	result = mSeed >> 17;
	result = ( ( result * ( max - min ) ) >> 15 ) + min;
	return( result );
}

// Try to get a seed independent of the random number system

int rvRandom::Init( void )
{
	mSeed *= ( unsigned long )sys->Milliseconds();

	return( mSeed );
}
