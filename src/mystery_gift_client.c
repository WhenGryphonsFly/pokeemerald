#include "global.h"
#include "malloc.h"
#include "decompress.h"
#include "overworld.h"
#include "script.h"
#include "battle_tower.h"
#include "mystery_gift.h"
#include "mystery_event_script.h"
#include "mystery_gift_client.h"

enum {
    FUNC_INIT,
    FUNC_DONE,
    FUNC_RECV,
    FUNC_SEND,
    FUNC_RUN,
    FUNC_WAIT,
    FUNC_RUN_MEVENT,
    FUNC_RUN_BUFFER,
};

EWRAM_DATA static struct MysteryGiftClient * sClient = NULL;

static void MysteryGiftClient_Init(struct MysteryGiftClient *, u32, u32);
static u32 MysteryGiftClient_CallFunc(struct MysteryGiftClient *);
static void MysteryGiftClient_Free(struct MysteryGiftClient *);

extern const struct MysteryGiftClientCmd gMysteryGiftClientScript_Init[];

void MysteryGiftClient_Create(bool32 isWonderNews)
{
    sClient = AllocZeroed(sizeof(*sClient));
    MysteryGiftClient_Init(sClient, 1, 0);
    sClient->isWonderNews = isWonderNews;
}

u32 MysteryGiftClient_Run(u16 * endVal)
{
    u32 result;
    if (sClient == NULL)
        return CLI_RET_END;
    result = MysteryGiftClient_CallFunc(sClient);
    if (result == CLI_RET_END)
    {
        *endVal = sClient->param;
        MysteryGiftClient_Free(sClient);
        FREE_AND_SET_NULL(sClient);
    }
    return result;
}

void MysteryGiftClient_AdvanceState(void)
{
    sClient->funcState++;
}

void * MysteryGiftClient_GetMsg(void)
{
    return sClient->msg;
}

void MysteryGiftClient_SetParam(u32 val)
{
    sClient->param = val;
}

static void MysteryGiftClient_Init(struct MysteryGiftClient * client, u32 sendPlayerId, u32 recvPlayerId)
{
    client->unused = 0;
    client->funcId = FUNC_INIT;
    client->funcState = 0;
    client->sendBuffer = AllocZeroed(MG_LINK_BUFFER_SIZE);
    client->recvBuffer = AllocZeroed(MG_LINK_BUFFER_SIZE);
    client->script = AllocZeroed(MG_LINK_BUFFER_SIZE);
    client->msg = AllocZeroed(CLIENT_MAX_MSG_SIZE);
    MysteryGiftLink_Init(&client->link, sendPlayerId, recvPlayerId);
}

static void MysteryGiftClient_Free(struct MysteryGiftClient * client)
{
    Free(client->sendBuffer);
    Free(client->recvBuffer);
    Free(client->script);
    Free(client->msg);
}

static void MysteryGiftClient_CopyRecvScript(struct MysteryGiftClient * client)
{
    memcpy(client->script, client->recvBuffer, MG_LINK_BUFFER_SIZE);
    client->cmdidx = 0;
}

static void MysteryGiftClient_InitSendWord(struct MysteryGiftClient * client, u32 ident, u32 word)
{
    CpuFill32(0, client->sendBuffer, MG_LINK_BUFFER_SIZE);
    *(u32 *)client->sendBuffer = word;
    MysteryGiftLink_InitSend(&client->link, ident, client->sendBuffer, sizeof(word));
}

static u32 Client_Init(struct MysteryGiftClient * client)
{
    memcpy(client->script, gMysteryGiftClientScript_Init, MG_LINK_BUFFER_SIZE);
    client->cmdidx = 0;
    client->funcId = FUNC_RUN;
    client->funcState = 0;
    return CLI_RET_INIT;
}

static u32 Client_Done(struct MysteryGiftClient * client)
{
    return CLI_RET_END;
}


static u32 Client_Recv(struct MysteryGiftClient * client)
{
    if (MysteryGiftLink_Recv(&client->link))
    {
        client->funcId = FUNC_RUN;
        client->funcState = 0;
    }
    return CLI_RET_ACTIVE;
}

static u32 Client_Send(struct MysteryGiftClient * client)
{
    if (MysteryGiftLink_Send(&client->link))
    {
        client->funcId = FUNC_RUN;
        client->funcState = 0;
    }
    return CLI_RET_ACTIVE;
}

