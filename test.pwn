#include <a_samp>
#include "samp-player-gangzones.inc"

new gangzone;

main()
{
	print("\n----------------------------------");
	print(" PlayerGangZone test script\n");
	print("----------------------------------\n");
}

public OnGameModeInit()
{
	SetGameModeText("test Script");
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	if (!strcmp("/gzcreate", cmdtext, true)
	{
		gangzone = PlayerGangZoneCreate(playerid, 1082.962, -2787.229, 2942.549, -1859.51);
		return 1;
	}
	if (!strcmp("/gzshow", cmdtext, true)
	{
		PlayerGangZoneShow(playerid, gangzone, 0xFF000099);
		return 1;
	}
	if (!strcmp("/gzhide", cmdtext, true)
	{
		PlayerGangZoneHide(playerid, gangzone);
		return 1;
	}
	if (!strcmp("/gzdelete", cmdtext, true)
	{
		PlayerGangZoneDestroy(playerid, gangzone);
		return 1;
	}
	return 0;
}