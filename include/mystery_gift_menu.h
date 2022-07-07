#ifndef GUARD_MYSTERY_GIFT_MENU_H
#define GUARD_MYSTERY_GIFT_MENU_H

extern bool8 gGiftIsFromEReader;

u16 GetMysteryGiftBaseBlock(void);
void CB2_MysteryGiftEReader(void);
void PrintMysteryGiftOrEReaderTopMenu(bool8 isJapanese, bool32 usePickOkCancel);
void MG_DrawCheckerboardPattern(u32 bg);
void MainCB_FreeAllBuffersAndReturnToInitTitleScreen(void);
bool32 PrintMysteryGiftMenuMessage(u8 *textState, const u8 *str);
void AddTextPrinterToWindow1(const u8 *src);
void CB2_InitEReader(void);
void CB2_InitMysteryGift(void);
void MG_DrawTextBorder(u8 windowId);
s8 DoMysteryGiftYesNo(u8 *textState, u16 *windowId, bool8 yesNoBoxPlacement, const u8 *str);
bool32 ExitWonderCardOrNews(bool32 isWonderNews, bool32 useCancel);

struct MysteryGiftTaskData
{
    u16 var; // Multipurpose
    u16 unused1;
    u16 unused2;
    u16 unused3;
    u8 state;
    u8 textState;
    u8 unused4;
    u8 unused5;
    bool8 isWonderNews;
    bool8 sourceIsFriend;
    u8 msgId;
    u8 * clientMsg;
    
    u32 source;
};

#endif //GUARD_MYSTERY_GIFT_MENU_H
