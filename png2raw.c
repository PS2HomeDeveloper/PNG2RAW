#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// _CRT_SECURE_NO_WARNINGS  // Put this to project settings->C/C++->Preprocessor->Preprocessor Definitions to avoid the warning of unsafe function on fopen

int main(int argc, char** argv)
{
    if (argc < 4 || argv[1] == "--help" || argv[1] == "-h")
    {
        printf("Usage: ./png2raw file.png output.raw rgb|rgba\n");
        return 1;
    }

    const char* input = argv[1];
    const char* output = argv[2];
    int want_channels = 3;

    // Determine desired channels
    if (strcmp(argv[3], "rgba") == 0)
    {
        want_channels = 4;
    }
    else if (strcmp(argv[3], "rgb") == 0)
    {
        want_channels = 3;
    }
    else
    {
        printf("The last argument must be 'rgb' or 'rgba'.\n");
        return 1;
    }

    // Load image data
    int w, h, channels;
    unsigned char* data = stbi_load(input, &w, &h, &channels, want_channels);
    if (!data)
    {
        printf("Failure on loading image %s\n", input);
        printf("Failure reason: %s\n", stbi_failure_reason());
        return 1;
    }

    // Write to raw file
    FILE* f = fopen(output, "wb");
    if (f == NULL)
    {
        printf("Failure on creating file %s\n", output);
        stbi_image_free(data);
        return 1;
    }

    // Write image data
    size_t img_size = w * h * want_channels;
    fwrite(data, 1, img_size, f);

    // Clean up
    fclose(f);
    stbi_image_free(data);

    printf("Sucssfully converted: %s -> %s (%dx%d, %s)\n", input, output, w, h, (want_channels == 3) ? "RGB" : "RGBA");
    return 0;
}