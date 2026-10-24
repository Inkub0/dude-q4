// dude-quake4-game: the definitions behind RavenIdlib.h, as Raven's idlib had them.

#include "precompiled.h"

// ================================================================================================
// math/Vector.cpp
// ================================================================================================

idVec4 vec4_one( 1.0f, 1.0f, 1.0f, 1.0f );

// ================================================================================================
// jscott: fast and reliable random routines (math/Math.cpp)
// ================================================================================================

unsigned int rvRandom::mSeed;

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
	mSeed *= ( unsigned int )sys->GetMilliseconds();		// DUDE: idSys's name for Raven's sys->Milliseconds()

	return( mSeed );
}

// ================================================================================================
// string escape codes and Split (Str.cpp), as rvStr:: statics
// ================================================================================================

/*
================
rvStr::IsEscape
================
*/
int rvStr::IsEscape( const char *s, int* type )  {
	if ( !s || *s != C_COLOR_ESCAPE || *(s+1) == C_COLOR_ESCAPE ) {
		return 0;
	}
	if ( type ) {
		*type = S_ESCAPE_UNKNOWN;
	}
	switch ( *(s+1) ) {
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		case ':':
			if ( type ) {
				*type = S_ESCAPE_COLORINDEX;
			}
			return 2;

		case '-': case '+':
			if ( type ) {
				*type = S_ESCAPE_COLOR;
			}
			return 2;
			
		case 'r': case 'R': 
			if ( type ) {			
				*type = S_ESCAPE_COMMAND;
			}
			return 2;
			
		case 'c': case 'C':
			if ( *(s+2) ) {
				if ( *(s+3) ) {
					if ( *(s+4) ) {
						if ( type ) {
							*type = S_ESCAPE_COLOR;
						}
						return 5;
					}
				}
			}
			return 0;
			
		case 'n': case 'N':
			if ( type ) {
				*type = S_ESCAPE_COMMAND;
			}
			if ( *(s+2) ) {
				return 3;
			}
			return 0;
			
		case 'i': case 'I':
			if ( *(s+2) && *(s+3) && *(s+4) ) {
				if ( type ) {
					*type = S_ESCAPE_ICON;
				}
				return 5;
			}
			return 0;		
	}			
	return 0;
}

/*
================
rvStr::LengthWithoutEscapes
================
*/
int rvStr::LengthWithoutEscapes( const char *s ) {
	int len;
	const char *p;

	if ( !s ) {
		return 0;
	}

	len = 0;
	p = s;
	while( *p ) {
		int esc;
		esc = rvStr::IsEscape ( p );
		if ( esc ) {
			p += esc;
			continue;
		}
		p++;
		len++;
	}

	return len;
}

/*
================
rvStr::IcmpNoEscape
================
*/
// RAVEN BEGIN
// bdube: escape codes
int rvStr::IcmpNoEscape ( const char *s1, const char *s2 ) {
	int c1, c2, d;

	do {
		for ( d = rvStr::IsEscape( s1 ); d; d = rvStr::IsEscape( s1 ) ) {
			s1 += d;
		}
		for ( d = rvStr::IsEscape( s2 ); d; d = rvStr::IsEscape( s2 ) ) {
			s2 += d;
		}
// RAVEN END
		c1 = *s1++;
		c2 = *s2++;

		d = c1 - c2;
		while( d ) {
			if ( c1 <= 'Z' && c1 >= 'A' ) {
				d += ('a' - 'A');
				if ( !d ) {
					break;
				}
			}
			if ( c2 <= 'Z' && c2 >= 'A' ) {
				d -= ('a' - 'A');
				if ( !d ) {
					break;
				}
			}
			return ( INTSIGNBITNOTSET( d ) << 1 ) - 1;
		}
	} while( c1 );

	return 0;		// strings are equal
}

/*
================
rvStr::RemoveEscapes
================
*/
char *rvStr::RemoveEscapes( char *string, int escapes ) {
	char *d;
	char *s;
	int c;

	s = string;
	d = string;
	while( (c = *s) != 0 ) {
		int esc;
		int type;
		esc = rvStr::IsEscape( s, &type );
		if ( esc && (type & escapes) ) {
			s += esc;
			continue;
		}		
		else {
			*d++ = c;
			if ( c == C_COLOR_ESCAPE && *(s+1) ) {
				s++;
			}
		}
		s++;
	}
	*d = '\0';

	return string;
}

