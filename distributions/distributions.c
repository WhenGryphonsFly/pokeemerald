#include "distributions.h"

// Yes, I'm aware of how dangerous this is. Due to how structs are never true constants in C - and how creating #defines for a struct this complicated would be even more dangerous - I am required to do this.
#define INCLUDE_C_FILES
#include "distribution_list.h"
#undef INCLUDE_C_FILES

const struct MysteryGiftDistributionData gDistributions_customDistributions[] = {
	// INSERT DISTRIBUTIONS HERE
	gDistribution1003,
	gDistribution1004,
};

const struct ListMenuItem gListMenuItems_customGifts[] = {
	// INSERT DISTRIBUTION NAMES HERE
	{ gDistribution1003.name, 0 },
	{ gDistribution1004.name, 1 },
	
};
