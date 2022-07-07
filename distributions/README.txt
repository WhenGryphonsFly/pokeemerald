To create a new distribution:
	* Create __distribution_name__.c and __distribution_name__.inc in the /custom folder
	* Define the Wonder Card, compatibility scripts, etc. in __distribution_name__.c
	* Define the Mystery Event and RAM Script in __distribution_namne__.inc
	
To add this distribution to the list
	* Add the following lines to the top of __distribution_name__.c:
		extern const u8 gDistribution####_MysteryEvent[];
		
	* Below the "// INSERT DISTRIBUTIONS HERE" comment in distributions.c, add:
		gDistribution####,
	* Below the "// INSERT DISTRIBUTION NAMES HERE" comment in distributions.c, add: (# is the next number)
		{ gDistribution####.name, # },
		
	* Below the "// INCLUDE DISTRIBUTION FILES HERE" comment in distribution_list.h, add:
		#include "custom/__distribution_name__.c"
	* Increment CUSTOM_DISTRIBUTION_COUNT in distributions.h
	
	* Below the "@ INSERT DISTRIBUTIONS HERE" comment in events.inc, add:
		.include "distributions/custom/__distribution_name__.inc"

