#ifndef GUARD_MYSTERY_GIFT_CLIENT_H
#define GUARD_MYSTERY_GIFT_CLIENT_H

#include "mystery_gift_link.h"

// Return values for client functions called by MysteryGiftClient_Run
enum {
    CLI_RET_INIT,
    CLI_RET_ACTIVE,
    CLI_RET_YES_NO,
    CLI_RET_PRINT_MSG,
    CLI_RET_ASK_TOSS,
    CLI_RET_COPY_MSG,
    CLI_RET_END,
};

/*
	Results of client commands (unlisted if result is unmodified)
	
	CLIENT_RETURN
		x
	CLIENT_DISPLAY_MESSAGE_AND_GET_YES_NO
		0, if the player selects "Yes"
		1, if the player selects "No"
	CLIENT_PROMPT_PLAYER_TO_ACCEPT_CARD
		0, if the player selects "Yes"
		1, if the player selects "No"
	CLIENT_ACCEPT_MYSTERY_EVENT
		The contents of data[2] of the Mystery Event upon completion
		data[2] is set by SetMysteryEventScriptStatus in RAM Scripts, among other sources
	CLIENT_ARBITRARY_CODE_EXECUTION
		Arbitrary
*/
// IDs for client script instructions
enum {
    CLIENT_NOP,
    CLIENT_RETURN,
    CLIENT_WAIT_FOR_RECEIVE_WITH_LINKID,
    CLIENT_WAIT_FOR_SEND_TO_FINISH,
    CLIENT_EXECUTE_COMMANDS_FROM_SERVER,
    CLIENT_DISPLAY_MESSAGE_AND_GET_YES_NO,
    CLIENT_EXECUTE_COMMANDS_FROM_SERVER_IF_RESULT_WAS_0,
    CLIENT_EXECUTE_COMMANDS_FROM_SERVER_IF_RESULT_WAS_1,
    CLIENT_BEGIN_SENDING_CLIENT_VERSION,
    CLIENT_ACCEPT_NEWS_IF_DIFFERENT_AND_BEGIN_SEND,
    CLIENT_ACCEPT_CARD,
    CLIENT_DISPLAY_MESSAGE,
    CLIENT_COPY_MESSAGE,
    CLIENT_PROMPT_PLAYER_TO_ACCEPT_CARD,
    CLIENT_BEGIN_SENDING_RESULT,
    CLIENT_ACCEPT_MYSTERY_EVENT,
    CLIENT_ACCEPT_STAMP,
    CLIENT_ACCEPT_RAM_SCRIPT,
    CLIENT_ACCEPT_EREADER_TRAINER,
    CLIENT_BEGIN_SENDING_GAME_STAT_AND_WAIT,
    CLIENT_BEGIN_SENDING_END_OF_COMMUNICATION_AND_WAIT,
    CLIENT_ARBITRARY_CODE_EXECUTION,
};

// IDs for client messages when ending a script.
// Given as the parameter to CLIENT_RETURN, and resolved to text in GetClientResultMessage
enum {
    CLI_MSG_NOTHING_SENT,
    CLI_MSG_RECORD_UPLOADED,
    CLI_MSG_CARD_RECEIVED,
    CLI_MSG_NEWS_RECEIVED,
    CLI_MSG_STAMP_RECEIVED,
    CLI_MSG_HAD_CARD,
    CLI_MSG_HAD_STAMP,
    CLI_MSG_HAD_NEWS,
    CLI_MSG_NO_ROOM_STAMPS,
    CLI_MSG_COMM_CANCELED,
    CLI_MSG_CANT_ACCEPT,
    CLI_MSG_COMM_ERROR,
    CLI_MSG_TRAINER_RECEIVED,
    CLI_MSG_BUFFER_SUCCESS,
    CLI_MSG_BUFFER_FAILURE,
};

#define CLIENT_MAX_MSG_SIZE 64

struct MysteryGiftClientCmd
{
    u32 instr;
    u32 parameter;
};

struct MysteryGiftClient
{
    u32 unused;
    u32 param;
    u32 funcId;
    u32 funcState;
    u32 cmdidx;
    void * sendBuffer;
    void * recvBuffer;
    struct MysteryGiftClientCmd * script;
    void * msg;
    struct MysteryGiftLink link;
    bool32 isWonderNews;
};

void MysteryGiftClient_Create(bool32 isWonderNews);
u32 MysteryGiftClient_Run(u16 * endVal);
void MysteryGiftClient_AdvanceState(void);
void * MysteryGiftClient_GetMsg(void);
void MysteryGiftClient_SetParam(u32 value);

#endif //GUARD_MYSTERY_GIFT_CLIENT_H
