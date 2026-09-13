#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "QRCodeGen/qrcodegen.h"
#include "TinyPng/TinyPngOut.h"

#define MAX_QR_INPUT_SIZE 2953

int main() {
    char s[MAX_QR_INPUT_SIZE];
    char PATH[2048];
    int SCALE = 1;

    do {
        printf("Enter Input String: ");
        fgets(s, MAX_QR_INPUT_SIZE, stdin);
        s[strlen(s)-1] = '\0';
    } while (strlen(s) <= 0);

    bool has_png_extention = false;
    do {
        printf("Enter Output PATH (must end with .png): ");
        fgets(PATH, 2048, stdin);
        PATH[strlen(PATH)-1] = '\0';
        if (
            (PATH[strlen(PATH)-1] == 'g' || PATH[strlen(PATH)-1] == 'G') &&
            (PATH[strlen(PATH)-2] == 'n' || PATH[strlen(PATH)-1] == 'N') &&
            (PATH[strlen(PATH)-3] == 'p' || PATH[strlen(PATH)-1] == 'P')
            )
                has_png_extention = true;
        else
            printf("Wrong extension try again!\n");
    } while (strlen(PATH) < 2 || !has_png_extention);

    do {
        printf("Enter Image Scale: ");
        scanf("%u", &SCALE);
    } while (SCALE <= 0);

    uint8_t qr[qrcodegen_BUFFER_LEN_MAX];
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
    bool ok = qrcodegen_encodeText(s,
        tempBuffer, qr, qrcodegen_Ecc_MEDIUM,
        qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
    if (!ok)
        return ok;

    FILE *output = fopen(PATH, "wb+");
    if (output == NULL) {
        perror("Couldn't open a file.\n");
        exit(-1);
    }


    int size = qrcodegen_getSize(qr);
    struct TinyPngOut writer;
    enum TinyPngOut_Status status = TinyPngOut_init(&writer, size*SCALE, size*SCALE, output);
    if (status != TINYPNGOUT_OK) {
        perror("Can't Generate The QR Code!\n");
        exit(-1);
    }
    uint8_t rgb_black[3] = {0, 0, 0};
    uint8_t rgb_white[3] = {255, 255, 255};
    for (int y = 0; y < size*SCALE; y++) {
        for (int x = 0; x < size*SCALE; x++) {
            bool color = qrcodegen_getModule(qr, x/SCALE, y/SCALE);
            TinyPngOut_write(&writer, color ? rgb_white : rgb_black, 1);
        }
    }

    fclose(output);
    printf("QR Code Generated Successfully Holding String \"%s\"\nIn path: \"%s\"", s, PATH);
    return 0;
}

// Bouallge Kaissar " It didn't work <:( "
// After some debugging it finally worked :)
