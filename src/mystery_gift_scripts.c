#include "global.h"
#include "mystery_gift_client.h"
#include "mystery_gift_server.h"
#include "mystery_gift.h"

const struct MysteryGiftClientCmd gMysteryGiftClientScript_Init[] = {
	{CLIENT_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_CLIENT_SCRIPT},
	{CLIENT_EXECUTE_COMMANDS_FROM_SERVER}
};

// Create arguments for SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT or SERVER_BEGIN_SENDING_MESSAGE_WITH_SIZE_AT
// (a script/text size and pointer to send to the client)
#define PTR_ARG(pointer) .parameter = sizeof(pointer), .ptr = pointer

//==================================================================//
// Get Version //===================================================//
//==================================================================//

static const struct MysteryGiftClientCmd sDistributionClient_SendClientVersion[] = {
	{CLIENT_BEGIN_SENDING_CLIENT_VERSION},
	{CLIENT_WAIT_FOR_SEND_TO_FINISH},
	{CLIENT_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_CLIENT_SCRIPT},
	{CLIENT_EXECUTE_COMMANDS_FROM_SERVER}
};



const struct MysteryGiftServerCmd gDistributionServer_GetClientVersion[] = {
	{SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT, PTR_ARG(sDistributionClient_SendClientVersion)},
	{SERVER_WAIT_FOR_SEND_TO_FINISH},
	{SERVER_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_GAME_DATA},
	{SERVER_PREPARE_TO_GET_CLIENT_VERSION},
	{SERVER_GET_CLIENT_VERSION},
	{SERVER_RETURN_FROM_SUBROUTINE}
};

//==================================================================//
// Incompatible //==================================================//
//==================================================================//

static const struct MysteryGiftClientCmd sDistributionClient_Incompatible[] = {
	{CLIENT_BEGIN_SENDING_END_OF_COMMUNICATION_AND_WAIT},
	{CLIENT_RETURN, CLI_MSG_CANT_ACCEPT}
};



const struct MysteryGiftServerCmd gDistributionServer_Incompatible[] = {
	{SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT, PTR_ARG(sDistributionClient_Incompatible)},
	{SERVER_WAIT_FOR_SEND_TO_FINISH},
	{SERVER_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_READY_END},
	{SERVER_RETURN, SVR_MSG_CANT_SEND_GIFT_1}
};

//==================================================================//
// Send Wonder Card //==============================================//
//==================================================================//

static const struct MysteryGiftClientCmd sDistributionClient_HadCard[] = {
	{CLIENT_BEGIN_SENDING_END_OF_COMMUNICATION_AND_WAIT},
	{CLIENT_RETURN, CLI_MSG_HAD_CARD}
};

static const struct MysteryGiftClientCmd sDistributionClient_AskToTossCard[] = {
	{CLIENT_PROMPT_PLAYER_TO_ACCEPT_CARD},
	{CLIENT_BEGIN_SENDING_RESULT},
	{CLIENT_WAIT_FOR_SEND_TO_FINISH},
	{CLIENT_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_CLIENT_SCRIPT},
	{CLIENT_EXECUTE_COMMANDS_FROM_SERVER}
};

static const struct MysteryGiftClientCmd sDistributionClient_RefusedCard[] = {
	{CLIENT_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_DYNAMIC_MSG},
	{CLIENT_COPY_MESSAGE},
	{CLIENT_BEGIN_SENDING_END_OF_COMMUNICATION_AND_WAIT},
	{CLIENT_RETURN, CLI_MSG_BUFFER_FAILURE}
};

static const u8 sDistributionClient_RefusedCardText[] = _("Canceled reading\nthe Card.");

static const struct MysteryGiftClientCmd sDistributionClient_NewCard[] = {
	{CLIENT_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_CARD},
	{CLIENT_ACCEPT_CARD},
	{CLIENT_BEGIN_SENDING_RESULT},
	{CLIENT_WAIT_FOR_SEND_TO_FINISH},
	{CLIENT_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_CLIENT_SCRIPT},
	{CLIENT_EXECUTE_COMMANDS_FROM_SERVER}
};



static const struct MysteryGiftServerCmd sDistributionServer_ClientCanceledCard[] = {
	{SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT, PTR_ARG(sDistributionClient_RefusedCard)},
	{SERVER_WAIT_FOR_SEND_TO_FINISH},
	{SERVER_BEGIN_SENDING_MESSAGE_WITH_SIZE_AT, PTR_ARG(sDistributionClient_RefusedCardText)},
	{SERVER_WAIT_FOR_SEND_TO_FINISH},
	{SERVER_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_READY_END},
	{SERVER_RETURN, SVR_MSG_CLIENT_CANCELED}
};

static const struct MysteryGiftServerCmd sDistributionServer_NewCard[] = {
	{SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT, PTR_ARG(sDistributionClient_NewCard)},
	{SERVER_WAIT_FOR_SEND_TO_FINISH},
	{SERVER_BEGIN_SENDING_CARD_AT},
	{SERVER_WAIT_FOR_SEND_TO_FINISH},
	{SERVER_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_RESPONSE},
	{SERVER_RETURN_FROM_SUBROUTINE}
};

static const struct MysteryGiftServerCmd sDistributionServer_TossCard[] = {
	{SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT, PTR_ARG(sDistributionClient_AskToTossCard)},
	{SERVER_WAIT_FOR_SEND_TO_FINISH},
	{SERVER_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_RESPONSE},
	{SERVER_READ_WORD_FROM_CLIENT},
	{SERVER_BRANCH_IF_RESULT_WAS, FALSE, sDistributionServer_NewCard},	   // Tossed old card, send new one
	{SERVER_BRANCH_ALWAYS, .ptr = sDistributionServer_ClientCanceledCard} // Kept old card, cancel new one
};

