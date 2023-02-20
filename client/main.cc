#include <cstdio>
#include <cstdint>
#include <string>
#include <unistd.h>
#include <hidapi.h>
#include <stb_image.h>

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

static void syntaxError()
{
    fprintf(stderr, "Syntax error\n");
    exit(1);
}

int main(int argc, char* argv[])
{
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

    if (argc != 2)
        syntaxError();

    if ((argv[1] == std::string("-q")))
    {
        printf("Size: %d x %d, depth: %d\n",
            report.width + 1,
            report.height + 1,
            report.depth);
        exit(0);
    }

    int w, h, n;
    uint8_t* imageData = stbi_load(argv[1], &w, &h, &n, 1);
    if (!imageData)
        perror("cannot open file");

    if ((w != (report.width + 1)) || (h != (report.height + 1)))
    {
        fprintf(stderr,
            "Image must be %d x %d\n",
            report.width + 1,
            report.height + 1);
        exit(1);
    }

    for (int x = 0; x < w; x += 8)
    {
        BlitReport br = {
            BLIT_REPORT_ID, (uint16_t)x, 0, (uint16_t)(x + 8), (uint16_t)h};
        for (int y = 0; y < h; y++)
        {
            uint8_t* ptr = &imageData[x + y * w];
            br.data[y] = (!ptr[0] << 7) | (!ptr[1] << 6) | (!ptr[2] << 5) |
                         (!ptr[3] << 4) | (!ptr[4] << 3) | (!ptr[5] << 2) |
                         (!ptr[6] << 1) | (!ptr[7] << 0);
        }

        for (;;)
        {
            int r = hid_write(dev, (uint8_t*)&br, sizeof(br));
            if (r >= 0)
                break;
            if (errno != EPIPE)
            {
                perror("failed to send image data");
                exit(1);
            }
        }
    }

    return 0;
}
