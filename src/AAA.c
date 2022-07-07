#include "global.h"
#include "main.h"
#include "list_menu.h"
#include "mystery_gift_menu.h"
#include "mystery_gift_view.h"
#include "task.h"
#include "text.h"
#include "title_screen.h"
#include "AAA.h"
#include "../distributions/distributions.h"

//===============================//
// List Menu Helpers //==========//
//===============================//

const static struct ListMenuTemplate sListMenuTemplate_Custom = {
	.items = NULL,
	.moveCursorFunc = ListMenuDefaultCursorMoveFunc,
	.itemPrintFunc = NULL,
	.totalItems = 0, // Overwritten
	.maxShowed = 0, // Overwritten
	.windowId = 0,
	.header_X = 0,
	.item_X = 8,
	.cursor_X = 0,
	.upText_Y = 0,
	.cursorPal = 2,
	.fillValue = 1,
	.cursorShadowPal = 3,
	.lettersSpacing = 0,
	.itemVerticalPadding = 0,
	.scrollMultiple = 0,
	.fontId = 2,
	.cursorKind = 0
};

const static struct WindowTemplate sWindowTemplate_Custom = {
	.bg = 0x00,
	.tilemapLeft = 0x08, // Overwritten
	.tilemapTop = 0x05, // Overwritten
	.width = 0x0e, // Overwritten
	.height = 0x05, // Overwritten
	.paletteNum = 12,
	.baseBlock = 0x0155
};

u32 CreateAndPollListMenu(const struct ListMenuItem* items, u8 totalItems) {
	s32 response;
	struct ListMenuTemplate listMenuTemplate = sListMenuTemplate_Custom;
	struct WindowTemplate windowTemplate = sWindowTemplate_Custom;
	
	listMenuTemplate.items = items;
	listMenuTemplate.totalItems = totalItems;
	listMenuTemplate.maxShowed = totalItems;
	SetListMenuWidth(&listMenuTemplate, &windowTemplate);
	SetListMenuHeight(&listMenuTemplate, &windowTemplate);
	
	response = DoMysteryGiftListMenu(&windowTemplate, &listMenuTemplate, 1, 0x00A, 0xE0);
	if (response != LIST_NOTHING_CHOSEN) {
		ClearWindowTilemap(2);
		CopyWindowToVram(2, COPYWIN_MAP);
	}
	return response;
}

void SetListMenuWidth(struct ListMenuTemplate* listMenuTemplate, struct WindowTemplate* windowTemplate) {
	u32 width;
	s32 finalWidth;
	u32 i;
	
	width = 0;
	for (i = 0; i < listMenuTemplate->totalItems; i++) {
		u32 curWidth = GetStringWidth(2, listMenuTemplate->items[i].name, listMenuTemplate->lettersSpacing);
		if (curWidth > width)
			width = curWidth;
	}
	finalWidth = (((width + 9) / 8) + 2) & ~1;
	windowTemplate->width = finalWidth;
	windowTemplate->tilemapLeft = (30 - finalWidth) / 2;
	return;
}

void SetListMenuHeight(struct ListMenuTemplate* listMenuTemplate, struct WindowTemplate* windowTemplate) {
	u32 height;
	u32 delta;
	u8 changeCenter;
	
	height = 0;
	switch (listMenuTemplate->maxShowed) {
		case 1:
			height = 2;
			break;
		case 2:
			height = 4;
			break;
		case 3:
			height = 6;
			break;
		case 4:
			height = 7;
			break;
		case 5:
			height = 9;
			break;
		case 6:
			height = 11;
			break;
		case 7:
			height = 13;
			break;
		case 8:
			height = 14;
			break;
		case 9:
		default:
			height = 16;
			listMenuTemplate->maxShowed = 9;
			break;
	}
	
	windowTemplate->height = height;
	delta = height / 2;
	changeCenter = (listMenuTemplate->maxShowed <= 3) ? 1 : 0;
	windowTemplate->tilemapTop = (0x0B - changeCenter) - delta - (height % 2);
	return;
}

//===============================//
// Short Codes //================//
//===============================//

inline u8 inline_getAscii0(u32 shortCode) {
	return (shortCode >> (32-ASCII_WIDTH)) & ASCII_BITS;
}
inline u8 inline_getAscii1(u32 shortCode) {
	return (shortCode >> (32-2*ASCII_WIDTH)) & ASCII_BITS;
}
inline u8 inline_getAscii2(u32 shortCode) {
	return (shortCode >> (32-3*ASCII_WIDTH)) & ASCII_BITS;
}
inline u8 inline_getLanguage(u32 shortCode) {
	return (shortCode >> REVISION_WIDTH) & LANGUAGE_BITS;
}
inline u8 inline_getRevision(u32 shortCode) {
	return shortCode & REVISION_BITS;
}

