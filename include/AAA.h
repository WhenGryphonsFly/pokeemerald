#ifndef GUARD_AAA_LIST_MENU_H
#define GUARD_AAA_LIST_MENU_H
#include "global.h"
#include "list_menu.h"
#include "mystery_gift_server.h"

//===============================//
// List Menu //==================//
//===============================//

//extern const struct ListMenuTemplate sListMenuTemplate_Custom;
//extern const struct WindowTemplate sWindowTemplate_Custom;

u32 CreateAndPollListMenu(const struct ListMenuItem* items, u8 totalItems);
void SetListMenuWidth(struct ListMenuTemplate* listMenuTemplate, struct WindowTemplate* windowTemplate);
void SetListMenuHeight(struct ListMenuTemplate* listMenuTemplate, struct WindowTemplate* windowTemplate);

//===============================//
// Short Codes //================//
//===============================//

bool32 CompareShortCodes(u32 clientShortCode, u32 queryShortCode);

#define ASCII_WIDTH 7
#define ASCII_BITS 0x7F
#define REVISION_WIDTH 8
#define REVISION_BITS 0xFF
#define LANGUAGE_BITS 0x7

#define SC_LANGUAGE(language) (( \
	(language == 'J') ? LANGUAGE_JAPANESE : ( \
	(language == 'E') ? LANGUAGE_ENGLISH : ( \
	(language == 'F') ? LANGUAGE_FRENCH : ( \
	(language == 'I') ? LANGUAGE_ITALIAN : ( \
	(language == 'D') ? LANGUAGE_GERMAN : ( \
	(language == 'S') ? LANGUAGE_SPANISH :  \
	0 ))))) \
) & LANGUAGE_BITS)


#define SHORT_CODE(gameCode, revision) ( \
	((gameCode[0] & ASCII_BITS) << (32-ASCII_WIDTH)) | \
	((gameCode[1] & ASCII_BITS) << (32-2*ASCII_WIDTH)) | \
	((gameCode[2] & ASCII_BITS) << (32-3*ASCII_WIDTH)) | \
	((SC_LANGUAGE(gameCode[3])) << REVISION_WIDTH) | \
	revision \
)

//===============================//
// Menu Layout //================//
//===============================//

void task00_mystery_gift(u8 taskId);

//===============================//
// Distribution Data //==========//
//===============================//

struct MysteryGiftDistributionData {
	u8 name[41];
	
	u16 wonderCardServerStamps[MAX_STAMP_CARD_STAMPS];
	struct WonderCard wonderCard;
	struct WonderNews wonderNews;
	
	const struct MysteryGiftServerCmd* initialServerCommands;
};

#endif//GUARD_AAA_LIST_MENU_H

