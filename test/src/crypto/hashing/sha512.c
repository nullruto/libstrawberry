
#include "../../stdincl.h"
#include <string.h>
#include <libstrawberry/crypto/hashing/sha512.h>

static uint8_t vectors_input[3][48] = {
	"",
	"The quick brown fox jumps over the lazy dog",
	"The quick brown fox jumps over the lazy dog."
};
static uint8_t vectors[3][64] = {
	{ 0xCF, 0x83, 0xE1, 0x35, 0x7E, 0xEF, 0xB8, 0xBD, 0xF1, 0x54, 0x28, 0x50, 0xD6, 0x6D, 0x80, 0x07, 0xD6, 0x20, 0xE4, 0x05, 0x0B, 0x57, 0x15, 0xDC, 0x83, 0xF4, 0xA9, 0x21, 0xD3, 0x6C, 0xE9, 0xCE, 0x47, 0xD0, 0xD1, 0x3C, 0x5D, 0x85, 0xF2, 0xB0, 0xFF, 0x83, 0x18, 0xD2, 0x87, 0x7E, 0xEC, 0x2F, 0x63, 0xB9, 0x31, 0xBD, 0x47, 0x41, 0x7A, 0x81, 0xA5, 0x38, 0x32, 0x7A, 0xF9, 0x27, 0xDA, 0x3E },
	{ 0x07, 0xE5, 0x47, 0xD9, 0x58, 0x6F, 0x6A, 0x73, 0xF7, 0x3F, 0xBA, 0xC0, 0x43, 0x5E, 0xD7, 0x69, 0x51, 0x21, 0x8F, 0xB7, 0xD0, 0xC8, 0xD7, 0x88, 0xA3, 0x09, 0xD7, 0x85, 0x43, 0x6B, 0xBB, 0x64, 0x2E, 0x93, 0xA2, 0x52, 0xA9, 0x54, 0xF2, 0x39, 0x12, 0x54, 0x7D, 0x1E, 0x8A, 0x3B, 0x5E, 0xD6, 0xE1, 0xBF, 0xD7, 0x09, 0x78, 0x21, 0x23, 0x3F, 0xA0, 0x53, 0x8F, 0x3D, 0xB8, 0x54, 0xFE, 0xE6 },
	{ 0x91, 0xEA, 0x12, 0x45, 0xF2, 0x0D, 0x46, 0xAE, 0x9A, 0x03, 0x7A, 0x98, 0x9F, 0x54, 0xF1, 0xF7, 0x90, 0xF0, 0xA4, 0x76, 0x07, 0xEE, 0xB8, 0xA1, 0x4D, 0x12, 0x89, 0x0C, 0xEA, 0x77, 0xA1, 0xBB, 0xC6, 0xC7, 0xED, 0x9C, 0xF2, 0x05, 0xE6, 0x7B, 0x7F, 0x2B, 0x8F, 0xD4, 0xC7, 0xDF, 0xD3, 0xA7, 0xA8, 0x61, 0x7E, 0x45, 0xF3, 0xC4, 0x63, 0xD4, 0x81, 0xC7, 0xE5, 0x86, 0xC3, 0x9A, 0xC1, 0xED }
};

sb_bool_t test_sha512() {
	sb_bool_t valid = sb_true;

	uint8_t buffer[64];

	uint32_t i;
	for (i = 0; i < 3; ++i) {
		sb_crypto_sha512(buffer, vectors_input[i], strlen((char*)vectors_input[i]));
		if (!sb_memequ(buffer, vectors[i], 64)) {
			valid = sb_false;
			status("sha512", "https://en.wikipedia.org/wiki/SHA-2", status_failed);
		} else {
			status("sha512", "https://en.wikipedia.org/wiki/SHA-2", status_passed);
		}
	}

	return valid;
}
