# samp-player-gangzones

# About
Creating per player gangzones to avoid normal (global) samp gangzones limit (1024).
Uses Pawn.RakNet to send RPCs.

PS: Yes. I know test.pwn file isn't actually a unit test script. You need to go in game and use those commands. sorry!

Thanks to [@J0shES](https://github.com/J0shES) for flash functions.

[![sampctl](https://shields.southcla.ws/badge/sampctl-samp--player--gangzones-2f2f2f.svg?style=for-the-badge)](https://github.com/AmyrAhmady/samp-player-gangzones)

## Installation

Simply install to your project:

```bash
sampctl package install AmyrAhmady/samp-player-gangzones
```

Include in your code and begin using the library:

```pawn
#include <samp-player-gangzones>
```

## Functions

```pawn
Player_GangZoneCreate(playerid, Float:minx, Float:miny, Float:maxx, Float:maxy);
Player_GangZoneDestroy(playerid, gangzoneid);
Player_GangZoneShow(playerid, gangzoneid, color = PLAYER_GZ_DEFAULT_COLOR);
Player_GangZoneHide(playerid, gangzoneid);
Player_GangZoneFlash(playerid, gangzoneid, color = PLAYER_GZ_DEFAULT_FLASH_COL);
Player_GangZoneStopFlash(playerid, gangzoneid);
```

## Testing

To test, simply run the package:

```bash
sampctl package run
```

and use these commands in game:

```bash
/gzcreate
/gzshow
/gzhide
/gzdelete
```
