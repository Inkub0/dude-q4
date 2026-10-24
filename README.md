# dude-quake4-game

The Quake 4 game library (`q4base.so` / `q4base.dll`) for [DUDE](https://github.com/Inkub0/dude),
built from Raven Software's Quake 4 SDK 1.4.2 game code.

## Licence

Everything in this repository derives from the **Quake 4 SDK** and is under its licence,
`EULA.Development Kit.rtf` (the "QUAKE 4 SOFTWARE DEVELOPMENT KIT LIMITED USE LICENSE
AGREEMENT"): **non-commercial use only**; copies may be passed on only free of charge and with
that agreement attached. It is **not** GPL, which is why this code lives apart from DUDE
(GPLv3) and talks to it only across the game-library boundary (`GetGameAPI`). DUDE's own code is
never copied in here, and nothing from here goes into DUDE. You need your own copy of Quake 4 to
play; no game data is included.

## Layout

- `source/` - the SDK source as Raven shipped it (first commit, unmodified): `game/` (the
  single-player game), `mpgame/` (multiplayer, not built yet), `idlib/` (Raven's idlib) and the
  engine interface headers (`framework/`, `renderer/`, `sound/`, `cm/`, `aas/`, `bse/`, `ui/`,
  `sys/`), which describe the closed Quake 4 engine and are read for reference only.
- Later commits adapt the game code to build against DUDE: one change per commit, each saying
  what it changes and why.

## Building

Built by DUDE's `tools/quake4/build-q4game.sh`, against the DUDE checkout's headers and its
idlib; the result goes next to `build/dude`, where DUDE looks for it when it runs Quake 4.

## Provenance

`source/` is the SDK 1.4.2 source from the initial import of the full SDK installer in
github.com/FriskTheFallenHuman/quake4sdk (commit 0682c1b8, 2025-01-19). A second, independent
copy, github.com/Reddragoon244/Quake-4-Source-Code (de2da6b, 2015), matches it on all 718 source
files, byte for byte once line endings are normalised.

Ideas may come from other ports of this code (idTech4A++, OpenQ4); where one is used, the
commit says so. Code reconstructed by decompiling Quake 4's binaries is never copied.