static u32 Client_Run(struct MysteryGiftClient * client)
{
    // process command
    struct MysteryGiftClientCmd * cmd = &client->script[client->cmdidx];
    client->cmdidx++;
    switch (cmd->instr)
    {
    case CLIENT_NOP: // nop
        break;
    case CLIENT_RETURN: // Return (parameter)
        client->param = cmd->parameter; // Set for endVal in MysteryGiftClient_Run
        client->funcId = FUNC_DONE;
        client->funcState = 0;
        break;
    case CLIENT_WAIT_FOR_RECEIVE_WITH_LINKID: // Wait for receive with expected magic number (parameter)
        MysteryGiftLink_InitRecv(&client->link, cmd->parameter, client->recvBuffer);
        client->funcId = FUNC_RECV;
        client->funcState = 0;
        break;
    case CLIENT_WAIT_FOR_SEND_TO_FINISH: // Wait for send to finish
        client->funcId = FUNC_SEND;
        client->funcState = 0;
        break;
    case CLIENT_BEGIN_SENDING_END_OF_COMMUNICATION_AND_WAIT: // Send entire buffer (tries to send buffer of size 0, i.e. just the magic number, but code elsewhere interprets 0 as max buffer size)
        MysteryGiftLink_InitSend(&client->link, MG_LINKID_READY_END, client->sendBuffer, 0);
        client->funcId = FUNC_SEND;
        client->funcState = 0;
        break;
    case CLIENT_BEGIN_SENDING_GAME_STAT_AND_WAIT: // Send game stat (parameter)
        MysteryGiftClient_InitSendWord(client, MG_LINKID_GAME_STAT, GetGameStat(cmd->parameter));
        client->funcId = FUNC_SEND;
        client->funcState = 0;
        break;
    case CLIENT_EXECUTE_COMMANDS_FROM_SERVER_IF_RESULT_WAS_0: // If (result) is 0, write recvBuffer to cmdBuffer
        if (client->param == FALSE)
            MysteryGiftClient_CopyRecvScript(client);
        break;
    case CLIENT_EXECUTE_COMMANDS_FROM_SERVER_IF_RESULT_WAS_1: // If (result) is 1, write recvBuffer to cmdBuffer
        if (client->param == TRUE)
            MysteryGiftClient_CopyRecvScript(client);
        break;
    case CLIENT_EXECUTE_COMMANDS_FROM_SERVER: // Write recvBuffer to cmdBuffer
        MysteryGiftClient_CopyRecvScript(client);
        break;
    case CLIENT_DISPLAY_MESSAGE_AND_GET_YES_NO: // Display message and yes(0)/no(1) prompt, and write to (result)
        memcpy(client->msg, client->recvBuffer, CLIENT_MAX_MSG_SIZE);
        client->funcId = FUNC_WAIT;
        client->funcState = 0;
        return CLI_RET_YES_NO;
    case CLIENT_DISPLAY_MESSAGE: // Display message
        memcpy(client->msg, client->recvBuffer, CLIENT_MAX_MSG_SIZE);
        client->funcId = FUNC_WAIT;
        client->funcState = 0;
        return CLI_RET_PRINT_MSG;
    case CLIENT_COPY_MESSAGE: // Copies a message (unknown what for)
        memcpy(client->msg, client->recvBuffer, CLIENT_MAX_MSG_SIZE);
        client->funcId = FUNC_WAIT;
        client->funcState = 0;
        return CLI_RET_COPY_MSG;
    case CLIENT_PROMPT_PLAYER_TO_ACCEPT_CARD: // Prompt to replace Wonder Card, and write to (result) (yes=0, no=1)
        client->funcId = FUNC_WAIT;
        client->funcState = 0;
        return CLI_RET_ASK_TOSS;
    case CLIENT_BEGIN_SENDING_CLIENT_VERSION: // Begin sending client's header information
        MysteryGift_LoadLinkGameData(client->sendBuffer, client->isWonderNews);
        MysteryGiftLink_InitSend(&client->link, MG_LINKID_GAME_DATA, client->sendBuffer, sizeof(struct MysteryGiftLinkGameData));
        break;
    case CLIENT_BEGIN_SENDING_RESULT: // Begin sending (result)
        // param here is set by MG_STATE_LINK_ASK_TOSS or MG_STATE_LINK_ASK_TOSS_UNRECEIVED
        MysteryGiftClient_InitSendWord(client, MG_LINKID_RESPONSE, client->param);
        break;
    case CLIENT_ACCEPT_CARD: // Replace Wonder Card with the one from the server
        SaveWonderCard(client->recvBuffer);
        break;
    case CLIENT_ACCEPT_NEWS_IF_DIFFERENT_AND_BEGIN_SEND: // Replace Wonder News with the one from the server if the ID is different, and begin sending whether the operation occurred or not
        if (!IsWonderNewsSameAsSaved(client->recvBuffer))
        {
            SaveWonderNews(client->recvBuffer);
            MysteryGiftClient_InitSendWord(client, MG_LINKID_RESPONSE, FALSE);
        }
        else
        {
            // Wonder News has already been saved (or is invalid).
            // Prepare a signal to indicate it was not saved.
            MysteryGiftClient_InitSendWord(client, MG_LINKID_RESPONSE, TRUE);
        }
        break;
    case CLIENT_ACCEPT_MYSTERY_EVENT: // Treat recvBuffer as Mystery Event rather than Mystery Gift, and set (result) to data[2] or last call to SetMysteryEventScriptStatus (Immediate Execution Script cmd 0x0E)
        client->funcId = FUNC_RUN_MEVENT;
        client->funcState = 0;
        break;
    case CLIENT_ACCEPT_STAMP: // Add PKMN icon in recvBuffer to bottom of Wonder Card if possible
        MysteryGift_TrySaveStamp(client->recvBuffer);
        break;
    case CLIENT_ACCEPT_RAM_SCRIPT: // Copy recvBuffer to RAM Script
        InitRamScript_NoObjectEvent(client->recvBuffer, 1000);
        break;
    case CLIENT_ACCEPT_EREADER_TRAINER: // Copy recvBuffer to EReader trainer
        memcpy(&gSaveBlock2Ptr->frontier.ereaderTrainer, client->recvBuffer, sizeof(gSaveBlock2Ptr->frontier.ereaderTrainer));
        ValidateEReaderTrainer();
        break;
    case CLIENT_ARBITRARY_CODE_EXECUTION: // Arbitrary code execution of recvBuffer; set (result)
        memcpy(gDecompressionBuffer, client->recvBuffer, MG_LINK_BUFFER_SIZE);
        client->funcId = FUNC_RUN_BUFFER;
        client->funcState = 0;
        break;
    }

    return CLI_RET_ACTIVE;
}

