#include "../../stdincl.h"
#include <string.h>
#include <libstrawberry/crypto/hashing/sha256.h>

static uint8_t vectors_input[4][48] = {
	"",
	"The quick brown fox jumps over the lazy dog",
	"The quick brown fox jumps over the lazy dog.",
	"unkncpck"
};
static uint8_t vectors[4][32] = {
	{ 0xE3, 0xB0, 0xC4, 0x42, 0x98, 0xFC, 0x1C, 0x14, 0x9A, 0xFB, 0xF4, 0xC8, 0x99, 0x6F, 0xB9, 0x24, 0x27, 0xAE, 0x41, 0xE4, 0x64, 0x9B, 0x93, 0x4C, 0xA4, 0x95, 0x99, 0x1B, 0x78, 0x52, 0xB8, 0x55 },
	{ 0xD7, 0xA8, 0xFB, 0xB3, 0x07, 0xD7, 0x80, 0x94, 0x69, 0xCA, 0x9A, 0xBC, 0xB0, 0x08, 0x2E, 0x4F, 0x8D, 0x56, 0x51, 0xE4, 0x6D, 0x3C, 0xDB, 0x76, 0x2D, 0x02, 0xD0, 0xBF, 0x37, 0xC9, 0xE5, 0x92 },
	{ 0xEF, 0x53, 0x7F, 0x25, 0xC8, 0x95, 0xBF, 0xA7, 0x82, 0x52, 0x65, 0x29, 0xA9, 0xB6, 0x3D, 0x97, 0xAA, 0x63, 0x15, 0x64, 0xD5, 0xD7, 0x89, 0xC2, 0xB7, 0x65, 0x44, 0x8C, 0x86, 0x35, 0xFB, 0x6C },
	{ 0x2F, 0x69, 0x8C, 0xD0, 0xA6, 0x45, 0x37, 0x77, 0x02, 0x6E, 0x39, 0x37, 0xA6, 0x46, 0x74, 0x10, 0xE9, 0xB9, 0xDB, 0x9B, 0xBC, 0xAC, 0x7B, 0xD8, 0xE8, 0x2A, 0x9A, 0xFB, 0x1F, 0xC3, 0x67, 0x1C }
};

sb_bool_t test_sha256() {
	sb_bool_t valid = sb_true;

	uint8_t buffer[32];

	uint32_t i;
	for (i = 0; i < 4; ++i) {
		sb_crypto_sha256(buffer, vectors_input[i], strlen((char*)vectors_input[i]));
		if (!sb_memequ(buffer, vectors[i], 32)) {
			valid = sb_false;
			status("sha256", "https://en.wikipedia.org/wiki/SHA-2", status_failed);
		} else {
			status("sha256", "https://en.wikipedia.org/wiki/SHA-2", status_passed);
		}
	}

	return valid;
}
