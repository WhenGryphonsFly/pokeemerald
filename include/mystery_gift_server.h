#ifndef GUARD_MYSTERY_GIFT_SERVER_H
#define GUARD_MYSTERY_GIFT_SERVER_H

#include "mystery_gift_link.h"

struct MysteryGiftServerCmd
{
    u32 instr;
    u32 parameter;
    const void * ptr;
};

extern const struct MysteryGiftServerCmd gDistributionServer_GetClientVersion[6];
extern const struct MysteryGiftServerCmd gDistributionServer_Incompatible[4];
extern const struct MysteryGiftServerCmd gDistributionServer_SendBufferedWonderCard[7];
extern const struct MysteryGiftServerCmd gDistributionServer_SendBufferedStamp[6];
extern const struct MysteryGiftServerCmd gDistributionServer_SendBufferedWonderNews[6];
extern const struct MysteryGiftServerCmd gDistributionServer_SendBufferedMysteryEvent[6];
extern const struct MysteryGiftServerCmd gDistributionServer_EndOfTransmission[6];

// Return values for Server_* functions.
// Other than SVR_RET_END, effectively useless (not checked for).
enum {
    SVR_RET_INIT,
    SVR_RET_ACTIVE,
    SVR_RET_UNUSED,
    SVR_RET_END
};

/* 
	Results of server commands (unlisted if result is unmodified)
	
	SERVER_RETURN(x)
		x
	SERVER_GET_CLIENT_VERSION
		A condensed form of the client's game code and revision, as specified by SHORT_CODE
		Specifically, bit packing is done by eliminating 8 bits from the game code.
			For the first three characters, treat them as ASCII, which allows discarding of the MSB
			For the fourth character, convert to a 3-bit language identifier, a contraction of 5 bits
	SERVER_CHECK_IF_CLIENT_HAS_CARD_AT(y) // y==0 uses buffer
		0, if the client has no card
		1, if the client has a card with the same ID as the server's card
		2, if the client has a card with a different ID as the server's card
	SERVER_READ_WORD_FROM_CLIENT
		The first 32 bits from the receive buffer
	SERVER_CAN_CLIENT_ACCEPT_STAMP_AT(y) // y==0 uses buffer
		1, if the client has no spaces left on their Wonder Card for stamps
		2, if the client has one space left and can receive the stamp
		3, if the client has at least one space left but already has a stamp with the same icon and ID
		4, if the client has more than one space left and can receive the stamp
	SERVER_GET_CLIENT_CARD_STAT(x)
		CARD_STAT_BATTLES_WON
		CARD_STAT_BATTLES_LOST
		CARD_STAT_NUM_TRADES
			The number of link wins/losses/trades the client has
		CARD_STAT_NUM_STAMPS
			The number of stamps currently on the client's Wonder Card
		CARD_STAT_MAX_STAMPS
			The maximum number of stamps the client's Wonder Card supports
	SERVER_CHECK_CLIENT_EASY_CHAT_PROFILE_AGAINST(y)
		0, if the client's easy chat profile does not match the profile at y
		1, if the client's easy chat profile does match the profile at y 
*/
// IDs for server script instructions
enum {
    SERVER_RETURN,
    SERVER_WAIT_FOR_SEND_TO_FINISH,
    SERVER_WAIT_FOR_RECEIVE_WITH_LINKID,
    SERVER_BRANCH_ALWAYS,
    SERVER_BRANCH_IF_RESULT_WAS,
    SERVER_PREPARE_TO_GET_CLIENT_VERSION,
    SERVER_GET_CLIENT_VERSION,
    SERVER_CHECK_IF_CLIENT_HAS_CARD_AT,
    SERVER_READ_WORD_FROM_CLIENT,
    SERVER_CAN_CLIENT_ACCEPT_STAMP_AT,
    SERVER_GET_CLIENT_CARD_STAT,
    SERVER_CHECK_CLIENT_EASY_CHAT_PROFILE_AGAINST,
    SERVER_CASE_12,
    SERVER_BEGIN_SENDING_CARD_AT,
    SERVER_BEGIN_SENDING_NEWS_AT,
    SERVER_BEGIN_SENDING_RAM_SCRIPT_WITH_SIZE_AT,
    SERVER_BEGIN_SENDING_STAMP_AT,
    SERVER_BEGIN_SENDING_MYSTERY_EVENT_WITH_SIZE_AT,
    SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT,
    SERVER_BEGIN_SENDING_CUSTOM_TRAINER_AT,
    SERVER_BEGIN_SENDING_MESSAGE_WITH_SIZE_AT,
    SERVER_COPY_TO_BUFFER_STAMP,
    SERVER_COPY_TO_BUFFER_CARD_AT,
    SERVER_COPY_TO_BUFFER_NEWS_AT,
    SERVER_COPY_TO_BUFFER_RAM_SCRIPT_WITH_SIZE_AT,
    SERVER_COPY_TO_BUFFER_CLIENT_COMMANDS_WITH_SIZE_AT,
    SERVER_CASE_26,
    SERVER_CASE_27,
    SERVER_CASE_28,
    SERVER_BEGIN_SENDING_ACE_WITH_SIZE_AT,
    SERVER_CASE_30,
    