static u32 Client_Wait(struct MysteryGiftClient * client)
{
    if (client->funcState)
    {
        client->funcId = FUNC_RUN;
        client->funcState = 0;
    }
    return CLI_RET_ACTIVE;
}

static u32 Client_RunMysteryEventScript(struct MysteryGiftClient * client)
{
    switch (client->funcState)
    {
    case 0:
        InitMysteryEventScriptContext(client->recvBuffer);
        client->funcState++;
        break;
    case 1:
        if (!RunMysteryEventScriptContextCommand(&client->param))
        {
            client->funcId = FUNC_RUN;
            client->funcState = 0;
        }
        break;
    }
    return CLI_RET_ACTIVE;
}

static u32 Client_RunBufferScript(struct MysteryGiftClient * client)
{
    // exec arbitrary code
    u32 (*func)(u32 *, struct SaveBlock2 *, struct SaveBlock1 *) = (void *)gDecompressionBuffer;
    if (func(&client->param, gSaveBlock2Ptr, gSaveBlock1Ptr) == 1)
    {
        client->funcId = FUNC_RUN;
        client->funcState = 0;
    }
    return CLI_RET_ACTIVE;
}

static u32 MysteryGiftClient_CallFunc(struct MysteryGiftClient * client)
{
    u32 (*funcs[])(struct MysteryGiftClient *) = {
        [FUNC_INIT] = Client_Init,
        [FUNC_DONE] = Client_Done,
        [FUNC_RECV] = Client_Recv,
        [FUNC_SEND] = Client_Send,
        [FUNC_RUN]  = Client_Run,
        [FUNC_WAIT] = Client_Wait,
        [FUNC_RUN_MEVENT] = Client_RunMysteryEventScript,
        [FUNC_RUN_BUFFER] = Client_RunBufferScript
    };
    return funcs[client->funcId](client);
}