bool32 CompareShortCodes(u32 clientShortCode, u32 queryShortCode) {
	u8 queryAscii0 = inline_getAscii0(queryShortCode);
	u8 clientAscii0 = inline_getAscii0(clientShortCode);
	if (queryAscii0) if (queryAscii0 != clientAscii0) return FALSE;

	u8 queryAscii1 = inline_getAscii1(queryShortCode);
	u8 clientAscii1 = inline_getAscii1(clientShortCode);
	if (queryAscii1) if (queryAscii1 != clientAscii1) return FALSE;

	u8 queryAscii2 = inline_getAscii2(queryShortCode);
	u8 clientAscii2 = inline_getAscii2(clientShortCode);
	if (queryAscii2) if (queryAscii2 != clientAscii2) return FALSE;

	u8 queryLanguage = inline_getLanguage(queryShortCode);
	u8 clientLanguage = inline_getLanguage(clientShortCode);
	if (queryLanguage) if (queryLanguage != clientLanguage) return FALSE;

	u8 queryRevision = inline_getRevision(queryShortCode);
	u8 clientRevision = inline_getRevision(clientShortCode);
	if (queryRevision != REVISION_BITS) if (queryRevision != clientRevision) return FALSE;

	return TRUE;
}

//===============================//
// Wonder Card Selection //======//
//===============================//

bool32 ShowWonderCardOrNews(u8 * state, const struct MysteryGiftDistributionData* mgData, bool32 isNews)
{
    s32 v0;
	bool32 cardOrNews = isNews;
	
	struct WonderCard* wonderCard = &mgData->wonderCard;
	struct WonderNews* wonderNews = &mgData->wonderNews;
	struct WonderCardMetadata wonderCardMetadata = {0, 0, 0, wonderCard->iconSpecies, {
		// Yes, it insists on this. No, I don't know why.
		{mgData->wonderCardServerStamps[0], mgData->wonderCardServerStamps[1], mgData->wonderCardServerStamps[2], mgData->wonderCardServerStamps[3], mgData->wonderCardServerStamps[4], mgData->wonderCardServerStamps[5], mgData->wonderCardServerStamps[6]},
		{1,2,3,4,5,6,7}
	}};

    switch (*state)
    {
    case 0:
        if (cardOrNews == 0)
        {
            WonderCard_Init(wonderCard, &wonderCardMetadata);
        }
        else
        {
            WonderNews_Init(wonderNews);
        }
        (*state)++;
        break;
    case 1:
        if (cardOrNews == 0)
        {
            v0 = WonderCard_Enter();
            check:
            if (v0 != 0)
            {
                goto done;
            }
            break;
        }
        else
        {
            v0 = WonderNews_Enter();
            goto check;
        }
    done:
        *state = 0;
        return TRUE;
    }

    return FALSE;
}

//===============================//
// Menu Layout //================//
//===============================//

const u8 gText_NotImplemented[] = _("This feature has not yet been\nimplemented.");

const u8 gText_BerryFixProgram[] = _("Berry Fix Program (R/S)");
const u8 gText_CustomDistributions[] = _("Custom Distributions (FR/LG/E)");
const static u8 sListMenuItems_RootMenu_Count = 2;
const static struct ListMenuItem sListMenuItems_RootMenu[] = {
	{ gText_BerryFixProgram,     0 },
	{ gText_CustomDistributions, 3 }
};

void task00_mystery_gift(u8 taskId) {
	struct MysteryGiftTaskData* data = (void*)gTasks[taskId].data;
	u32 result;
	bool32 isNews = gDistributions_customDistributions[data->source].wonderCard.flagId == 0;
	
	switch (data->state) {
		case  0: // Initial state
			data->state = 1;
			break;
		case  1: // Root Menu
			switch (CreateAndPollListMenu(sListMenuItems_RootMenu, sListMenuItems_RootMenu_Count)) {
				case 0: // Berry Fix
					SetMainCallback2(CB2_GoToBerryFixScreen);
					break;
				case 3: // Custom Distributions
					data->state = 30;
					break;
			}
			break;
		case  2: // Not yet implemented
			if (PrintMysteryGiftMenuMessage(&data->textState, gText_NotImplemented)) {
				data->state = 0;
			}
			break;
		
		case 30: // Custom Distributions
			result = CreateAndPollListMenu(gListMenuItems_customGifts, CUSTOM_DISTRIBUTION_COUNT);
			switch (result) {
				case LIST_CANCEL:
					data->state = 1;
					break;
				case LIST_NOTHING_CHOSEN:
					break;
				default:
					data->source = result;
					data->state = 31;
					break;
			}
			break;
		case 31: // Prepare to Display Wonder Card or News
			if (ShowWonderCardOrNews(&data->textState, &gDistributions_customDistributions[data->source], isNews))
				data->state = 32;
			break;
		case 32: // Display Wonder Card or News
			if (!isNews) {
				if (JOY_NEW(A_BUTTON)) /*data->state = 34 [[!!!]] */;
				if (JOY_NEW(B_BUTTON)) data->state = 33;
			} else switch (WonderNews_GetInput(gMain.newKeys)) {
				case 0:
					break;
				case 1:
					data->state = 33;
					break;
			}
			break;
		case 33: // Return to case 30
			if (ExitWonderCardOrNews(isNews, 0))
				data->state = 30;
			break;
	}
}

