#ifndef GUARD_SCRIPT_MENU_CONSTANTS_H
#define GUARD_SCRIPT_MENU_CONSTANTS_H

#define MULTICHOICE(name) {name, ARRAY_COUNT(name)}

#define MAX_MULTICHOICE_WIDTH 28

#define MULTI_B_PRESSED  127

// Multichoice Ids
#define MULTI_BRINEY_ON_DEWFORD            0
#define MULTI_PC                           1 // Exit only, populated by CreatePCMultichoice
#define MULTI_ENTERINFO                    2
#define MULTI_CONTEST_INFO                 3
#define MULTI_CONTEST_TYPE                 4
#define MULTI_BASE_PC_NO_REGISTRY          5
#define MULTI_BASE_PC_WITH_REGISTRY        6
#define MULTI_REGISTER_MENU                7
#define MULTI_SSTIDAL_LILYCOVE             8 // Exit only, populated by CreateLilycoveSSTidalMultichoice
#define MULTI_UNUSED_9                     9
#define MULTI_UNUSED_10                    10
#define MULTI_FRONTIER_PASS_INFO           11
#define MULTI_BIKE                         12
#define MULTI_STATUS_INFO                  13
#define MULTI_BRINEY_OFF_DEWFORD           14
#define MULTI_UNUSED_15                    15
#define MULTI_VIEWED_PAINTINGS             16
#define MULTI_YESNOINFO                    17
#define MULTI_BATTLE_MODE                  18
#define MULTI_UNUSED_19                    19
#define MULTI_YESNOINFO_2                  20
#define MULTI_UNUSED_21                    21
#define MULTI_UNUSED_22                    22
#define MULTI_CHALLENGEINFO                23
#define MULTI_LEVEL_MODE                   24
#define MULTI_MECHADOLL1_Q1                25
#define MULTI_MECHADOLL1_Q2                26
#define MULTI_MECHADOLL1_Q3                27
#define MULTI_MECHADOLL2_Q1                28
#define MULTI_MECHADOLL2_Q2                29
#define MULTI_MECHADOLL2_Q3                30
#define MULTI_MECHADOLL3_Q1                31
#define MULTI_MECHADOLL3_Q2                32
#define MULTI_MECHADOLL3_Q3                33
#define MULTI_MECHADOLL4_Q1                34
#define MULTI_MECHADOLL4_Q2                35
#define MULTI_MECHADOLL4_Q3                36
#define MULTI_MECHADOLL5_Q1                37
#define MULTI_MECHADOLL5_Q2                38
#define MULTI_MECHADOLL5_Q3                39
#define MULTI_UNUSED_40                    40
#define MULTI_UNUSED_41                    41
#define MULTI_VENDING_MACHINE              42
#define MULTI_MACH_BIKE_INFO               43
#define MULTI_ACRO_BIKE_INFO               44
#define MULTI_SATISFACTION                 45
#define MULTI_STERN_DEEPSEA                46
#define MULTI_UNUSED_ASH_VENDOR            47 // Replaced by scrollable multichoice
#define MULTI_GAME_CORNER_DOLLS            48
#define MULTI_GAME_CORNER_COINS            49
#define MULTI_HOWS_FISHING                 50
#define MULTI_UNUSED_51                    51
#define MULTI_SSTIDAL_SLATEPORT_WITH_BF    52
#define MULTI_SSTIDAL_BATTLE_FRONTIER      53
#define MULTI_RIGHTLEFT                    54
#define MULTI_GAME_CORNER_TMS              55
#define MULTI_SSTIDAL_SLATEPORT_NO_BF      56
#define MULTI_FLOORS                       57
#define MULTI_SHARDS_R                     58
#define MULTI_SHARDS_Y                     59
#define MULTI_SHARDS_RY                    60
#define MULTI_SHARDS_B                     61
#define MULTI_SHARDS_RB                    62
#define MULTI_SHARDS_YB                    63
#define MULTI_SHARDS_RYB                   64
#define MULTI_SHARDS_G                     65
#define MULTI_SHARDS_RG                    66
#define MULTI_SHARDS_YG                    67
#define MULTI_SHARDS_RYG                   68
#define MULTI_SHARDS_BG                    69
#define MULTI_SHARDS_RBG                   70
#define MULTI_SHARDS_YBG                   71
#define MULTI_SHARDS_RYBG                  72
#define MULTI_TOURNEY_WITH_RECORD          73
#define MULTI_CABLE_CLUB_NO_RECORD_MIX     74
#define MULTI_WIRELESS_NO_RECORD_BERRY     75
#define MULTI_CABLE_CLUB_WITH_RECORD_MIX   76
#define MULTI_WIRELESS_NO_BERRY            77
#define MULTI_WIRELESS_NO_RECORD           78
#define MULTI_WIRELESS_ALL_SERVICES        79
#define MULTI_WIRELESS_MINIGAME            80
#define MULTI_LINK_LEADER                  81
#define MULTI_CONTEST_RANK                 82
#define MULTI_FRONTIER_ITEM_CHOOSE         83
#define MULTI_LINK_CONTEST_INFO            84
#define MULTI_LINK_CONTEST_MODE            85
#define MULTI_FORCED_START_MENU            86
#define MULTI_FRONTIER_GAMBLER_BET         87
#define MULTI_TENT                         88
#define MULTI_UNUSED_SSTIDAL_1             89 // These 4 were replaced by CreateLilycoveSSTidalMultichoice
#define MULTI_UNUSED_SSTIDAL_2             90 //
#define MULTI_UNUSED_SSTIDAL_3             91 //
#define MULTI_UNUSED_SSTIDAL_4             92 //
#define MULTI_FOSSIL                       93
#define MULTI_YESNO                        94
#define MULTI_FRONTIER_RULES               95
#define MULTI_BATTLE_ARENA_RULES           96
#define MULTI_BATTLE_TOWER_RULES           97
#define MULTI_BATTLE_DOME_RULES            98
#define MULTI_BATTLE_FACTORY_RULES         99
#define MULTI_BATTLE_PALACE_RULES          100
#define MULTI_BATTLE_PYRAMID_RULES         101
#define MULTI_BATTLE_PIKE_RULES            102
#define MULTI_GO_ON_RECORD_REST_RETIRE     103
#define MULTI_GO_ON_REST_RETIRE            104
#define MULTI_GO_ON_RECORD_RETIRE          105
#define MULTI_GO_ON_RETIRE                 106
#define MULTI_TOURNEY_NO_RECORD            107
#define MULTI_TV_LATI                      108
#define MULTI_BATTLE_TOWER_FEELINGS        109
#define MULTI_WHERES_RAYQUAZA              110
#define MULTI_SLATEPORT_TENT_RULES         111
#define MULTI_FALLARBOR_TENT_RULES         112
#define MULTI_TAG_MATCH_TYPE               113

