extern const u8 gDistribution1003_MysteryEvent[];

const static struct WonderCard sDistribution1003_WonderCard = {
	.flagId = 1003, // EM_FLAG_WONDER_CARD_UNUSED_1
	.iconSpecies = SPECIES_HO_OH,
	.idNumber = 1003,
	
	.type = CARD_TYPE_STAMP,
	.bgType = WONDER_BG_BLUE,
	.sendType = SEND_TYPE_ALLOWED_ALWAYS,
	.maxStamps = 7,
	
	.titleText = _("OMNI-TICKET$"),
	.subtitleText = _("Exchange Card$"),
	.bodyText = {
		_("Go to the second floor of the POKéMON$"),
		_("CENTER and meet the delivery person.$"),
		_("Receive the TICKETS and then save$"),
		_("the game!!$")
	},
	.footerLine1Text = _("Do not toss before receiving the gift!$"),
	.footerLine2Text = _("Press {A_BUTTON} for a cookie.$")
};

const static struct WonderNews sDistribution1003_WonderNews = {
	.id = 1003,
	.sendType = SEND_TYPE_ALLOWED,
	.bgType = WONDER_BG_BLUE,
	.titleText = _("It's a whole new world we live in…$"),
	.bodyText = {
		_("I've worked on this off-and-on for$"),
		_("a few years, but I've finally done it:$"),
		_("an easily modifiable custom$"),
		_("distribution. I wish to extend my$"),
		_("thanks to ajxpk and all of the members$"),
		_("of pret. I look forward to working on$"),
		_("this project for years to come.$"),
		_("$"),
		_("- WhenGryphonsFly{PLAY_BGM}{MUS_RG_SLOW_PALLET}$"),
		_("P.S. Shoutouts to SimpleFlips$")
	}
};

const static struct MysteryGiftServerCmd sDistribution1003_FRLGE[] = {
	SERVER_MACRO_SEND_CARD(sDistribution1003_WonderCard),
	
	SERVER_MACRO_SEND_STAMP(SPECIES_HO_OH, 1),
	
	SERVER_MACRO_SEND_NEWS(sDistribution1003_WonderNews),
	SERVER_MACRO_SEND_MYSTERY_EVENT(gDistribution1003_MysteryEvent),
	SERVER_MACRO_END_TRANSMISSION
};

const static struct MysteryGiftServerCmd sDistribution1003_InitialServerCommands[] = {
	SERVER_MACRO_GET_VERSION,
	{SERVER_BRANCH_IF_CLIENT_VERSION_MATCHES, SHORT_CODE("\0\0\0J", 0xFF), gDistributionServer_Incompatible},
	{SERVER_BRANCH_IF_CLIENT_VERSION_MATCHES, SHORT_CODE("BPR\0", 0xFF), sDistribution1003_FRLGE},
	{SERVER_BRANCH_IF_CLIENT_VERSION_MATCHES, SHORT_CODE("BPG\0", 0xFF), sDistribution1003_FRLGE},
	{SERVER_BRANCH_IF_CLIENT_VERSION_MATCHES, SHORT_CODE("BPE\0", 0xFF), sDistribution1003_FRLGE},
	{SERVER_BRANCH_ALWAYS, .ptr = gDistributionServer_Incompatible}
};

const struct MysteryGiftDistributionData gDistribution1003 = {
	.name = _("Omni-Ticket$"),
	
	.wonderCardServerStamps = {SPECIES_MEW, SPECIES_LUGIA, SPECIES_HO_OH, 0, SPECIES_DEOXYS, SPECIES_LATIOS, SPECIES_LATIAS},
	.wonderCard = sDistribution1003_WonderCard,
	.wonderNews = sDistribution1003_WonderNews,
	
	.initialServerCommands = sDistribution1003_InitialServerCommands,
};