/*
================
rvStr::Split
================
*/
void rvStr::Split( const char* source, idList<idStr>& list, const char delimiter, const char groupDelimiter  ) {
	const idStr localSource( source );
	int sourceLength = localSource.Length();
	idStr element;
	int startIndex = 0;
	int endIndex = -1;
	char currentChar = '\0';

	list.Clear();
	while( startIndex < sourceLength ) {
		currentChar = localSource[ startIndex ];
		if( currentChar == groupDelimiter ) {
			endIndex = localSource.Find( groupDelimiter, ++startIndex );
			if( endIndex == -1 ) {
				common->Error( "Couldn't find expected char %c in idStr::Split\n", groupDelimiter );
			}
			element = localSource.Mid( startIndex, endIndex );
			element.Strip( groupDelimiter );
			list.Append( element );
			element.Clear();
			startIndex = endIndex + 1;
			continue;
		} else if( currentChar == delimiter ) {
			element += '\0';
			list.Append( element );
			element.Clear();
			endIndex = ++startIndex;
			continue;
		}

		startIndex++;
		element += currentChar;
	}

	if( element.Length() ) {
		element += '\0';
		list.Append( element );
	}
}

// ================================================================================================
// idMsgQueue, idBitMsgQueue (BitMsg.cpp)
// ================================================================================================

/*
===============
idMsgQueue::idMsgQueue
===============
*/
idMsgQueue::idMsgQueue( void ) {
	Init( 0 );
}

/*
===============
idMsgQueue::Init
===============
*/
void idMsgQueue::Init( int sequence ) {
	first = last = sequence;
	startIndex = endIndex = 0;
}

/*
===============
idMsgQueue::Add
===============
*/
bool idMsgQueue::Add( const byte *data, const int size, bool sequencing ) {
	if ( GetSpaceLeft() < size + 8 ) {
		return false;
	}

	assert( size );

	WriteUShort( size );
	if ( sequencing ) {
		WriteLong( last );
	}
	last++;
	WriteData( data, size );
	return true;
}

/*
===============
idMsgQueue::AddConcat
===============
*/
bool idMsgQueue::AddConcat( const byte *data1, const int size1, const byte *data2, const int size2, bool sequencing ) {
	if ( GetSpaceLeft() < size1 + size2 + 8 ) {
		return false;
	}

	assert( size1 && size2 );
	
	WriteUShort( size1 + size2 );
	if ( sequencing ) {
		WriteLong( last );
	}
	last++;
	WriteData( data1, size1 );
	WriteData( data2, size2 );
	return true;
}

/*
===============
idMsgQueue::Get
===============
*/
bool idMsgQueue::Get( byte *data, int dataSize, int &size, bool sequencing ) {
	if ( sequencing ? ( first == last ) : ( startIndex == endIndex ) ) {
		size = 0;
		return false;
	}
	int sequence;
	size = ReadUShort();
	if ( data && size > dataSize ) {
		common->Error( "idMsgQueue::Get  buffer size of %d < get size of %d", dataSize, size );
	}

	if ( sequencing ) {
		sequence = ReadLong();
		assert( sequence == first );
	}
	ReadData( data, size );
	first++;
	return true;
}

/*
===============
idMsgQueue::GetTotalSize
===============
*/
int idMsgQueue::GetTotalSize( void ) const {
	if ( startIndex <= endIndex ) {
		return ( endIndex - startIndex );
	} else {
		return ( sizeof( buffer ) - startIndex + endIndex );
	}
}

/*
===============
idMsgQueue::GetSpaceLeft
===============
*/
int idMsgQueue::GetSpaceLeft( void ) const {
	if ( startIndex <= endIndex ) {
		return sizeof( buffer ) - ( endIndex - startIndex ) - 1;
	} else {
		return ( startIndex - endIndex ) - 1;
	}
}

/*
===============
idMsgQueue::CopyToBuffer
===============
*/
void idMsgQueue::CopyToBuffer( byte *buf ) const {
	if ( startIndex <= endIndex ) {
// RAVEN BEGIN
// JSinger: Changed to call optimized memcpy
		SIMDProcessor->Memcpy( buf, buffer + startIndex, endIndex - startIndex );
// RAVEN END
	} else {
// RAVEN BEGIN
// JSinger: Changed to call optimized memcpy
		SIMDProcessor->Memcpy( buf, buffer + startIndex, sizeof( buffer ) - startIndex );
		SIMDProcessor->Memcpy( buf + sizeof( buffer ) - startIndex, buffer, endIndex );
// RAVEN END
	}
}