    SERVER_BRANCH_IF_CLIENT_VERSION_MATCHES,
    SERVER_CALL_SUBROUTINE,
    SERVER_RETURN_FROM_SUBROUTINE,
    SERVER_COPY_TO_BUFFER_MYSTERY_EVENT_AT,
    SERVER_COPY_TO_BUFFER_ACE_WITH_SIZE_AT
};

#define SERVER_MACRO_GET_VERSION \
	{SERVER_CALL_SUBROUTINE, .ptr = gDistributionServer_GetClientVersion}
	
#define SERVER_MACRO_SEND_CARD(cardLocation) \
	{SERVER_COPY_TO_BUFFER_CARD_AT, .ptr = &cardLocation},\
	{SERVER_CALL_SUBROUTINE, .ptr = gDistributionServer_SendBufferedWonderCard}

#define SERVER_MACRO_SEND_STAMP(stampSpecies, stampId) \
	{SERVER_COPY_TO_BUFFER_STAMP, .parameter = ((stampSpecies << 16) | stampId)},\
	{SERVER_CALL_SUBROUTINE, .ptr = gDistributionServer_SendBufferedStamp}

#define SERVER_MACRO_SEND_NEWS(newsLocation) \
	{SERVER_COPY_TO_BUFFER_NEWS_AT, .ptr = &newsLocation},\
	{SERVER_CALL_SUBROUTINE, .ptr = gDistributionServer_SendBufferedWonderNews}

#define SERVER_MACRO_SEND_MYSTERY_EVENT(eventLocation) \
	{SERVER_COPY_TO_BUFFER_MYSTERY_EVENT_AT, .ptr = &eventLocation},\
	{SERVER_CALL_SUBROUTINE, .ptr = gDistributionServer_SendBufferedMysteryEvent}

#define SERVER_MACRO_END_TRANSMISSION \
	{SERVER_CALL_SUBROUTINE, .ptr = gDistributionServer_EndOfTransmission}

// IDs for server messages when ending a script.
// Given as the parameter to SERVER_RETURN, and resolved to text in GetServerResultMessage
enum {
    SVR_MSG_NOTHING_SENT,
    SVR_MSG_RECORD_UPLOADED,
    SVR_MSG_CARD_SENT,
    SVR_MSG_NEWS_SENT,
    SVR_MSG_STAMP_SENT,
    SVR_MSG_HAS_CARD,
    SVR_MSG_HAS_STAMP,
    SVR_MSG_HAS_NEWS,
    SVR_MSG_NO_ROOM_STAMPS,
    SVR_MSG_CLIENT_CANCELED,
    SVR_MSG_CANT_SEND_GIFT_1,
    SVR_MSG_COMM_ERROR,
    SVR_MSG_GIFT_SENT_1,
    SVR_MSG_GIFT_SENT_2,
    SVR_MSG_CANT_SEND_GIFT_2,
};

struct MysteryGiftServer
{
    u32 unused;
    u32 param;
    u32 funcId;
    u32 cmdidx;
    const struct MysteryGiftServerCmd * script;
    void * recvBuffer;
    struct WonderCard * card;
    struct WonderNews * news;
    struct MysteryGiftLinkGameData * linkGameData;
    const void * ramScript;
    u32 ramScriptSize;
    const void * clientScript;
    u32 clientScriptSize;
    u32 stamp;
    struct MysteryGiftLink link;
    
    void* callbackScript;
    u32 callbackIdx;
    const void * mysteryEventScript;
    u32 mysteryEventScriptSize;
    const void * aceFunction;
    u32 aceFunctionSize;
};

void MysteryGiftServer_CreateForCard();
void MysteryGiftServer_CreateForNews();
u32 MysteryGiftServer_Run(u16 * endVal);

#endif //GUARD_MYSTERY_GIFT_SERVER_H
