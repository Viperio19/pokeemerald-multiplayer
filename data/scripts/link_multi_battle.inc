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

EventScript_OpenLinkMenu::
	delay 10
	multichoice 20, 0, MULTI_LINK_MENU, FALSE
	switch VAR_RESULT
	case 0, LinkMenu_EventScript_AskForConnection
	case 1, LinkMenu_EventScript_ToggleSoullink
	case 2, LinkMenu_EventScript_AskMixRandomizers
	case 3, LinkMenu_EventScript_CloseMenu
	case MULTI_B_PRESSED, LinkMenu_EventScript_CloseMenu
	end

LinkMenu_EventScript_AskForConnection::
	specialvar VAR_RESULT, IsLinkConnectionEstablished
	goto_if_eq VAR_RESULT, TRUE, LinkMenu_EventScript_LinkIsAlreadyEstablished
	msgbox LinkMenu_Text_DoYouWantToConnect, MSGBOX_YESNO
	switch VAR_RESULT
	case YES, LinkMenu_EventScript_SetUpConnection
	case NO, LinkMenu_EventScript_ReopenLinkMenu
	case MULTI_B_PRESSED, LinkMenu_EventScript_ReopenLinkMenu

LinkMenu_EventScript_SetUpConnection::
	specialvar VAR_RESULT, IsWirelessAdapterConnected
	goto_if_eq VAR_RESULT, TRUE, LinkMenu_EventScript_TryWirelessLink
	goto LinkMenu_EventScript_TryCableLink

LinkMenu_EventScript_LinkIsAlreadyEstablished::
	message LinkMenu_Text_LinkIsAlreadyEstablished
	waitmessage
	delay 60
	closemessage
	goto EventScript_OpenLinkMenu

LinkMenu_EventScript_TryCableLink::
	message gText_PleaseWaitForLink
	waitmessage
	setvar VAR_0x8004, USING_LINK_MULTI
	setvar VAR_0x8005, 0
	special TryBattleLinkup
	waitstate
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, LinkMenu_EventScript_CableLinkSuccessful
	goto_if_eq VAR_RESULT, LINKUP_SOMEONE_NOT_READY, LinkMenu_EventScript_CableLinkFailed
	goto_if_eq VAR_RESULT, LINKUP_DIFF_SELECTIONS, LinkMenu_EventScript_CableLinkFailed
	goto_if_eq VAR_RESULT, LINKUP_WRONG_NUM_PLAYERS, LinkMenu_EventScript_CableLinkFailed
	goto_if_eq VAR_RESULT, LINKUP_FAILED, LinkMenu_EventScript_CableLinkFailed
	goto_if_eq VAR_RESULT, LINKUP_CONNECTION_ERROR, LinkMenu_EventScript_CableLinkFailed
	goto_if_eq VAR_RESULT, LINKUP_FAILED_BATTLE_TOWER, LinkMenu_EventScript_CableLinkFailed
	end

LinkMenu_EventScript_CableLinkSuccessful::
	message LinkMenu_Text_LinkConnectionEstablished
	waitmessage
	delay 60
	closemessage
	releaseall
	end

LinkMenu_EventScript_CableLinkFailed::
	message LinkMenu_Text_LinkConnectionFailed
	waitmessage
	delay 60
	closemessage
	goto EventScript_OpenLinkMenu

LinkMenu_EventScript_TryWirelessLink::
	setvar VAR_0x8004, LINK_GROUP_BATTLE_TOWER
	goto LinkMenu_EventScript_ChooseLeader
	end

LinkMenu_EventScript_ChooseLeader::
	message CableClub_Text_ChooseGroupLeaderOfTwo
	waitmessage
	multichoice 16, 6, MULTI_LINK_LEADER, FALSE
	switch VAR_RESULT
	case 0, LinkMenu_EventScript_TryJoinGroup
	case 1, LinkMenu_EventScript_TryBecomeLeader
	case 2, LinkMenu_EventScript_CableLinkFailed
	case MULTI_B_PRESSED, LinkMenu_EventScript_CableLinkFailed
	end

LinkMenu_EventScript_TryBecomeLeader::
	call CableClub_EventScript_TryBecomeLinkLeader
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, LinkMenu_EventScript_WirelessLinkSuccessful
	goto_if_eq VAR_RESULT, LINKUP_FAILED, LinkMenu_EventScript_ChooseLeader
	goto_if_eq VAR_RESULT, LINKUP_RETRY_ROLE_ASSIGN, LinkMenu_EventScript_TryBecomeLeader
	goto_if_eq VAR_RESULT, LINKUP_FAILED_BATTLE_TOWER, LinkMenu_EventScript_CableLinkFailed
	release
	return

LinkMenu_EventScript_TryJoinGroup::
	call CableClub_EventScript_TryJoinLinkGroup
	goto_if_eq VAR_RESULT, LINKUP_SUCCESS, LinkMenu_EventScript_WirelessLinkSuccessful
	goto_if_eq VAR_RESULT, LINKUP_FAILED, LinkMenu_EventScript_ChooseLeader
	goto_if_eq VAR_RESULT, LINKUP_RETRY_ROLE_ASSIGN, LinkMenu_EventScript_TryJoinGroup
	goto_if_eq VAR_RESULT, LINKUP_FAILED_BATTLE_TOWER, LinkMenu_EventScript_CableLinkFailed
	release
	return

LinkMenu_EventScript_WirelessLinkSuccessful::
	goto LinkMenu_EventScript_CableLinkSuccessful
	end

