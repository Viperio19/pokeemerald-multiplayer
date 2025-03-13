LinkMultiBattle_EventScript_TryEnterLinkMultisChallenge::
	setvar VAR_FRONTIER_BATTLE_MODE, NOT_FRONTIER_MODE_LINK_MULTIS
	frontier_set FRONTIER_DATA_LVL_MODE, FRONTIER_LVL_OPEN
	fadescreen FADE_TO_BLACK
	setvar VAR_0x8004, FRONTIER_LVL_OPEN
	setvar VAR_0x8005, MULTI_PARTY_SIZE
	special ChooseHalfPartyForBattle
	waitstate
	goto_if_eq VAR_RESULT, 0, EventScript_DoTrainerBattle
	specialvar VAR_RESULT, IsWirelessAdapterConnected
	goto_if_eq VAR_RESULT, TRUE, LinkMultiBattle_EventScript_TryWirelessLink
	goto LinkMultiBattle_EventScript_TryCableLink

LinkMultiBattle_EventScript_TryCableLink::
	frontier_set FRONTIER_DATA_SELECTED_MON_ORDER
	setvar VAR_TEMP_CHALLENGE_STATUS, 0
	tower_init
	frontier_set FRONTIER_DATA_PAUSED, FALSE
	special LoadPlayerParty
	setvar VAR_0x8004, USING_BATTLE_TOWER
	message gText_PleaseWaitForLink
	waitmessage
	setvar VAR_0x8005, 0
	special TryBattleLinkup
	waitstate
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, LinkMultiBattle_EventScript_CableLinkSuccessful
	goto_if_eq VAR_RESULT, LINKUP_SOMEONE_NOT_READY, EventScript_DoTrainerBattle
	goto_if_eq VAR_RESULT, LINKUP_DIFF_SELECTIONS, EventScript_DoTrainerBattle
	goto_if_eq VAR_RESULT, LINKUP_WRONG_NUM_PLAYERS, EventScript_DoTrainerBattle
	goto_if_eq VAR_RESULT, LINKUP_FAILED, EventScript_DoTrainerBattle
	goto_if_eq VAR_RESULT, LINKUP_CONNECTION_ERROR, EventScript_DoTrainerBattle
	goto_if_eq VAR_RESULT, LINKUP_FAILED_BATTLE_TOWER, EventScript_DoTrainerBattle
	end

LinkMultiBattle_EventScript_CableLinkSuccessful::
	setvar VAR_BRAVO_TRAINER_BATTLE_TOWER_ON, FALSE
	@ message BattleFrontier_BattleTowerLobby_Text_SaveGameBeforeShowingIn
	@ waitmessage
	@ tower_save CHALLENGE_STATUS_SAVING
	@ special SaveForBattleTowerLink
	@ waitstate
	@ playse SE_SAVE
	@ waitse
	special TrySetBattleTowerLinkType
	goto LinkMultiBattle_EventScript_PrepareParty
	end

LinkMultiBattle_EventScript_PrepareParty::
	special SavePlayerParty
	setvar VAR_0x8004, FRONTIER_UTIL_FUNC_SET_PARTY_ORDER
	setvar VAR_0x8005, MULTI_PARTY_SIZE
	special CallFrontierUtilFunc
	setvar VAR_RESULT, 0
LinkMultiBattle_EventScript_WaitForLinkOpponentLoad::
	tower_loadvaropponents
	delay 1
	goto_if_ne VAR_RESULT, 6, LinkMultiBattle_EventScript_WaitForLinkOpponentLoad
	clearflag FLAG_CANCEL_BATTLE_ROOM_CHALLENGE
	setvar VAR_TEMP_CHALLENGE_STATUS, 0
	goto LinkMultiBattle_EventScript_DoTowerBattle
	end

LinkMultiBattle_EventScript_EnterRoom::
	setvar VAR_TEMP_0, 1
	frontier_get FRONTIER_DATA_BATTLE_NUM
	goto LinkMultiBattle_EventScript_DoTowerBattle

LinkMultiBattle_EventScript_DoTowerBattle::
	setvar VAR_TEMP_0, 1
	frontier_get FRONTIER_DATA_BATTLE_NUM
	setvar VAR_TEMP_2, 0
	frontier_set FRONTIER_DATA_RECORD_DISABLED, FALSE
	setvar VAR_0x8004, SPECIAL_BATTLE_TOWER
	setvar VAR_0x8005, 0
	setflag FLAG_NO_REMATCH_AFTER_LINK_BATTLE
	special DoLinkMultiBattle
	waitstate
	tower_closelink
	special CloseLink
	special LoadPlayerPartyAfterLinkMultiBattle
	special SavePlayerParty
	return

LinkMultiBattle_EventScript_TryWirelessLink::
	setvar VAR_0x8004, LINK_GROUP_BATTLE_TOWER
	goto LinkMultiBattle_EventScript_ChooseLeader
	end

LinkMultiBattle_EventScript_ChooseLeader::
	message CableClub_Text_ChooseGroupLeaderOfTwo
	waitmessage
	multichoice 16, 6, MULTI_LINK_LEADER, FALSE
	switch VAR_RESULT
	case 0, LinkMultiBattle_EventScript_TryJoinGroup
	case 1, LinkMultiBattle_EventScript_TryBecomeLeader
	case 2, EventScript_DoTrainerBattle
	case MULTI_B_PRESSED, EventScript_DoTrainerBattle
	end

LinkMultiBattle_EventScript_TryBecomeLeader::
	call CableClub_EventScript_TryBecomeLinkLeader
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, LinkMultiBattle_EventScript_WirelessLinkSuccessful
	goto_if_eq VAR_RESULT, LINKUP_FAILED, LinkMultiBattle_EventScript_ChooseLeader
	goto_if_eq VAR_RESULT, LINKUP_RETRY_ROLE_ASSIGN, LinkMultiBattle_EventScript_TryBecomeLeader
	goto_if_eq VAR_RESULT, LINKUP_FAILED_BATTLE_TOWER, EventScript_DoTrainerBattle
	release
	return

LinkMultiBattle_EventScript_TryJoinGroup::
	call CableClub_EventScript_TryJoinLinkGroup
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, LinkMultiBattle_EventScript_WirelessLinkSuccessful
	goto_if_eq VAR_RESULT, LINKUP_FAILED, LinkMultiBattle_EventScript_ChooseLeader
	goto_if_eq VAR_RESULT, LINKUP_RETRY_ROLE_ASSIGN, LinkMultiBattle_EventScript_TryJoinGroup
	goto_if_eq VAR_RESULT, LINKUP_FAILED_BATTLE_TOWER, EventScript_DoTrainerBattle
	release
	return

LinkMultiBattle_EventScript_WirelessLinkSuccessful::
	setvar VAR_BRAVO_TRAINER_BATTLE_TOWER_ON, FALSE
	goto LinkMultiBattle_EventScript_PrepareParty
	end
