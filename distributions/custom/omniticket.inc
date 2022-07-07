gDistribution1003_MysteryEvent::
	me_checkcompat gDistribution1003_MysteryEvent
	@ No checksum/CRC because the wireless adapter already does a checksum
	me_initramscript 0xFF 0xFF 0xFF sDistribution1003_RamScriptStart sDistribution1003_RamScriptEnd
	me_setrecordmixinggift @ Will this ever execute? [[!!!]]
	me_end
	
sDistribution1003_RamScriptStart:
	setvaddress sDistribution1003_RamScriptStart
	lock
	faceplayer
	compare_ptr_to_value 0x80000AE 'E'
	vgoto_if 5 sDistribution1003_FireRedStart
	checkflag FLAG_ENABLE_SHIP_SOUTHERN_ISLAND
	vcall_if 5 sDistribution1003_EmeraldEonTicket
	checkflag FLAG_ENABLE_SHIP_NAVEL_ROCK
	vcall_if 5 sDistribution1003_EmeraldMysticTicket
	checkflag FLAG_ENABLE_SHIP_BIRTH_ISLAND
	vcall_if 5 sDistribution1003_EmeraldAuroraTicket
	checkflag FLAG_ENABLE_SHIP_FARAWAY_ISLAND
	vcall_if 5 sDistribution1003_EmeraldOldSeaMap
	special EnableNationalPokedex
	vgoto sDistribution1003_RamScriptThankYou

sDistribution1003_FireRedStart:
	checkflag FR_FLAG_ENABLE_SHIP_NAVEL_ROCK
	vcall_if 5 sDistribution1003_FireRedMysticTicket
	checkflag FR_FLAG_ENABLE_SHIP_BIRTH_ISLAND
	vcall_if 5 sDistribution1003_FireRedAuroraTicket
	@ special EnableNationalDex [[!!!]]
	@ fallthrough

sDistribution1003_RamScriptThankYou:
	vmessage sDistribution1003_TextThankYou
	waitmessage
	waitbuttonpress
	release
	end
	
sDistribution1003_TextThankYou:
	.string "Thank you for using the MYSTERY\n"
	.string "GIFT System.$"
	
sDistribution1003_EmeraldEonTicket:
	giveitem ITEM_EON_TICKET
	setflag FLAG_ENABLE_SHIP_SOUTHERN_ISLAND
	return

sDistribution1003_EmeraldMysticTicket:
	giveitem ITEM_MYSTIC_TICKET
	setflag FLAG_ENABLE_SHIP_NAVEL_ROCK
	return

sDistribution1003_EmeraldAuroraTicket:
	giveitem ITEM_AURORA_TICKET
	setflag FLAG_ENABLE_SHIP_BIRTH_ISLAND
	return

sDistribution1003_EmeraldOldSeaMap:
	giveitem ITEM_OLD_SEA_MAP
	setflag FLAG_ENABLE_SHIP_FARAWAY_ISLAND
	return

sDistribution1003_FireRedMysticTicket:
	giveitem ITEM_MYSTIC_TICKET
	setflag FR_FLAG_ENABLE_SHIP_NAVEL_ROCK
	return

sDistribution1003_FireRedAuroraTicket:
	giveitem ITEM_AURORA_TICKET
	setflag FR_FLAG_ENABLE_SHIP_BIRTH_ISLAND
	return

sDistribution1003_RamScriptEnd:
	end
