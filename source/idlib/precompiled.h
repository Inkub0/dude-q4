// dude-quake4-game: this replaces Raven's precompiled.h (the original is in the first commit).
//
// Raven's version pulled in the SDK's copies of the Quake 4 engine's interfaces. This game
// library runs on DUDE, so every engine header comes from the DUDE checkout instead (the build
// puts <DUDE>/neo first on the include path): the types the engine and the game hand each other
// - idStr, idDict, idList, idVec3, renderEntity_t, trace_t, usercmd_t ... - must have DUDE's
// layout. What Raven's game code needs on top of that comes from DUDE's own Quake 4 headers or
// from adaptations in this repository, never from the SDK's engine headers.

#ifndef __PRECOMPILED_H__
#define __PRECOMPILED_H__

#ifdef __cplusplus

// Raven's build switches, as its Linux build set them (precompiled.h, __linux__ branch)
#define PC_CVAR_ARCHIVE			CVAR_ARCHIVE
#define _OPENGL
#ifndef _LITTLE_ENDIAN
#define _LITTLE_ENDIAN
#endif
#define _CASE_SENSITIVE_FILESYSTEM
#define _USE_OPENAL
#define _GLVAS_SUPPPORT
#define NEWLINE					"\n"
#define RESTRICT
#define TIME_THIS_SCOPE(x)
#undef _XBOX
#undef _CONSOLE

class AlignmentChecker {
public:
	static void UpdateCount( void const * const ptr ) {}
	static void ClearCount() {}
	static void Print() {}
};

#include <stddef.h>
#include <limits.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <typeinfo>
#include <errno.h>
#include <math.h>

// DUDE: platform, system services
#include "sys/platform.h"
#include "sys/sys_public.h"

// DUDE's idlib, in the order id's original Lib.h included it
#include "idlib/Lib.h"
#include "idlib/Heap.h"
#include "idlib/containers/List.h"
#include "idlib/math/Simd.h"
#include "idlib/math/Math.h"
#include "idlib/math/Random.h"
#include "idlib/math/Complex.h"
#include "idlib/math/Vector.h"
#include "idlib/math/Matrix.h"
#include "idlib/math/Angles.h"
#include "idlib/math/Quat.h"
#include "idlib/math/Rotation.h"
#include "idlib/math/Plane.h"
#include "idlib/math/Pluecker.h"
#include "idlib/math/Polynomial.h"
#include "idlib/math/Extrapolate.h"
#include "idlib/math/Interpolate.h"
#include "idlib/math/Curve.h"
#include "idlib/math/Ode.h"
#include "idlib/math/Lcp.h"
#include "idlib/bv/Sphere.h"
#include "idlib/bv/Bounds.h"
#include "idlib/bv/Box.h"
#include "idlib/bv/Frustum.h"
#include "idlib/geometry/DrawVert.h"
#include "idlib/geometry/JointTransform.h"
#include "idlib/geometry/Winding.h"
#include "idlib/geometry/Winding2D.h"
#include "idlib/geometry/Surface.h"
#include "idlib/geometry/Surface_Patch.h"
#include "idlib/geometry/Surface_Polytope.h"
#include "idlib/geometry/Surface_SweptSpline.h"
#include "idlib/geometry/TraceModel.h"
#include "idlib/Str.h"
#include "idlib/Token.h"
#include "idlib/Lexer.h"
#include "idlib/Parser.h"
#include "idlib/Base64.h"
#include "idlib/CmdArgs.h"
#include "idlib/containers/BTree.h"
#include "idlib/containers/BinSearch.h"
#include "idlib/containers/HashIndex.h"
#include "idlib/containers/HashTable.h"
#include "idlib/containers/StaticList.h"
#include "idlib/containers/LinkList.h"
#include "idlib/containers/Hierarchy.h"
#include "idlib/containers/Queue.h"
#include "idlib/containers/Stack.h"
#include "idlib/containers/StrList.h"
#include "idlib/containers/StrPool.h"
#include "idlib/containers/VectorSet.h"
#include "idlib/containers/PlaneSet.h"
#include "idlib/hashing/CRC32.h"
#include "idlib/hashing/MD4.h"
#include "idlib/hashing/MD5.h"
#include "idlib/Dict.h"
#include "idlib/LangDict.h"
#include "idlib/BitMsg.h"
#include "idlib/MapFile.h"
#include "idlib/Timer.h"

// Raven's idlib types and switches the game uses that never cross into the engine (Raven's
// AutoPtr, LexerFactory, TextCompiler and AutoCrit are not used by the game code)
#include "RavenIdlib.h"

// DUDE's engine interfaces (framework, decls, renderer, sound, networking, GUIs, collision, AAS)
#include "framework/BuildVersion.h"
#include "framework/BuildDefines.h"
#include "framework/Licensee.h"
#include "framework/CmdSystem.h"
#include "framework/CVarSystem.h"
#include "framework/Common.h"
#include "framework/File.h"
#include "framework/FileSystem.h"
#include "framework/UsercmdGen.h"
#include "framework/DeclManager.h"
#include "framework/DeclTable.h"
#include "framework/DeclSkin.h"
#include "framework/DeclEntityDef.h"
#include "framework/DeclAF.h"
#include "framework/DeclPDA.h"
#include "framework/DeclQuake4.h"
#include "renderer/Cinematic.h"
#include "renderer/Material.h"
#include "renderer/Model.h"
#include "renderer/ModelManager.h"
#include "renderer/RenderSystem.h"
#include "renderer/RenderWorld.h"
#include "sound/sound.h"
#include "framework/async/NetworkSystem.h"
#include "ui/ListGUI.h"
#include "ui/UserInterface.h"
#include "cm/CollisionModel.h"
#include "tools/compilers/aas/AASFile.h"
#include "tools/compilers/aas/AASFileManager.h"

// the engine <-> game interface is DUDE's framework/Game.h; game/Game.h includes it and keeps
// the game-side definitions Raven had there
#include "../game/Game.h"

// the game
#include "../game/Game_local.h"

#endif	/* __cplusplus */

#endif /* !__PRECOMPILED_H__ */