/*
===============
idMsgQueue::WriteByte
===============
*/
void idMsgQueue::WriteByte( byte b ) {
	buffer[endIndex] = b;
	endIndex = ( endIndex + 1 ) & ( MAX_MSG_QUEUE_SIZE - 1 );
}

/*
===============
idMsgQueue::ReadByte
===============
*/
byte idMsgQueue::ReadByte( void ) {
	byte b = buffer[startIndex];
	startIndex = ( startIndex + 1 ) & ( MAX_MSG_QUEUE_SIZE - 1 );
	return b;
}

/*
===============
idMsgQueue::WriteShort
===============
*/
void idMsgQueue::WriteShort( int s ) {
	WriteByte( ( s >>  0 ) & 255 );
	WriteByte( ( s >>  8 ) & 255 );
}

/*
===============
idMsgQueue::WriteUShort
===============
*/
void idMsgQueue::WriteUShort( int s ) {
	WriteByte( ( s >>  0 ) & 255 );
	WriteByte( ( s >>  8 ) & 255 );
}

/*
===============
idMsgQueue::ReadShort
===============
*/
int idMsgQueue::ReadShort( void ) {
// RAVEN BEGIN
// ddynerman: removed side-effecting bitwise or
	byte l = ReadByte();
	byte h = ReadByte();
	return (short)(l | ( h << 8 )); // sign extend
// RAVEN LOW
}

/*
===============
idMsgQueue::ReadUShort
===============
*/
int idMsgQueue::ReadUShort( void ) {
// RAVEN BEGIN
// ddynerman: removed side-effecting bitwise or
	byte l = ReadByte();
	byte h = ReadByte();
	return l | ( h << 8 );
// RAVEN LOW
}

/*
===============
idMsgQueue::WriteLong
===============
*/
void idMsgQueue::WriteLong( int l ) {
	WriteByte( ( l >>  0 ) & 255 );
	WriteByte( ( l >>  8 ) & 255 );
	WriteByte( ( l >> 16 ) & 255 );
	WriteByte( ( l >> 24 ) & 255 );
}

/*
===============
idMsgQueue::ReadLong
===============
*/
int idMsgQueue::ReadLong( void ) {
// RAVEN BEGIN
// ddynerman: removed side-effecting bitwise or
	byte ll = ReadByte();
	byte lh = ReadByte();
	byte hl = ReadByte();
	byte hh = ReadByte();
	return ll | ( lh << 8 ) | ( hl << 16 ) | ( hh << 24 );
// RAVEN END
}

/*
===============
idMsgQueue::WriteData
===============
*/
void idMsgQueue::WriteData( const byte *data, const int size ) {
	for ( int i = 0; i < size; i++ ) {
		WriteByte( data[i] );
	}
}

/*
===============
idMsgQueue::ReadData
===============
*/
void idMsgQueue::ReadData( byte *data, const int size ) {
	if ( data ) {
		for ( int i = 0; i < size; i++ ) {
			data[i] = ReadByte();
		}
	} else {
		for ( int i = 0; i < size; i++ ) {
			ReadByte();
		}
	}
}

/*
===============
idMsgQueue::WriteTo
===============
*/
void idMsgQueue::WriteTo( idBitMsg &msg ) {
	msg.WriteUShort( GetTotalSize() );
	assert( startIndex == 0 );
	msg.WriteData( buffer + startIndex, endIndex - startIndex );
}

/*
===============
idMsgQueue::FlushTo
===============
*/
void idMsgQueue::FlushTo( idBitMsg &msg ) {
	WriteTo( msg );
	Init( 0 );
}

/*
===============
idMsgQueue::ReadFrom
===============
*/
void idMsgQueue::ReadFrom( const idBitMsg &msg ) {
	Init( 0 );
	endIndex = msg.ReadUShort();
	msg.ReadData( buffer, endIndex );
}

/*
===============
idMsgQueue::Save
===============
*/
void idMsgQueue::Save( idFile *file ) const {
	file->WriteInt( first );
	file->WriteInt( last );
	file->WriteInt( startIndex );
	file->WriteInt( endIndex );
	file->Write( buffer + startIndex, endIndex - startIndex );
}
/*
===============
idMsgQueue::Restore
===============
*/
void idMsgQueue::Restore( idFile *file ) {
	file->ReadInt( first );
	file->ReadInt( last );
	file->ReadInt( startIndex );
	file->ReadInt( endIndex );
	file->Read( buffer + startIndex, endIndex - startIndex );
}

