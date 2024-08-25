#ifndef DECRYPT_HPP
#define DECRYPT_HPP

#include <cstdio>
#include <cstring>
#include <omp.h>
#include "sbox.hpp"

void SubstituirBytesInverso(unsigned char estado[4][4]) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            estado[i][j] = AES_InvSbox[estado[i][j]];
        }
    }
}

void DeslocarLinhasInverso(unsigned char estado[4][4]) {
    unsigned char temp;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            temp = estado[1][3];
            estado[1][3] = estado[1][2];
            estado[1][2] = estado[1][1];
            estado[1][1] = estado[1][0];
            estado[1][0] = temp;
        }
        #pragma omp section
        {
            temp = estado[2][0];
            estado[2][0] = estado[2][2];
            estado[2][2] = temp;
            temp = estado[2][1];
            estado[2][1] = estado[2][3];
            estado[2][3] = temp;
        }
        #pragma omp section
        {
            temp = estado[3][0];
            estado[3][0] = estado[3][1];
            estado[3][1] = estado[3][2];
            estado[3][2] = estado[3][3];
            estado[3][3] = temp;
        }
    }
}

void MisturarColunasInverso(unsigned char estado[4][4]) {
    unsigned char temp[4];
    #pragma omp parallel for private(temp)
    for (int i = 0; i < 4; i++) {
        temp[0] = multiplicarGalois(estado[0][i], 0x0e) ^ multiplicarGalois(estado[1][i], 0x0b) ^ multiplicarGalois(estado[2][i], 0x0d) ^ multiplicarGalois(estado[3][i], 0x09);
        temp[1] = multiplicarGalois(estado[0][i], 0x09) ^ multiplicarGalois(estado[1][i], 0x0e) ^ multiplicarGalois(estado[2][i], 0x0b) ^ multiplicarGalois(estado[3][i], 0x0d);
        temp[2] = multiplicarGalois(estado[0][i], 0x0d) ^ multiplicarGalois(estado[1][i], 0x09) ^ multiplicarGalois(estado[2][i], 0x0e) ^ multiplicarGalois(estado[3][i], 0x0b);
        temp[3] = multiplicarGalois(estado[0][i], 0x0b) ^ multiplicarGalois(estado[1][i], 0x0d) ^ multiplicarGalois(estado[2][i], 0x09) ^ multiplicarGalois(estado[3][i], 0x0e);

        for (int j = 0; j < 4; j++) {
            estado[j][i] = temp[j];
        }
    }
}

void descriptografarAES(unsigned char* entrada, unsigned char* saida, unsigned char* chaveExpandida) {
    unsigned char estado[4][4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            estado[j][i] = entrada[i * 4 + j];
        }
    }

    AdicionarChaveRodada(estado, chaveExpandida + 14 * 16);

    for (int rodada = 13; rodada > 0; rodada--) {
        DeslocarLinhasInverso(estado);
        SubstituirBytesInverso(estado);
        AdicionarChaveRodada(estado, chaveExpandida + (rodada * 16));
        MisturarColunasInverso(estado);
    }

    DeslocarLinhasInverso(estado);
    SubstituirBytesInverso(estado);
    AdicionarChaveRodada(estado, chaveExpandida);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            saida[i * 4 + j] = estado[j][i];
        }
    }
}

#endif