const struct MysteryGiftServerCmd gDistributionServer_SendBufferedWonderCard[] = {
	{SERVER_CHECK_IF_CLIENT_HAS_CARD_AT},
	{SERVER_BRANCH_IF_RESULT_WAS, HAS_DIFF_CARD, sDistributionServer_TossCard},
	{SERVER_BRANCH_IF_RESULT_WAS, HAS_NO_CARD, sDistributionServer_NewCard},
	// HAS_SAME_CARD
	{SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT, PTR_ARG(sDistributionClient_HadCard)},
	{SERVER_WAIT_FOR_SEND_TO_FINISH},
	{SERVER_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_READY_END},
	{SERVER_RETURN, SVR_MSG_HAS_CARD}
};

//==================================================================//
// Send Stamp //====================================================//
//==================================================================//

static const struct MysteryGiftClientCmd sDistributionClient_AddStamp[] = {
	{CLIENT_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_STAMP},
	{CLIENT_ACCEPT_STAMP},
	{CLIENT_BEGIN_SENDING_RESULT},
	{CLIENT_WAIT_FOR_SEND_TO_FINISH},
	{CLIENT_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_CLIENT_SCRIPT},
	{CLIENT_EXECUTE_COMMANDS_FROM_SERVER}
};

const struct MysteryGiftServerCmd gDistributionServer_SendBufferedStamp[] = {
	{SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT, PTR_ARG(sDistributionClient_AddStamp)},
	{SERVER_WAIT_FOR_SEND_TO_FINISH},
	{SERVER_BEGIN_SENDING_STAMP_AT},
	{SERVER_WAIT_FOR_SEND_TO_FINISH},
	{SERVER_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_RESPONSE},
	{SERVER_RETURN_FROM_SUBROUTINE}
};

//==================================================================//
// Send Wonder News //==============================================//
//==================================================================//

static const struct MysteryGiftClientCmd sDistributionClient_SaveNews[] = {
	{CLIENT_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_NEWS},
	{CLIENT_ACCEPT_NEWS_IF_DIFFERENT_AND_BEGIN_SEND},
	{CLIENT_WAIT_FOR_SEND_TO_FINISH},
	{CLIENT_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_CLIENT_SCRIPT},
	{CLIENT_EXECUTE_COMMANDS_FROM_SERVER}
};



const struct MysteryGiftServerCmd gDistributionServer_SendBufferedWonderNews[] = {
	{SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT, PTR_ARG(sDistributionClient_SaveNews)},
	{SERVER_WAIT_FOR_SEND_TO_FINISH},
	{SERVER_BEGIN_SENDING_NEWS_AT},
	{SERVER_WAIT_FOR_SEND_TO_FINISH},
	{SERVER_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_RESPONSE},
	{SERVER_RETURN_FROM_SUBROUTINE}
};

//==================================================================//
// Send Mystery Event //============================================//
//==================================================================//

static const struct MysteryGiftClientCmd sDistributionClient_MysteryEvent[] = {
	{CLIENT_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_MYSTERY_EVENT},
	{CLIENT_ACCEPT_MYSTERY_EVENT},
	{CLIENT_BEGIN_SENDING_RESULT},
	{CLIENT_WAIT_FOR_SEND_TO_FINISH},
	{CLIENT_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_CLIENT_SCRIPT},
	{CLIENT_EXECUTE_COMMANDS_FROM_SERVER}
};



const struct MysteryGiftServerCmd gDistributionServer_SendBufferedMysteryEvent[] = {
	{SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT, PTR_ARG(sDistributionClient_MysteryEvent)},
	{SERVER_WAIT_FOR_SEND_TO_FINISH},
	{SERVER_BEGIN_SENDING_MYSTERY_EVENT_WITH_SIZE_AT},
	{SERVER_WAIT_FOR_SEND_TO_FINISH},
	{SERVER_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_RESPONSE},
	{SERVER_RETURN_FROM_SUBROUTINE}
};

//==================================================================//
// End of Transmission //===========================================//
//==================================================================//

static const struct MysteryGiftClientCmd sDistributionClient_EndOfTransmission[] = {
	{CLIENT_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_DYNAMIC_MSG},
	{CLIENT_COPY_MESSAGE},
	{CLIENT_BEGIN_SENDING_END_OF_COMMUNICATION_AND_WAIT},
	{CLIENT_RETURN, CLI_MSG_BUFFER_SUCCESS}
};

static const u8 sDistributionClient_EndOfTransmissionText[] = _("Thank you for using the\nMYSTERY GIFT system.");



const struct MysteryGiftServerCmd gDistributionServer_EndOfTransmission[] = {
	{SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT, PTR_ARG(sDistributionClient_EndOfTransmission)},
	{SERVER_WAIT_FOR_SEND_TO_FINISH},
	{SERVER_BEGIN_SENDING_MESSAGE_WITH_SIZE_AT, PTR_ARG(sDistributionClient_EndOfTransmissionText)},
	{SERVER_WAIT_FOR_SEND_TO_FINISH},
	{SERVER_WAIT_FOR_RECEIVE_WITH_LINKID, MG_LINKID_READY_END},
	{SERVER_RETURN, SVR_MSG_GIFT_SENT_1}
};
