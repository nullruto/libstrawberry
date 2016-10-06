
#include "../../stdincl.h"
#include <string.h>
#include <libstrawberry/crypto/hashing/sha384.h>

static uint8_t vectors_input[3][48] = {
	"",
	"The quick brown fox jumps over the lazy dog",
	"The quick brown fox jumps over the lazy dog."
};
static uint8_t vectors[3][48] = {
    { 0x38 , 0xB0 , 0x60 , 0xA7 , 0x51 , 0xAC , 0x96 , 0x38 , 0x4C , 0xD9 , 0x32 , 0x7E , 0xB1 , 0xB1 , 0xE3 , 0x6A , 0x21 , 0xFD , 0xB7 , 0x11 , 0x14 , 0xBE , 0x07 , 0x43 , 0x4C , 0x0C , 0xC7 , 0xBF , 0x63 , 0xF6 , 0xE1 , 0xDA , 0x27 , 0x4E , 0xDE , 0xBF , 0xE7 , 0x6F , 0x65 , 0xFB , 0xD5 , 0x1A , 0xD2 , 0xF1 , 0x48 , 0x98 , 0xB9 , 0x5B },
    { 0xCA , 0x73 , 0x7F , 0x10 , 0x14 , 0xA4 , 0x8F , 0x4C , 0x0B , 0x6D , 0xD4 , 0x3C , 0xB1 , 0x77 , 0xB0 , 0xAF , 0xD9 , 0xE5 , 0x16 , 0x93 , 0x67 , 0x54 , 0x4C , 0x49 , 0x40 , 0x11 , 0xE3 , 0x31 , 0x7D , 0xBF , 0x9A , 0x50 , 0x9C , 0xB1 , 0xE5 , 0xDC , 0x1E , 0x85 , 0xA9 , 0x41 , 0xBB , 0xEE , 0x3D , 0x7F , 0x2A , 0xFB , 0xC9 , 0xB1 },
    { 0xED , 0x89 , 0x24 , 0x81 , 0xD8 , 0x27 , 0x2C , 0xA6 , 0xDF , 0x37 , 0x0B , 0xF7 , 0x06 , 0xE4 , 0xD7 , 0xBC , 0x1B , 0x57 , 0x39 , 0xFA , 0x21 , 0x77 , 0xAA , 0xE6 , 0xC5 , 0x0E , 0x94 , 0x66 , 0x78 , 0x71 , 0x8F , 0xC6 , 0x7A , 0x7A , 0xF2 , 0x81 , 0x9A , 0x02 , 0x1C , 0x2F , 0xC3 , 0x4E , 0x91 , 0xBD , 0xB6 , 0x34 , 0x09 , 0xD7 }
};

sb_bool_t test_sha384() {
	sb_bool_t valid = sb_true;

	uint8_t buffer[48];

	uint32_t i;
	for (i = 0; i < 3; ++i) {
		sb_crypto_sha384(buffer, vectors_input[i], strlen((char*)vectors_input[i]));
		if (!sb_memequ(buffer, vectors[i], 48)) {
			valid = sb_false;
			status("sha384", "https://en.wikipedia.org/wiki/SHA-2", status_failed);
		} else {
			status("sha384", "https://en.wikipedia.org/wiki/SHA-2", status_passed);
		}
	}

	return valid;
}
