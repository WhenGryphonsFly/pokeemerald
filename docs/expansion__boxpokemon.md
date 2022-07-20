### Credits

* hjk321, for [this commit](https://github.com/hjk321/pokeemerald/commit/9782dd87fce7e6e897794cd345fbfc71d622d449) confirming which functions needed to be altered.

### Changelog

* Reduce the size of BoxPokemon to 60 bytes by:
    * Removing superfluous information and filler
    * Removing unused and tournament-only ribbons
    * Removing checksum-related information
    * Bitpacking various fields
    * Reimplementing Pokérus to facilitate bitpacking
* Add a `forme` field to facilitate Deoxys/Arceus/etc. formes
* Expand `metLocation` from 8 to 9 bits wide (necessary supporting functions not yet expanded)
* Add preprocessor directives to make removing unneeded fields easier
    * OT name, OT gender, Pokérus, and markings can be removed to save an additional 8 bytes
    * Contest stats and ribbons can be removed to save an additional 8 bytes
    * Met information, non-contest ribbons, and 3 bits of the `forme` field can be removed to save an additional 4 bytes

### Notes

* The `species` field is only 9 bits wide, meaning only generation IV Pokémon can be added without further modification.
    * If additional Pokémon are desired, the recommendation is to reorder the items in `/include/constants/items.h` such that all holdable items have a value less than 256. This would allow the changing of `u32 species:9; u32 heldItem:9;` to `u32 species:10; u32 heldItem:8;`.
    * Alternatively, `metLocation` could be reduced back down to 8 bits, and `species` could be split similarly to `forme`.
* By default, Pokémon may have up to 32 formes. However, if 3 bits of `forme` are removed as described above, Pokémon will be limited to only 4 formes.
