#include "global.h"
#include "malloc.h"
#include "script.h"
#include "mystery_gift.h"
#include "mystery_gift_server.h"
#include "mystery_gift_link.h"
#include "AAA.h"

enum {
    FUNC_INIT,
    FUNC_DONE,
    FUNC_RECV,
    FUNC_SEND,
    FUNC_RUN,
};

EWRAM_DATA static struct MysteryGiftServer * sServer = NULL;

static void MysteryGiftServer_Init(struct MysteryGiftServer *, const void *, u32, u32);
static void MysteryGiftServer_Free(struct MysteryGiftServer *);
static u32 MysteryGiftServer_CallFunc(struct MysteryGiftServer *);

void MysteryGiftServer_CreateForNews(void)
{
    sServer = AllocZeroed(sizeof(*sServer));
    MysteryGiftServer_Init(sServer, gDistributionServer_SendBufferedWonderNews, 0, 1);
}

void MysteryGiftServer_CreateForCard(void)
{
    sServer = AllocZeroed(sizeof(*sServer));
    MysteryGiftServer_Init(sServer, gDistributionServer_SendBufferedWonderCard, 0, 1);
}

u32 MysteryGiftServer_Run(u16 * endVal)
{
    u32 result;
    if (sServer == NULL)
        return SVR_RET_END;
    result = MysteryGiftServer_CallFunc(sServer);
    if (result == SVR_RET_END)
    {
        *endVal = sServer->param;
        MysteryGiftServer_Free(sServer);
        FREE_AND_SET_NULL(sServer);
    }
    return result;
}

static void MysteryGiftServer_Init(struct MysteryGiftServer * svr, const void * script, u32 sendPlayerId, u32 recvPlayerId)
{
    svr->unused = 0;
    svr->funcId = FUNC_INIT;
    svr->card = AllocZeroed(sizeof(*svr->card));
    svr->news = AllocZeroed(sizeof(*svr->news));
    svr->recvBuffer = AllocZeroed(MG_LINK_BUFFER_SIZE);
    svr->linkGameData = AllocZeroed(sizeof(*svr->linkGameData));
    svr->script = script;
    svr->cmdidx = 0;
    MysteryGiftLink_Init(&svr->link, sendPlayerId, recvPlayerId);
}

static void MysteryGiftServer_Free(struct MysteryGiftServer * svr)
{
    Free(svr->card);
    Free(svr->news);
    Free(svr->recvBuffer);
    Free(svr->linkGameData);
}

static void MysteryGiftServer_InitSend(struct MysteryGiftServer * svr, u32 ident, const void * src, u32 size)
{
    AGB_ASSERT(size <= MG_LINK_BUFFER_SIZE);
    MysteryGiftLink_InitSend(&svr->link, ident, src, size);
}

// Given the command pointer parameter and the 'default' normal data.
// If the command's pointer is not empty use that as the send data, otherwise use the default.
static const void * MysteryGiftServer_GetSendData(const void * dynamicData, const void * defaultData)
{
    if (dynamicData != NULL)
        return dynamicData;
    else
        return defaultData;
}

static u32 MysteryGiftServer_Compare(const void * a, const void * b)
{
    if (b < a)
        return 0;
    else if (b == a)
        return 1;
    else
        return 2;
}

static u32 Server_Init(struct MysteryGiftServer * svr)
{
    svr->funcId = FUNC_RUN;
    return SVR_RET_INIT;
}

static u32 Server_Done(struct MysteryGiftServer * svr)
{
    return SVR_RET_END;
}

static u32 Server_Recv(struct MysteryGiftServer * svr)
{
    if (MysteryGiftLink_Recv(&svr->link))
        svr->funcId = FUNC_RUN;
    return SVR_RET_ACTIVE;
}

static u32 Server_Send(struct MysteryGiftServer * svr)
{
    if (MysteryGiftLink_Send(&svr->link))
        svr->funcId = FUNC_RUN;
    return SVR_RET_ACTIVE;
}

