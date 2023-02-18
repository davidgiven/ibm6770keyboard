#include <cstdio>
#include <cstdint>
#include <string>
#include <hidapi.h>
#include <netpbm/pbm.h>

static constexpr uint8_t DISPLAY_ATTRIBUTES_REPORT_ID = 21;
static constexpr uint8_t BLIT_REPORT_ID = 22;

typedef struct
{
    uint8_t reportId;
    uint16_t width;
    uint16_t height;
    uint16_t depth;
} __attribute__((__packed__)) DisplayAttributesReport;

typedef struct
{
    uint8_t reportId;
    uint16_t x1, y1;
    uint16_t x2, y2;
    uint8_t data[50];
} __attribute__((__packed__)) BlitReport;

int main(int argc, char* argv[])
{
    pbm_init(&argc, argv);
    hid_init();

    hid_device* dev = hid_open(0x1209, 0x6e01, nullptr);
    if (!dev)
    {
        printf("Unable to open device\n");
        hid_exit();
        return 1;
    }

    DisplayAttributesReport report = {DISPLAY_ATTRIBUTES_REPORT_ID};
    if (hid_get_feature_report(dev, (uint8_t*)&report, sizeof(report)) < 0)
    {
        perror("failed to query device");
        exit(1);
    }

    if ((argc >= 2) && (argv[1] == std::string("-q")))
    {
        printf("Size: %d x %d, depth: %d\n",
            report.width,
            report.height,
            report.depth);
        exit(0);
    }

    int w, h;
    uint8_t** imageData = pbm_readpbm(stdin, &w, &h);

    if ((w != (report.width+1)) || (h != (report.height+1)))
    {
        fprintf(stderr, "Image must be %d x %d\n", report.width, report.height);
        exit(1);
    }

    for (uint16_t x = 0; x < w; x += 8)
    {
        BlitReport br = {BLIT_REPORT_ID, x, 0, (uint16_t)(x + 8), (uint16_t)h};
        for (uint16_t y = 0; y < h; y++)
            br.data[y] = imageData[y][x / 8];

        if (hid_write(dev, (uint8_t*)&br, sizeof(br)) < 0)
        {
            perror("failed to send image data");
            exit(1);
        }
    }

    return 0;
}
