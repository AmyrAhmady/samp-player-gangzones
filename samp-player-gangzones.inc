#include <Pawn.RakNet>

#define MAX_PLAYER_GZ 128
#define PLAYER_GZ_DEFAULT_COLOR 0xFFFFFFAA

static enum _:E_PLAYER_GZ_DATA
{
	Float:player_GZ_MinX,
	Float:player_GZ_MinY,
	Float:player_GZ_MaxX,
	Float:player_GZ_MaxY,
	player_GZ_Color,
	bool:player_GZ_Used,
	bool:player_GZ_Shown
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

	PlayerGangZoneData[playerid][idx][player_GZ_MinX]	= minx;
	PlayerGangZoneData[playerid][idx][player_GZ_MinY]	= miny;
	PlayerGangZoneData[playerid][idx][player_GZ_MaxX]	= maxx;
	PlayerGangZoneData[playerid][idx][player_GZ_MaxY]	= maxy;
	PlayerGangZoneData[playerid][idx][player_GZ_Color]	= PLAYER_GZ_DEFAULT_COLOR;
	PlayerGangZoneData[playerid][idx][player_GZ_Used]	= true;
	PlayerGangZoneData[playerid][idx][player_GZ_Shown]	= false;

	return idx;
}

stock PlayerGangZoneShow(playerid, gangzoneid, color = PLAYER_GZ_DEFAULT_COLOR)
{
	if(!PlayerGangZoneData[playerid][gangzoneid][player_GZ_Used]) return 0;

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
		!PlayerGangZoneData[playerid][gangzoneid][player_GZ_Shown]) return 0;

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

stock PlayerGangZoneDestroy(playerid, gangzoneid)
{
	if(!PlayerGangZoneData[playerid][gangzoneid][player_GZ_Used]) return 0;
	if(PlayerGangZoneData[playerid][gangzoneid][player_GZ_Shown])
		PlayerGangZoneHide(playerid, gangzoneid);
	PlayerGangZoneData[playerid][gangzoneid][player_GZ_Used] = false;
	PlayerGangZoneData[playerid][gangzoneid][player_GZ_MinX]	= 0.0;
	PlayerGangZoneData[playerid][gangzoneid][player_GZ_MinY]	= 0.0;
	PlayerGangZoneData[playerid][gangzoneid][player_GZ_MaxX]	= 0.0;
	PlayerGangZoneData[playerid][gangzoneid][player_GZ_MaxY]	= 0.0;
	PlayerGangZoneData[playerid][gangzoneid][player_GZ_Color]	= 0;
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	for (new i = 0; i < MAX_PLAYER_GZ; ++i)
		PlayerGangZoneData[playerid][i][player_GZ_Used] = false;
	#if defined pGZ_OnPlayerDisconnect
		pGZ_OnPlayerDisconnect(playerid,  reason);
	#endif
	return 1;
}

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect pGZ_OnPlayerDisconnect