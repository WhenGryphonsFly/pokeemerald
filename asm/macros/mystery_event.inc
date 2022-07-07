	@ Does nothing.
	.macro me_nop
	.byte 0x00
	.endm
	
	@ Checks event compatibility. Required as first command.
	.macro me_checkcompat setVirtualAddress:req
	.byte 0x01
	.4byte \setVirtualAddress
	.2byte 1
	.4byte 1
	.2byte 5 @ 0x4 | 0x1
	.4byte 0x203 @ 0x200 | 0x2 | 0x1
	.endm
	
	@ Terminates script execution.
	.macro me_end
	.byte 0x02
	.endm
	
	@ Displays a message if the status provided either is 0xFF or matches the current status.
	.macro me_setmsg status:req, messageVirtualAddress:req
	.byte 0x03
	.byte \status
	.4byte \messageVirtualAddress
	.endm
	
	@ Sets the current execution status
	.macro me_setstatus status:req
	.byte 0x04
	.byte \status
	.endm
	
	@ Executes a Ram Script immediately.
	.macro me_runscript scriptVirtualAddress:req
	.byte 0x05
	.4byte \scriptVirtualAddress
	.endm
	
	@ Saves a Ram Script for later usage, replacing the object event specified.
	.macro me_initramscript mapGroup:req, mapNum:req, objectId:req, scriptStartVirtualAddress:req, scriptEndExclusiveVirtualAddress:req
	.byte 0x06
	.byte \mapGroup
	.byte \mapNum
	.byte \objectId
	.4byte \scriptStartVirtualAddress
	.4byte \scriptEndExclusiveVirtualAddress
	.endm
	
	@ Saves an Enigma Berry. Due to constraints of Mystery Gift, this will never execute successfully.
	.macro me_setenigmaberry berryVirtualAddress:req
	.byte 0x07
	.4byte \berryVirtualAddress
	.endm
	
	@ Gives a Ribbon to all Pokémon in the player's party.
	.macro me_giveribbon ribbonName:req, ribbonFlavorText:req
	.byte 0x08
	.byte \ribbonName
	.byte \ribbonFlavorText
	.endm
	
	@ Gives the National Pokédex to the player.
	.macro me_givenationaldex
	.byte 0x09
	.endm
	
	@ Adds a rare word to the Easy Chat System.
	.macro me_addrareword word:req
	.byte 0x0a
	.byte \word
	.endm
	
	@ Sets the record mixing gift. To receive the gift, the other player must not have the item. Regarding event tickets, only the Eon Ticket properly enables flags. Therefore, only the Eon Ticket or one of the Bikes makes sense.
	.macro me_setrecordmixinggift item=0x113, quantity=255
	.byte 0x0b
	.byte 1
	.byte \quantity
	.2byte \item
	.endm
	
	@ Gives the player a Pokémon, possibly with Mail, if there is space in the party.
	.macro me_givepokemon pokemonVirtualAddress:req
	.byte 0x0c
	.4byte \pokemonVirtualAddress
	.endm
	
	@ Saves a custom trainer.
	.macro me_addtrainer trainerVirtualAddress:req
	.byte 0x0d
	.4byte \trainerVirtualAddress
	.endm
	
	@ Enables the RTC reset function. Press B + Select + Left on the title screen to access the feature.
	.macro me_enableresetrtc
	.byte 0x0e
	.endm
	
	@ Calculates a simple checksum.
	.macro me_checksum checksum:req, protectionStartVirtualAddress:req, protectionEndVirtualAddress:req
	.byte 0x0f
	.4byte checksum
	.4byte protectionStartVirtualAddress
	.4byte protectionEndVirtualAddress
	.endm
	
	@ Calculates a CRC32, because apparently this is serious business.
	.macro me_crc crc:req, protectionStartVirtualAddress:req, protectionEndVirtualAddress:req
	.byte 0x10
	.4byte crc
	.4byte protectionStartVirtualAddress
	.4byte protectionEndVirtualAddress
	.endm
