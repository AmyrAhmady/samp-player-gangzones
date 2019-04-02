#if !defined PAWNRAKNET_INC_
	#include <Pawn.RakNet>
#endif

#if !defined MAX_PLAYER_GZ
	#define MAX_PLAYER_GZ 256
#endif

#define PLAYER_GZ_DEFAULT_COLOR 0xFFFFFFAA
#define PLAYER_GZ_DEFAULT_FLASH_COL 0xFF0000AA

static enum _:E_PLAYER_GZ_DATA
{
	Float:player_GZ_MinX,
	Float:player_GZ_MinY,
	Float:player_GZ_MaxX,
	Float:player_GZ_MaxY,
	player_GZ_Color,
	player_GZ_Flash_Color,
	bool:player_GZ_Used,
	bool:player_GZ_Shown,
	bool:player_GZ_Flashing
}

static PlayerGangZoneData[MAX_PLAYERS][MAX_PLAYER_GZ][E_PLAYER_GZ_DATA];

stock PlayerGangZoneCreate(playerid, Float:minx, Float:miny, Float:maxx, Float:maxy)
{
	new idx;
	for (new i = 0; i < MAX_PLAYER_GZ; ++i)
	{
		if(!PlayerGangZoneData[playerid][i][player_GZ_Used]) 
		{
			idx = i;
			break;
		}
	}

	if(idx > MAX_PLAYER_GZ-1) return -1;

	PlayerGangZoneData[playerid][idx][player_GZ_MinX] = minx;
	PlayerGangZoneData[playerid][idx][player_GZ_MinY] = miny;
	PlayerGangZoneData[playerid][idx][player_GZ_MaxX] = maxx;
	PlayerGangZoneData[playerid][idx][player_GZ_MaxY] = maxy;
	PlayerGangZoneData[playerid][idx][player_GZ_Color] = PLAYER_GZ_DEFAULT_COLOR;
	PlayerGangZoneData[playerid][idx][player_GZ_Flash_Color] = PLAYER_GZ_DEFAULT_FLASH_COL;
	PlayerGangZoneData[playerid][idx][player_GZ_Used] = true;
	PlayerGangZoneData[playerid][idx][player_GZ_Shown] = false;
	PlayerGangZoneData[playerid][idx][player_GZ_Flashing] = false;

	return idx;
}

stock PlayerGangZoneShow(playerid, gangzoneid, color = PLAYER_GZ_DEFAULT_COLOR)
{
	if(!PlayerGangZoneData[playerid][gangzoneid][player_GZ_Used] || gangzoneid > MAX_PLAYER_GZ-1 || gangzoneid < 0) return 0;

	PlayerGangZoneData[playerid][gangzoneid][player_GZ_Color] = color;
	PlayerGangZoneData[playerid][gangzoneid][player_GZ_Shown] = true;
	new abgr_color = (((color << 16) | color & 0xFF00) << 8) | (((color >>> 16) | color & 0xFF0000) >>> 8);

	new BitStream:gz_bs = BS_New();
		
	BS_WriteValue(
		gz_bs,
		PR_UINT16, 1023 - gangzoneid,
		PR_FLOAT, PlayerGangZoneData[playerid][gangzoneid][player_GZ_MinX],
		PR_FLOAT, PlayerGangZoneData[playerid][gangzoneid][player_GZ_MinY],
		PR_FLOAT, PlayerGangZoneData[playerid][gangzoneid][player_GZ_MaxX],
		PR_FLOAT, PlayerGangZoneData[playerid][gangzoneid][player_GZ_MaxY],
		PR_UINT32, abgr_color
	);
		
	BS_RPC(gz_bs, playerid, 0x6C);
	BS_Delete(gz_bs);
	return 1;
}