// Lilycove SS Tidal Multichoice Selections
#define SSTIDAL_SELECTION_SLATEPORT        0
#define SSTIDAL_SELECTION_BATTLE_FRONTIER  1
#define SSTIDAL_SELECTION_SOUTHERN_ISLAND  2
#define SSTIDAL_SELECTION_NAVEL_ROCK       3
#define SSTIDAL_SELECTION_BIRTH_ISLAND     4
#define SSTIDAL_SELECTION_FARAWAY_ISLAND   5
#define SSTIDAL_SELECTION_EXIT             6
#define SSTIDAL_SELECTION_COUNT            7

// Std String Ids
#define STDSTRING_COOL             0
#define STDSTRING_BEAUTY           1
#define STDSTRING_CUTE             2
#define STDSTRING_SMART            3
#define STDSTRING_TOUGH            4
#define STDSTRING_NORMAL           5
#define STDSTRING_SUPER            6
#define STDSTRING_HYPER            7
#define STDSTRING_MASTER           8
#define STDSTRING_COOL2            9
#define STDSTRING_BEAUTY2          10
#define STDSTRING_CUTE2            11
#define STDSTRING_SMART2           12
#define STDSTRING_TOUGH2           13
#define STDSTRING_ITEMS            14
#define STDSTRING_KEYITEMS         15
#define STDSTRING_POKEBALLS        16
#define STDSTRING_TMHMS            17
#define STDSTRING_BERRIES          18
#define STDSTRING_SINGLE           19
#define STDSTRING_DOUBLE           20
#define STDSTRING_MULTI            21
#define STDSTRING_MULTI_LINK       22
#define STDSTRING_BATTLE_TOWER     23
#define STDSTRING_BATTLE_DOME      24
#define STDSTRING_BATTLE_FACTORY   25
#define STDSTRING_BATTLE_PALACE    26
#define STDSTRING_BATTLE_ARENA     27
#define STDSTRING_BATTLE_PIKE      28
#define STDSTRING_BATTLE_PYRAMID   29

