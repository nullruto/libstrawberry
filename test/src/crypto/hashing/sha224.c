#include "../../stdincl.h"
#include <string.h>
#include <libstrawberry/crypto/hashing/sha224.h>

static uint8_t vectors_input[3][48] = {
	"",
	"The quick brown fox jumps over the lazy dog",
	"The quick brown fox jumps over the lazy dog."
};
static uint8_t vectors[3][28] = {
	{ 0xD1 , 0x4A , 0x02 , 0x8C , 0x2A , 0x3A , 0x2B , 0xC9 , 0x47 , 0x61 , 0x02 , 0xBB , 0x28 , 0x82 , 0x34 , 0xC4 , 0x15 , 0xA2 , 0xB0 , 0x1F , 0x82 , 0x8E , 0xA6 , 0x2A , 0xC5 , 0xB3 , 0xE4 , 0x2F },
	{ 0x73 , 0x0E , 0x10 , 0x9B , 0xD7 , 0xA8 , 0xA3 , 0x2B , 0x1C , 0xB9 , 0xD9 , 0xA0 , 0x9A , 0xA2 , 0x32 , 0x5D , 0x24 , 0x30 , 0x58 , 0x7D , 0xDB , 0xC0 , 0xC3 , 0x8B , 0xAD , 0x91 , 0x15 , 0x25 },
	{ 0x61 , 0x9C , 0xBA , 0x8E , 0x8E , 0x05 , 0x82 , 0x6E , 0x9B , 0x8C , 0x51 , 0x9C , 0x0A , 0x5C , 0x68 , 0xF4 , 0xFB , 0x65 , 0x3E , 0x8A , 0x3D , 0x8A , 0xA0 , 0x4B , 0xB2 , 0xC8 , 0xCD , 0x4C }
};

sb_bool_t test_sha224() {
	sb_bool_t valid = sb_true;

	uint8_t buffer[28];

	uint32_t i;
	for (i = 0; i < 3; ++i) {
		sb_crypto_sha224(buffer, vectors_input[i], strlen((char*)vectors_input[i]));
		if (!sb_memequ(buffer, vectors[i], 28)) {
			valid = sb_false;
			status("sha224", "https://en.wikipedia.org/wiki/SHA-2", status_failed);
		} else {
			status("sha224", "https://en.wikipedia.org/wiki/SHA-2", status_passed);
		}
	}

	return valid;
}