/*
===============
idBitMsgQueue::idBitMsgQueue
===============
*/
idBitMsgQueue::idBitMsgQueue( void ) {
	Init();
}

/*
===============
idBitMsgQueue::Init
===============
*/
void idBitMsgQueue::Init( void ) {
	readTimestamp = false;
	writeList.Clear();
	readList.Clear();
}

/*
===============
idBitMsgQueue::Add
===============
*/
void idBitMsgQueue::Add( const idBitMsg &msg, const int timestamp ) {
	while ( writeList.NextNode() && writeList.Next()->GetSpaceLeft() < ( msg.GetSize() + 8+4 ) ) {
		writeList.NextNode()->AddToEnd( readList );
	}

	if ( !writeList.NextNode() ) {
		idLinkList< idMsgQueue > *node = new idLinkList< idMsgQueue >;
		node->SetOwner( new idMsgQueue );
		node->AddToEnd( writeList );
	}

	writeList.Next()->WriteLong( timestamp );
	if ( !writeList.Next()->Add( msg.GetData(), msg.GetSize(), false ) ) {
		assert( false );
	}
}

/*
===============
idBitMsgQueue::Get
===============
*/
bool idBitMsgQueue::Get( idBitMsg &msg, int &timestamp ) {
	while ( readList.NextNode() && !readList.Next()->GetTotalSize() ) {
		readList.Next()->Init( 0 );
		readList.NextNode()->AddToEnd( writeList );
	}

	if ( readList.NextNode() ) {
		int size;

		timestamp = readTimestamp ? nextTimestamp : readList.Next()->ReadLong();
		readTimestamp = true;
		if ( readList.Next()->Get( msg.GetData(), msg.GetMaxSize(), size, false ) ) {
			msg.SetSize( size );
			msg.BeginReading();
			readTimestamp = false;
			return true;
		}

		assert( false );
	} else if ( writeList.NextNode() && writeList.Next()->GetTotalSize() ) {
		int size;

		timestamp = readTimestamp ? nextTimestamp : writeList.Next()->ReadLong();
		readTimestamp = true;
		if ( writeList.Next()->Get( msg.GetData(), msg.GetMaxSize(), size, false ) ) {
			msg.SetSize( size );
			msg.BeginReading();
			readTimestamp = false;
			return true;
		}

		assert( false );
	}

	msg.SetSize( 0 );
	return false;
}

/*
===============
idBitMsgQueue::GetTimestamp
===============
*/
bool idBitMsgQueue::GetTimestamp( int &timestamp ) {
	if ( readTimestamp ) {
		timestamp = nextTimestamp;
		return true;
	}

	while ( readList.NextNode() && !readList.Next()->GetTotalSize() ) {
		readList.Next()->Init( 0 );
		readList.NextNode()->AddToEnd( writeList );
	}

	if ( readList.NextNode() ) {
		timestamp = nextTimestamp = readList.Next()->ReadLong();
		readTimestamp = true;
		return true;
	} else if ( writeList.NextNode() && writeList.Next()->GetTotalSize() ) {
		timestamp = nextTimestamp = writeList.Next()->ReadLong();
		readTimestamp = true;
		return true;
	}

	return false;
}

/*
================
rvAssertFailed

Once per assert site (file and line), then silent. The file names are string literals, one per
source file, so the pointer identifies the site with the line.
================
*/
#ifdef _DEBUG
void rvAssertFailed( const char *file, int line, const char *expression ) {
	static const int MAX_SITES = 512;
	static const char *	siteFiles[MAX_SITES];
	static int			siteLines[MAX_SITES];
	static int			numSites = 0;

	for ( int i = 0; i < numSites; i++ ) {
		if ( siteLines[i] == line && siteFiles[i] == file ) {
			return;
		}
	}
	if ( numSites < MAX_SITES ) {
		siteFiles[numSites] = file;
		siteLines[numSites] = line;
		numSites++;
	}
	if ( idLib::common != NULL ) {
		idLib::common->Warning( "ASSERTION FAILED (continuing): %s(%d): '%s'", file, line, expression );
	} else {
		fprintf( stderr, "ASSERTION FAILED (continuing): %s(%d): '%s'\n", file, line, expression );
	}
}
#endif