LinkMenu_EventScript_ReopenLinkMenu::
	closemessage
	goto EventScript_OpenLinkMenu

LinkMenu_EventScript_ToggleSoullink::
	call_if_unset FLAG_GOT_SOULLINK_EXPLANATION, LinkMenu_EventScript_ExplainSoullink
	goto_if_set FLAG_SOULLINK_ENABLED, LinkMenu_EventScript_AskDisableSoullink
	msgbox LinkMenu_Text_DoYouWantToEnableSoullink, MSGBOX_YESNO
	switch VAR_RESULT
	case YES, LinkMenu_EventScript_EnabledSoullink
	case NO, LinkMenu_EventScript_ReopenLinkMenu
	case MULTI_B_PRESSED, LinkMenu_EventScript_ReopenLinkMenu

LinkMenu_EventScript_ExplainSoullink::
	setflag FLAG_GOT_SOULLINK_EXPLANATION
	msgbox LinkMenu_Text_ExplainSoullink, MSGBOX_DEFAULT
	return

LinkMenu_EventScript_EnabledSoullink::
	setflag FLAG_SOULLINK_ENABLED
	message LinkMenu_Text_EnabledSoullink
	waitmessage
	delay 60
	closemessage
	goto EventScript_OpenLinkMenu

LinkMenu_EventScript_AskDisableSoullink::
	msgbox LinkMenu_Text_DoYouWantToDisableSoullink, MSGBOX_YESNO
	switch VAR_RESULT
	case YES, LinkMenu_EventScript_DisabledSoullink
	case NO, LinkMenu_EventScript_ReopenLinkMenu
	case MULTI_B_PRESSED, LinkMenu_EventScript_ReopenLinkMenu

LinkMenu_EventScript_DisabledSoullink::
	clearflag FLAG_SOULLINK_ENABLED
	message LinkMenu_Text_DisabledSoullink
	waitmessage
	delay 60
	closemessage
	goto EventScript_OpenLinkMenu

LinkMenu_EventScript_AskMixRandomizers::
	specialvar VAR_RESULT, IsLinkConnectionEstablished
	goto_if_eq VAR_RESULT, FALSE, LinkMenu_EventScript_NeedLinkToMixRandomizers
	msgbox LinkMenu_Text_DoYouWantToMixRandomizers, MSGBOX_YESNO
	switch VAR_RESULT
	case YES, LinkMenu_EventScript_MixRandomizers
	case NO, LinkMenu_EventScript_ReopenLinkMenu
	case MULTI_B_PRESSED, LinkMenu_EventScript_ReopenLinkMenu

LinkMenu_EventScript_NeedLinkToMixRandomizers::
	message LinkMenu_Text_NeedLinkToMixRandomizers
	waitmessage
	delay 60
	closemessage
	goto EventScript_OpenLinkMenu

LinkMenu_EventScript_MixRandomizers::
	special BattleTowerReconnectLink
	message gText_PleaseWaitForLink
LinkMenu_EventScript_WaitForMixRandomizers::
	special MixRandomizers
	delay 1
	goto_if_lt VAR_RESULT, 3, LinkMenu_EventScript_WaitForMixRandomizers
	message LinkMenu_Text_MixedRandomizers
	waitmessage
	delay 60
	closemessage
	goto EventScript_OpenLinkMenu

LinkMenu_EventScript_CloseMenu::
	releaseall
	end

LinkMultiBattle_Text_SelectedDifferentTrainer:
	.string "The other player selected a\n"
	.string "different trainer.$"

LinkMultiBattle_Text_InitiatingLinkBattle:
	.string "Initiating link battle.$"

LinkMenu_Text_DoYouWantToConnect:
	.string "Do you want to connect to\n"
	.string "another player?$"

LinkMenu_Text_LinkIsAlreadyEstablished:
	.string "Link is already established.$"

LinkMenu_Text_LinkConnectionEstablished:
	.string "Link established!$"

LinkMenu_Text_LinkConnectionFailed:
	.string "Link failed.$"

LinkMenu_Text_ExplainSoullink:
	.string "In a SOULLINK, your POKéMON are\n"
	.string "linked to the other player's POKéMON\l"
	.string "based on their MET LOCATION.\p"
	.string "If a POKéMON faints in a multibattle,\n"
	.string "other POKéMON with the same\l"
	.string "MET LOCATION will faint as well.\p"
	.string "POKéMON with that MET LOCATION\n"
	.string "cannot be healed anymore.\p"
	.string "The fainted MET LOCATIONS are updated\n"
	.string "after each multibattle in case a\l"
	.string "POKéMON fainted in a solo battle.$"

LinkMenu_Text_DoYouWantToEnableSoullink:
	.string "Do you want to enable the\n"
	.string "SOULLINK MODE?$"

LinkMenu_Text_EnabledSoullink:
	.string "SOULLINK MODE is now enabled.$"

LinkMenu_Text_DoYouWantToDisableSoullink:
	.string "Do you want to disable the\n"
	.string "SOULLINK MODE?$"

LinkMenu_Text_DisabledSoullink:
	.string "SOULLINK MODE is now disabled.$"

LinkMenu_Text_DoYouWantToMixRandomizers:
	.string "Do you want to mix randomizers\n"
	.string "with the other player?$"

LinkMenu_Text_NeedLinkToMixRandomizers:
	.string "You need to establish a link\n"
	.string "before you can mix randomizers.$"

LinkMenu_Text_MixedRandomizers:
	.string "Successfully mixed randomizers!$"
