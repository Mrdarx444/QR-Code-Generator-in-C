#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "QRCodeGen/qrcodegen.h"
#include "TinyPng/TinyPngOut.h"

void draw(bool bit) {
    if (bit) {
        printf("1 ");
    } else {
        printf("0  ");
    }
}

int main() {
    // Text data
    uint8_t qr[qrcodegen_BUFFER_LEN_MAX];
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
    bool ok = qrcodegen_encodeText("Hello, gdf gd fg dg fd gd gWorld",
        tempBuffer, qr, qrcodegen_Ecc_MEDIUM,
        qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_3, true);
    if (!ok)
        return ok;

    FILE *output = fopen("out.png", "wb+");
    if (output == NULL) {
        perror("Couldn't open a file.\n");
        exit(-1);
    }


    int size = qrcodegen_getSize(qr);
    struct TinyPngOut writer;
    int SCALE = 10;
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
    return 0;
}

// Bouallge Kaissar