static u32 Server_Run(struct MysteryGiftServer * svr)
{
    // process command
    const struct MysteryGiftServerCmd * cmd = &svr->script[svr->cmdidx];
    const void * ptr;
    svr->cmdidx++;

    switch (cmd->instr)
    {
    case SERVER_RETURN: // Return (parameter)
        AGB_ASSERT(cmd->ptr == NULL);
        svr->funcId = FUNC_DONE;
        svr->param = cmd->parameter; // Set for endVal in MysteryGiftServer_Run
        break;
    case SERVER_WAIT_FOR_SEND_TO_FINISH: // Wait for send to finish
        svr->funcId = FUNC_SEND;
        break;
    case SERVER_WAIT_FOR_RECEIVE_WITH_LINKID: // Wait for receive with expected maguc number (parameter)
        AGB_ASSERT(cmd->ptr == NULL);
        MysteryGiftLink_InitRecv(&svr->link, cmd->parameter, svr->recvBuffer);
        svr->funcId = FUNC_RECV;
        break;
    case SERVER_BRANCH_ALWAYS: // Branch to set of instructions specified by (ptr)
        AGB_ASSERT(cmd->parameter == 0);
        svr->cmdidx = 0;
        svr->script = cmd->ptr;
        break;
    case SERVER_PREPARE_TO_GET_CLIENT_VERSION: // Copy contents of recvBuffer to secondary buffer for validation of client's header information
        AGB_ASSERT(cmd->parameter == 0);
        AGB_ASSERT(cmd->ptr == NULL);
        memcpy(svr->linkGameData, svr->recvBuffer, sizeof(*svr->linkGameData));
        break;
    case SERVER_GET_CLIENT_VERSION: // Validate client's version/revision, and set (result)
        AGB_ASSERT(cmd->parameter == 0);
        AGB_ASSERT(cmd->ptr == NULL);
        svr->param = MysteryGift_ValidateLinkGameData(svr->linkGameData, FALSE);
        break;
    case SERVER_CASE_30: // Validate client's version/revision, and set (result)
        // Emerald accepts FRLG news but not cards, hence this check. However, pokedistribution ignores the second argument of MysteryGift_ValidateLinkGameData, so this case is effectively useless.
        AGB_ASSERT(cmd->parameter == 0);
        AGB_ASSERT(cmd->ptr == NULL);
        svr->param = MysteryGift_ValidateLinkGameData(svr->linkGameData, TRUE);
        break;
    case SERVER_BRANCH_IF_RESULT_WAS: // If previous (result) equals (parameter), branch to (ptr)
        if (svr->param == cmd->parameter)
        {
            svr->cmdidx = 0;
            svr->script = cmd->ptr;
        }
        break;
    case SERVER_CHECK_IF_CLIENT_HAS_CARD_AT: // Check if client already has a Wonder Card with this ID, and set (result) (HAS_NO_CARD if no Card, HAS_SAME_CARD if this Card/ID, HAS_DIFF_CARD if different Card)
        AGB_ASSERT(cmd->parameter == 0);
        ptr = MysteryGiftServer_GetSendData(cmd->ptr, svr->card);
        svr->param = MysteryGift_CompareCardFlags(ptr, svr->linkGameData, ptr);
        break;
    case SERVER_READ_WORD_FROM_CLIENT: // Set (result) to first 32 bits of recvBuffer
        AGB_ASSERT(cmd->parameter == 0);
        AGB_ASSERT(cmd->ptr == NULL);
        svr->param = *(u32 *)svr->recvBuffer;
        break;
    case SERVER_CAN_CLIENT_ACCEPT_STAMP_AT: // Set (result) depending on if client can receive another Stamp (odd if false, even if true)
        AGB_ASSERT(cmd->parameter == 0);
        ptr = MysteryGiftServer_GetSendData(cmd->ptr, &svr->stamp);
        svr->param = MysteryGift_CheckStamps(ptr, svr->linkGameData, ptr);
        break;
    case SERVER_GET_CLIENT_CARD_STAT: // Set (result) to client's link wins/losses/trades or Stamp allowed/received dependent on (parameter)
        AGB_ASSERT(cmd->ptr == NULL);
        svr->param = MysteryGift_GetCardStatFromLinkData(svr->linkGameData, cmd->parameter);
        break;
    case SERVER_CHECK_CLIENT_EASY_CHAT_PROFILE_AGAINST: // Check if client's Easy Chat Profile matches the profile at (parameter), and set (result)
        AGB_ASSERT(cmd->parameter == 0);
        svr->param = MysteryGift_DoesQuestionnaireMatch(svr->linkGameData, cmd->ptr);
        break;
    case SERVER_CASE_12: // Appears to compare the locations of the pointers, rather than the data at those pointers. Unused regardless.
        AGB_ASSERT(cmd->parameter == 0);
        svr->param = MysteryGiftServer_Compare(cmd->ptr, *(void **)svr->recvBuffer);
        break;
    case SERVER_BEGIN_SENDING_NEWS_AT: // Begin sending Wonder News located at (ptr), or if null send buffered Wonder News
        AGB_ASSERT(cmd->parameter == 0);
        MysteryGiftServer_InitSend(svr, MG_LINKID_NEWS, MysteryGiftServer_GetSendData(cmd->ptr, svr->news), sizeof(*svr->news));
        break;
    case SERVER_BEGIN_SENDING_CARD_AT: // Begin sending Wonder Card located at (ptr), or if null send buffered Wonder Card
        AGB_ASSERT(cmd->parameter == 0);
        MysteryGiftServer_InitSend(svr, MG_LINKID_CARD, MysteryGiftServer_GetSendData(cmd->ptr, svr->card), sizeof(*svr->card));
        break;
    case SERVER_BEGIN_SENDING_STAMP_AT: // Begin sending stamp located at (ptr), or if null send buffered stamp
        AGB_ASSERT(cmd->parameter == 0);
        MysteryGiftServer_InitSend(svr, MG_LINKID_STAMP, MysteryGiftServer_GetSendData(cmd->ptr, &svr->stamp), sizeof(svr->stamp));
        break;
    case SERVER_BEGIN_SENDING_RAM_SCRIPT_WITH_SIZE_AT: // Begin sending (parameter) bytes located at (ptr), or if null send general-purpose buffer 1 (client expects RAM Script)
        if (cmd->ptr == NULL)
            MysteryGiftServer_InitSend(svr, MG_LINKID_RAM_SCRIPT, svr->ramScript, svr->ramScriptSize);
        else
            MysteryGiftServer_InitSend(svr, MG_LINKID_RAM_SCRIPT, cmd->ptr, cmd->parameter);
        break;
    case SERVER_BEGIN_SENDING_CLIENT_COMMANDS_WITH_SIZE_AT: // Begin sending (parameter) bytes located at (ptr), or if null send general-purpose buffer 2 (client expects to jump buffer / write client.recvBuffer to client.cmdBuffer)
        if (cmd->ptr == NULL)
            MysteryGiftServer_InitSend(svr, MG_LINKID_CLIENT_SCRIPT, svr->clientScript, svr->clientScriptSize);
        else
            MysteryGiftServer_InitSend(svr, MG_LINKID_CLIENT_SCRIPT, cmd->ptr, cmd->parameter);
        break;
    case SERVER_BEGIN_SENDING_CUSTOM_TRAINER_AT: // Begin sending e-Reader trainer located at (ptr)
        AGB_ASSERT(cmd->parameter == 0);
        MysteryGiftServer_InitSend(svr, MG_LINKID_EREADER_TRAINER, cmd->ptr, sizeof(struct BattleTowerEReaderTrainer));
        break;
    case SERVER_BEGIN_SENDING_MESSAGE_WITH_SIZE_AT: // Begin sending (parameter) bytes located at (ptr) (client expects size 0x40 buffer; used for strings)
        MysteryGiftServer_InitSend(svr, MG_LINKID_DYNAMIC_MSG, cmd->ptr, cmd->parameter);
        break;
    case SERVER_BEGIN_SENDING_MYSTERY_EVENT_WITH_SIZE_AT: // Begin sending (parameter) bytes located at (ptr) (unknown whether this was for Mystery Events or ACE; pokedistribution uses this for Mystery Events)
        if (cmd->ptr == NULL)
            MysteryGiftServer_InitSend(svr, MG_LINKID_MYSTERY_EVENT, svr->mysteryEventScript, svr->mysteryEventScriptSize);
        else
            MysteryGiftServer_InitSend(svr, MG_LINKID_MYSTERY_EVENT, cmd->ptr, cmd->parameter);
        break;
    case SERVER_COPY_TO_BUFFER_CARD_AT: // Copy Wonder Card at (ptr) to WC buffer
        AGB_ASSERT(cmd->parameter == 0);
        memcpy(svr->card, cmd->ptr, sizeof(*svr->card));
        break;
    case SERVER_COPY_TO_BUFFER_NEWS_AT: // Copy Wonder News at (ptr) to WN buffer
        AGB_ASSERT(cmd->parameter == 0);
        memcpy(svr->news, cmd->ptr, sizeof(*svr->news));
        break;
    case SERVER_COPY_TO_BUFFER_STAMP: // Copy stamp (parameter) to 32-bit buffer
        svr->stamp = cmd->parameter;
        break;
    case SERVER_COPY_TO_BUFFER_RAM_SCRIPT_WITH_SIZE_AT: // Set address of general-purpose buffer 1 to (ptr), and set buffer size to (parameter)
        svr->ramScript = cmd->ptr;
        svr->ramScriptSize = cmd->parameter;
        break;
    case SERVER_COPY_TO_BUFFER_CLIENT_COMMANDS_WITH_SIZE_AT: // Set address of general-purpose buffer 2 to (ptr), and set buffer size to (parameter)
        svr->clientScript = cmd->ptr;
        svr->clientScriptSize = cmd->parameter;
        break;
    case SERVER_CASE_26: // Copy Wonder Card in save file to WC buffer, changing SEND_TYPE_ALLOWED to SEND_TYPE_DISALLOWED
        AGB_ASSERT(cmd->parameter == 0 && cmd->ptr == NULL);
        memcpy(svr->card, GetSavedWonderCard(), sizeof(*svr->card));
        // DisableWonderCardSending(svr->card); // pokedistribution is the original server, so this needs to be commented out
        break;
    case SERVER_CASE_27: // Copy Wonder News in save file to WN buffer
        AGB_ASSERT(cmd->parameter == 0 && cmd->ptr == NULL);
        memcpy(svr->news, GetSavedWonderNews(), sizeof(*svr->news));
        break;
    case SERVER_CASE_28: // Copy RAM Script in save file to general-purpose buffer 1 (but only if 2F attendant)
        AGB_ASSERT(cmd->parameter == 0 && cmd->ptr == NULL);
        svr->ramScript = GetSavedRamScriptIfValid();
        break;
    case SERVER_BEGIN_SENDING_ACE_WITH_SIZE_AT: // Begin sending (parameter) bytes located at (ptr) (unknown whether this was for Mystery Events or ACE; pokedistribution uses this for ACE)
        if (cmd->ptr == NULL)
            MysteryGiftServer_InitSend(svr, MG_LINKID_ACE, svr->aceFunction, svr->aceFunctionSize);
        else
            MysteryGiftServer_InitSend(svr, MG_LINKID_ACE, cmd->ptr, cmd->parameter);
        break;
        
    case SERVER_BRANCH_IF_CLIENT_VERSION_MATCHES:
        if (CompareShortCodes(svr->param, cmd->parameter)) {
            svr->cmdidx = 0;
            svr->script = cmd->ptr;
        }
        break;
    case SERVER_CALL_SUBROUTINE:
        svr->callbackIdx = svr->cmdidx;
        svr->callbackScript = svr->script;
        svr->cmdidx = 0;
        svr->script = cmd->ptr;
        break;
    case SERVER_RETURN_FROM_SUBROUTINE:
        svr->cmdidx = svr->callbackIdx;
        svr->script = svr->callbackScript;
        svr->callbackIdx = 0;
        svr->callbackScript = NULL;
        break;
    case SERVER_COPY_TO_BUFFER_MYSTERY_EVENT_AT:
        svr->mysteryEventScript = cmd->ptr;
        svr->mysteryEventScriptSize = MG_LINK_BUFFER_SIZE;
        break;
    case SERVER_COPY_TO_BUFFER_ACE_WITH_SIZE_AT:
        svr->aceFunction = cmd->ptr;
        svr->aceFunctionSize = cmd->parameter;
        break;
    }

    return SVR_RET_ACTIVE;
}

static u32 (*const sFuncTable[])(struct MysteryGiftServer *) = {
    [FUNC_INIT] = Server_Init,
    [FUNC_DONE] = Server_Done,
    [FUNC_RECV] = Server_Recv,
    [FUNC_SEND] = Server_Send,
    [FUNC_RUN] = Server_Run
};

static u32 MysteryGiftServer_CallFunc(struct MysteryGiftServer * svr)
{
    u32 response;
    AGB_ASSERT(svr->funcId < ARRAY_COUNT(sFuncTable));
    response = sFuncTable[svr->funcId](svr);
    AGB_ASSERT(svr->funcId < ARRAY_COUNT(sFuncTable));
    return response;
}
