extern const u8 gDistribution1004_MysteryEvent[];

const static struct WonderCard sDistribution1004_WonderCard = {
	.flagId = 1004, // EM_FLAG_WONDER_CARD_UNUSED_2
	.iconSpecies = SPECIES_LATIOS,
	.idNumber = 1004,
	
	.type = CARD_TYPE_GIFT,
	.bgType = WONDER_BG_BLUE,
	.sendType = SEND_TYPE_ALLOWED_ALWAYS,
	
	.titleText = _("ANOTHER BIKE$"),
	.subtitleText = _("Exchange Card$"),
	.bodyText = {
		_("Go to the second floor of the POKéMON$"),
		_("CENTER and meet the delivery person.$"),
		_("Receive the BIKE and then save the$"),
		_("game!!$")
	},
	.footerLine1Text = _("Do not toss this Exchange Card$"),
	.footerLine2Text = _("before receiving the BIKE!!$")
};

const static struct MysteryGiftServerCmd sDistribution1003_Emerald[] = {
	SERVER_MACRO_SEND_CARD(sDistribution1004_WonderCard),
	SERVER_MACRO_SEND_MYSTERY_EVENT(gDistribution1004_MysteryEvent),
	SERVER_MACRO_END_TRANSMISSION
};

const static struct MysteryGiftServerCmd sDistribution1004_InitialServerCommands[] = {
	SERVER_MACRO_GET_VERSION,
	{SERVER_BRANCH_IF_CLIENT_VERSION_MATCHES, SHORT_CODE("\0\0\0J", 0xFF), gDistributionServer_Incompatible},
	{SERVER_BRANCH_IF_CLIENT_VERSION_MATCHES, SHORT_CODE("BPR\0", 0xFF), gDistributionServer_Incompatible},
	{SERVER_BRANCH_IF_CLIENT_VERSION_MATCHES, SHORT_CODE("BPG\0", 0xFF), gDistributionServer_Incompatible},
	{SERVER_BRANCH_IF_CLIENT_VERSION_MATCHES, SHORT_CODE("BPE\0", 0xFF), sDistribution1003_Emerald},
	{SERVER_BRANCH_ALWAYS, .ptr = gDistributionServer_Incompatible}
};

const struct MysteryGiftDistributionData gDistribution1004 = {
	.name = _("Another Bike$"),
	
	.wonderCardServerStamps = NULL,
	.wonderCard = sDistribution1004_WonderCard,
	.wonderNews = NULL,
	
	.initialServerCommands = sDistribution1004_InitialServerCommands
};
