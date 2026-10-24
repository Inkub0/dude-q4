
#ifndef __LIST_GAME_H__
#define __LIST_GAME_H__

// GCC 4 compiles templates when they are encountered in a source file,
// not when they are used. Therefore all references, must be resolved.
// RemoveContents() from idLib\List.h references global variables only
// available in the GAME DLL.

// DUDE: a free function - DUDE's idList has no RemoveContents member; the game's
// list.RemoveContents( clear ) calls rvListRemoveContents( list, clear )

/*
================
idList<type>::RemoveContents
================
*/
template< class type >
ID_INLINE void rvListRemoveContents( idList<type> &list, bool clear ) {
	list.RemoveNull();

	for( int ix = list.Num() - 1; ix >= 0; --ix ) {
		list[ ix ]->PostEventMS( &EV_Remove, 0 );
		list[ ix ] = NULL;
	}

	if ( clear ) {
		list.Clear();
	} else {
		memset( list.Ptr(), 0, list.Allocated() );
	}
}

#endif // __LIST_GAME_H__
