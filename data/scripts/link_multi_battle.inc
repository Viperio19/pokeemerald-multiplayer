LinkMultiBattle_EventScript_TryLinkMultiBattle::
	setvar VAR_FRONTIER_BATTLE_MODE, NOT_FRONTIER_MODE_LINK_MULTIS
	fadescreen FADE_TO_BLACK
	setvar VAR_0x8004, FRONTIER_LVL_OPEN
	setvar VAR_0x8005, MULTI_PARTY_SIZE
	special ChooseHalfPartyForBattle
	waitstate
	goto_if_eq VAR_RESULT, 0, LinkMultiBattle_EventScript_DoRegularBattle
	special BattleTowerReconnectLink
	goto LinkMultiBattle_EventScript_PrepareParty

LinkMultiBattle_EventScript_PrepareParty::
	frontier_set FRONTIER_DATA_SELECTED_MON_ORDER
	special LoadPlayerParty
	special SavePlayerParty
	setvar VAR_0x8004, FRONTIER_UTIL_FUNC_SET_PARTY_ORDER
	setvar VAR_0x8005, MULTI_PARTY_SIZE
	special CallFrontierUtilFunc
	setvar VAR_RESULT, 0
	setvar VAR_0x8009, FALSE
	message gText_PleaseWaitForLink
LinkMultiBattle_EventScript_WaitForLinkOpponentLoad::
	tower_loadvaropponents
	delay 1
	goto_if_lt VAR_RESULT, 11, LinkMultiBattle_EventScript_WaitForLinkOpponentLoad
	goto_if_eq VAR_RESULT, 12, LinkMultiBattle_EventScript_DoRegularBattle
	goto_if_eq VAR_0x8009, TRUE, LinkMultiBattle_EventScript_SelectedDifferentTrainer
	goto LinkMultiBattle_EventScript_DoLinkMultiBattle
	end

LinkMultiBattle_EventScript_SelectedDifferentTrainer::
	msgbox LinkMultiBattle_Text_SelectedDifferentTrainer, MSGBOX_DEFAULT
	goto LinkMultiBattle_EventScript_DoRegularBattle

LinkMultiBattle_EventScript_DoRegularBattle::
	special LoadPlayerParty
	goto EventScript_InitiateSoloBattle

LinkMultiBattle_EventScript_DoLinkMultiBattle::
	message LinkMultiBattle_Text_InitiatingLinkBattle
	waitmessage
	special DoLinkMultiBattle
	waitstate
	special SavePlayerParty
	gotobeatenscript
	releaseall
	end

EventScript_SetUpLinkMultiBattleConnection::
	specialvar VAR_RESULT, IsLinkConnectionEstablished
	goto_if_eq VAR_RESULT, TRUE, LinkMultiBattle_EventScript_LinkIsAlreadyEstablished
	specialvar VAR_RESULT, IsWirelessAdapterConnected
	goto_if_eq VAR_RESULT, TRUE, LinkMultiBattle_EventScript_TryWirelessLink
	goto LinkMultiBattle_EventScript_TryCableLink

LinkMultiBattle_EventScript_LinkIsAlreadyEstablished::
	message LinkMultiBattle_Text_LinkIsAlreadyEstablished
	waitmessage
	delay 60
	closemessage
	releaseall
	end

LinkMultiBattle_EventScript_TryCableLink::
	message gText_PleaseWaitForLink
	waitmessage
	setvar VAR_0x8004, USING_LINK_MULTI
	setvar VAR_0x8005, 0
	special TryBattleLinkup
	waitstate
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, LinkMultiBattle_EventScript_CableLinkSuccessful
	goto_if_eq VAR_RESULT, LINKUP_SOMEONE_NOT_READY, LinkMultiBattle_EventScript_CableLinkFailed
	goto_if_eq VAR_RESULT, LINKUP_DIFF_SELECTIONS, LinkMultiBattle_EventScript_CableLinkFailed
	goto_if_eq VAR_RESULT, LINKUP_WRONG_NUM_PLAYERS, LinkMultiBattle_EventScript_CableLinkFailed
	goto_if_eq VAR_RESULT, LINKUP_FAILED, LinkMultiBattle_EventScript_CableLinkFailed
	goto_if_eq VAR_RESULT, LINKUP_CONNECTION_ERROR, LinkMultiBattle_EventScript_CableLinkFailed
	goto_if_eq VAR_RESULT, LINKUP_FAILED_BATTLE_TOWER, LinkMultiBattle_EventScript_CableLinkFailed
	end

LinkMultiBattle_EventScript_CableLinkSuccessful::
	message LinkMultiBattle_Text_LinkConnectionEstablished
	waitmessage
	delay 60
	closemessage
	releaseall
	end

LinkMultiBattle_EventScript_CableLinkFailed::
	message LinkMultiBattle_Text_LinkConnectionFailed
	waitmessage
	delay 60
	closemessage
	releaseall
	end

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
	case 2, LinkMultiBattle_EventScript_CableLinkFailed
	case MULTI_B_PRESSED, LinkMultiBattle_EventScript_CableLinkFailed
	end

LinkMultiBattle_EventScript_TryBecomeLeader::
	call CableClub_EventScript_TryBecomeLinkLeader
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, LinkMultiBattle_EventScript_WirelessLinkSuccessful
	goto_if_eq VAR_RESULT, LINKUP_FAILED, LinkMultiBattle_EventScript_ChooseLeader
	goto_if_eq VAR_RESULT, LINKUP_RETRY_ROLE_ASSIGN, LinkMultiBattle_EventScript_TryBecomeLeader
	goto_if_eq VAR_RESULT, LINKUP_FAILED_BATTLE_TOWER, LinkMultiBattle_EventScript_CableLinkFailed
	release
	return

LinkMultiBattle_EventScript_TryJoinGroup::
	call CableClub_EventScript_TryJoinLinkGroup
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, LinkMultiBattle_EventScript_WirelessLinkSuccessful
	goto_if_eq VAR_RESULT, LINKUP_FAILED, LinkMultiBattle_EventScript_ChooseLeader
	goto_if_eq VAR_RESULT, LINKUP_RETRY_ROLE_ASSIGN, LinkMultiBattle_EventScript_TryJoinGroup
	goto_if_eq VAR_RESULT, LINKUP_FAILED_BATTLE_TOWER, LinkMultiBattle_EventScript_CableLinkFailed
	release
	return

LinkMultiBattle_EventScript_WirelessLinkSuccessful::
	goto LinkMultiBattle_EventScript_CableLinkSuccessful
	end

LinkMultiBattle_Text_SelectedDifferentTrainer:
	.string "The other player selected a\n"
	.string "different trainer.$"

LinkMultiBattle_Text_InitiatingLinkBattle:
	.string "Initiating link battle.$"

LinkMultiBattle_Text_LinkIsAlreadyEstablished:
	.string "Link is already established.$"

LinkMultiBattle_Text_LinkConnectionEstablished:
	.string "Link established!$"

LinkMultiBattle_Text_LinkConnectionFailed:
	.string "Link failed.$"
