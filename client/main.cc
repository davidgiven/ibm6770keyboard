#include <cstdio>
#include <cstdint>
#include <hidapi.h>

int main(int argc, const char* argv[])
{
	hid_init();

	hid_device* h = hid_open(0x1209, 0x6e01, nullptr);
	if (!h) {
		printf("Unable to open device\n");
		hid_exit();
 		return 1;
	}

	uint8_t buf[] = { 0x41, 0, 1, 2, 4, 5, 6, 7, 8, 9 };
	hid_write(h, buf, sizeof(buf));

}

