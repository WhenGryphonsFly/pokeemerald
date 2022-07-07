@ Even with all four event tickets, it is impossible for the key items pocket to be full.
@ The pocket size is 30, but only 22 items can be present during normal gameplay.
@ Event tickets only raise this to 26.

@ Also, key items cannot be deposited into the PC or given to Pokémon,
@ so no need to check for that either.

gDistribution1004_MysteryEvent::
	me_checkcompat gDistribution1004_MysteryEvent
	@ No checksum/CRC because the wireless adapter already does a checksum
	me_initramscript 0xFF 0xFF 0xFF sDistribution1004_RamScriptStart sDistribution1004_RamScriptEnd
	me_end

sDistribution1004_RamScriptStart:
	setvaddress sDistribution1004_RamScriptStart
	lock
	faceplayer
	checkitem ITEM_ACRO_BIKE
	vgoto_if 1 sDistribution1004_GiveMachBike
	checkitem ITEM_MACH_BIKE
	vgoto_if 1 sDistribution1004_GiveAcroBike
	
	@ Doesn't have either bike
	vmessage sDistribution1004_TextVisitBikeShop
	@ fallthrough
	
sDistribution1004_RamScriptThankYou:
	vmessage sDistribution1004_TextThankYou
	waitmessage
	waitbuttonpress
	release
	end
	
sDistribution1004_TextThankYou:
	.string "Thank you for using the MYSTERY\n"
	.string "GIFT System.$"

sDistribution1004_GiveAcroBike:
	giveitem ITEM_ACRO_BIKE
	vgoto sDistribution1003_RamScriptThankYou

sDistribution1004_GiveMachBike:
	giveitem ITEM_MACH_BIKE
	vgoto sDistribution1003_RamScriptThankYou

sDistribution1004_TextVisitBikeShop:
	.string "I'm sorry, but I am afraid that I\n"
	.string "cannot give you your gift at this\l"
	.string "time.\p"
	.string "Please visit RYDEL'S CYCLES\n"
	.string "before seeing me again.$"

sDistribution1004_RamScriptEnd:
	end