//==============================================================//
// FireRed

#define FR_SCR_MENU_CANCEL 127
#define FR_SCR_MENU_UNSET  255

#define FR_MULTICHOICE_YES_NO                                      0
#define FR_MULTICHOICE_EEVEELUTIONS                                1
#define FR_MULTICHOICE_TRAINER_CARD_ICON_TINT                      2
#define FR_MULTICHOICE_HOF_QUIT                                    3
#define FR_MULTICHOICE_EGGS_QUIT                                   4
#define FR_MULTICHOICE_VICTORIES_QUIT                              5
#define FR_MULTICHOICE_HOF_EGGS_QUIT                               6
#define FR_MULTICHOICE_HOF_VICTORIES_QUIT                          7
#define FR_MULTICHOICE_EGGS_VICTORIES_QUIT                         8
#define FR_MULTICHOICE_HOF_EGGS_VICTORIES_QUIT                     9
#define FR_MULTICHOICE_EXIT                                       10
#define FR_MULTICHOICE_EXIT_2                                     11
#define FR_MULTICHOICE_EXIT_3                                     12
#define FR_MULTICHOICE_BIKE_SHOP                                  13
#define FR_MULTICHOICE_GAME_CORNER_POKEMON_PRIZES                 14
#define FR_MULTICHOICE_TRAINER_SCHOOL_WHITEBOARD                  15
#define FR_MULTICHOICE_YES_NO_INFO                                16
#define FR_MULTICHOICE_SINGLE_DOUBLE_MULTI_INFO_EXIT              17
#define FR_MULTICHOICE_YES_NO_INFO_2                              18
#define FR_MULTICHOICE_CHALLENGE_INFO_EXIT                        19
#define FR_MULTICHOICE_ROOFTOP_B1F                                20
#define FR_MULTICHOICE_HELIX                                      21
#define FR_MULTICHOICE_DOME                                       22
#define FR_MULTICHOICE_AMBER                                      23
#define FR_MULTICHOICE_HELIX_AMBER                                24
#define FR_MULTICHOICE_DOME_AMBER                                 25
#define FR_MULTICHOICE_CELADON_VENDING_MACHINE                    26
#define FR_MULTICHOICE_GAME_CORNER_COIN_PURCHASE_COUNTER          27
#define FR_MULTICHOICE_EXCELLENT_NOT_SO_BAD                       28
#define FR_MULTICHOICE_RIGHT_LEFT                                 29
#define FR_MULTICHOICE_GAME_CORNER_TMPRIZES                       30
#define FR_MULTICHOICE_DEPT_STORE_ELEVATOR                        31
#define FR_MULTICHOICE_THIRSTY_GIRL_FRESH_WATER                   32
#define FR_MULTICHOICE_THIRSTY_GIRL_SODA_POP                      33
#define FR_MULTICHOICE_THIRSTY_GIRL_FRESH_WATER_SODA_POP          34
#define FR_MULTICHOICE_THIRSTY_GIRL_LEMONADE                      35
#define FR_MULTICHOICE_THIRSTY_GIRL_FRESH_WATER_LEMONADE          36
#define FR_MULTICHOICE_THIRSTY_GIRL_SODA_POP_LEMONADE             37
#define FR_MULTICHOICE_THIRSTY_GIRL_FRESH_WATER_SODA_POP_LEMONADE 38
#define FR_MULTICHOICE_TRADE_CENTER_COLOSSEUM                     39
#define FR_MULTICHOICE_LINK_WIRELESS                              40
#define FR_MULTICHOICE_GAME_CORNER_BATTLE_ITEM_PRIZES             41
#define FR_MULTICHOICE_ROCKET_HIDEOUT_ELEVATOR                    42
#define FR_MULTICHOICE_LINKED_DIRECT_UNION                        43
#define FR_MULTICHOICE_ISLAND_23                                  44
#define FR_MULTICHOICE_ISLAND_13                                  45
#define FR_MULTICHOICE_ISLAND_12                                  46
#define FR_MULTICHOICE_TRADE_COLOSSEUM_CRUSH                      47
#define FR_MULTICHOICE_48                                         48
#define FR_MULTICHOICE_POKEJUMP_DODRIO                            49
#define FR_MULTICHOICE_TRADE_COLOSSEUM_2                          50
#define FR_MULTICHOICE_MUSHROOMS                                  51
#define FR_MULTICHOICE_TRADE_COLOSSEUM_BLANK_CRUSH                52
#define FR_MULTICHOICE_TRADE_COLOSSEUM_BLANK                      53
#define FR_MULTICHOICE_SEVII_NAVEL                                54
#define FR_MULTICHOICE_SEVII_BIRTH                                55
#define FR_MULTICHOICE_SEVII_NAVEL_BIRTH                          56
#define FR_MULTICHOICE_SEAGALLOP_123                              57
#define FR_MULTICHOICE_SEAGALLOP_V23                              58
#define FR_MULTICHOICE_SEAGALLOP_V13                              59
#define FR_MULTICHOICE_SEAGALLOP_V12                              60
#define FR_MULTICHOICE_SEAGALLOP_VERMILION                        61
#define FR_MULTICHOICE_62                                         62
#define FR_MULTICHOICE_JOIN_OR_LEAD                               63
#define FR_MULTICHOICE_TRAINER_TOWER_MODE                         64