stock PlayerGangZoneHide(playerid, gangzoneid)
{
	if(!PlayerGangZoneData[playerid][gangzoneid][player_GZ_Used] || 
		!PlayerGangZoneData[playerid][gangzoneid][player_GZ_Shown] ||
		gangzoneid > MAX_PLAYER_GZ-1 || gangzoneid < 0) return 0;

	PlayerGangZoneData[playerid][gangzoneid][player_GZ_Shown] = false;
	new BitStream:gz_bs = BS_New();
		
	BS_WriteValue(
		gz_bs,
		PR_UINT16, 1023 - gangzoneid
	);
		
	BS_RPC(gz_bs, playerid, 0x78);
	BS_Delete(gz_bs);
	return 1;
}

stock PlayerGangZoneFlash(playerid, gangzoneid, color = PLAYER_GZ_DEFAULT_FLASH_COL)
{
	if(!PlayerGangZoneData[playerid][gangzoneid][player_GZ_Used] || gangzoneid > MAX_PLAYER_GZ-1 || gangzoneid < 0) return 0;

	PlayerGangZoneData[playerid][gangzoneid][player_GZ_Flash_Color]  = color;
	PlayerGangZoneData[playerid][gangzoneid][player_GZ_Flashing] = true;
	new abgr_color = (((color << 16) | color & 0xFF00) << 8) | (((color >>> 16) | color & 0xFF0000) >>> 8);

	new BitStream:gz_bs = BS_New();
		
	BS_WriteValue(
		gz_bs,
		PR_UINT16, 1023 - gangzoneid,
		PR_UINT32, abgr_color
	);
		
	BS_RPC(gz_bs, playerid, 0x79);
	BS_Delete(gz_bs);
	return 1;
}

stock PlayerGangZoneStopFlash(playerid, gangzoneid)
{
	if(!PlayerGangZoneData[playerid][gangzoneid][player_GZ_Used] ||
		!PlayerGangZoneData[playerid][gangzoneid][player_GZ_Flashing] ||
		gangzoneid > MAX_PLAYER_GZ-1 || gangzoneid < 0) return 0;

	PlayerGangZoneData[playerid][gangzoneid][player_GZ_Flashing] = false;
	new abgr_color = (((color << 16) | color & 0xFF00) << 8) | (((color >>> 16) | color & 0xFF0000) >>> 8);

	new BitStream:gz_bs = BS_New();
		
	BS_WriteValue(
		gz_bs,
		PR_UINT16, 1023 - gangzoneid
	);
		
	BS_RPC(gz_bs, playerid, 0x55);
	BS_Delete(gz_bs);
	return 1;
}

stock PlayerGangZoneDestroy(playerid, gangzoneid)
{
	if(!PlayerGangZoneData[playerid][gangzoneid][player_GZ_Used] || gangzoneid > MAX_PLAYER_GZ-1 || gangzoneid < 0) return 0;
	if(PlayerGangZoneData[playerid][gangzoneid][player_GZ_Shown])
		PlayerGangZoneHide(playerid, gangzoneid);
	PlayerGangZoneData[playerid][gangzoneid][player_GZ_Flashing] = false;
	PlayerGangZoneData[playerid][gangzoneid][player_GZ_Used] = false;
	PlayerGangZoneData[playerid][gangzoneid][player_GZ_MinX] = 0.0;
	PlayerGangZoneData[playerid][gangzoneid][player_GZ_MinY] = 0.0;
	PlayerGangZoneData[playerid][gangzoneid][player_GZ_MaxX] = 0.0;
	PlayerGangZoneData[playerid][gangzoneid][player_GZ_MaxY] = 0.0;
	PlayerGangZoneData[playerid][gangzoneid][player_GZ_Color] = 0;
	PlayerGangZoneData[playerid][gangzoneid][player_GZ_Flash_Color] = 0;
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	for (new i = 0; i < MAX_PLAYER_GZ; ++i)
		PlayerGangZoneData[playerid][i][player_GZ_Used] = false;
	return CallLocalFunction("pGZ_OnPlayerDisconnect", "ii", playerid, reason);
}

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect pGZ_OnPlayerDisconnect
forward pGZ_OnPlayerDisconnect(playerid, reason);