#define FR_LISTMENU_BADGES                    0
#define FR_LISTMENU_SILPHCO_FLOORS            1
#define FR_LISTMENU_ROCKET_HIDEOUT_FLOORS     2
#define FR_LISTMENU_DEPT_STORE_FLOORS         3
#define FR_LISTMENU_WIRELESS_LECTURE_HEADERS  4
#define FR_LISTMENU_BERRY_POWDER              5
#define FR_LISTMENU_TRAINER_TOWER_FLOORS      6

// Std String Ids
#define FR_STDSTRING_COOL              0
#define FR_STDSTRING_BEAUTY            1
#define FR_STDSTRING_CUTE              2
#define FR_STDSTRING_SMART             3
#define FR_STDSTRING_TOUGH             4
#define FR_STDSTRING_COOL2             5
#define FR_STDSTRING_BEAUTY2           6
#define FR_STDSTRING_CUTE2             7
#define FR_STDSTRING_SMART2            8
#define FR_STDSTRING_TOUGH2            9
#define FR_STDSTRING_ITEMS             10
#define FR_STDSTRING_KEY_ITEMS         11
#define FR_STDSTRING_POKEBALLS         12
#define FR_STDSTRING_TMHMS             13
#define FR_STDSTRING_BERRIES           14
#define FR_STDSTRING_BOULDER_BADGE     15
#define FR_STDSTRING_CASCADE_BADGE     16
#define FR_STDSTRING_THUNDER_BADGE     17
#define FR_STDSTRING_RAINBOW_BADGE     18
#define FR_STDSTRING_SOUL_BADGE        19
#define FR_STDSTRING_MARSH_BADGE       20
#define FR_STDSTRING_VOLCANO_BADGE     21
#define FR_STDSTRING_EARTH_BADGE       22
#define FR_STDSTRING_COINS             23
#define FR_STDSTRING_ITEMS_POCKET      24
#define FR_STDSTRING_KEY_ITEMS_POCKET  25
#define FR_STDSTRING_POKEBALLS_POCKET  26
#define FR_STDSTRING_TM_CASE           27
#define FR_STDSTRING_BERRY_POUCH       28


#endif //GUARD_SCRIPT_MENU_CONSTANTS_H
