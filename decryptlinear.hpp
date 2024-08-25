#ifndef DECRYPTLINEAR_HPP
#define DECRYPTLINEAR_HPP

#include "sbox.hpp"
#include <cstdio>
#include <cstring>

void LinearSubstituirBytesInverso(unsigned char estado[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      estado[i][j] = AES_InvSbox[estado[i][j]];
    }
  }
}

void LinearDeslocarLinhasInverso(unsigned char estado[4][4]) {
  unsigned char temp;

  temp = estado[1][3];
  estado[1][3] = estado[1][2];
  estado[1][2] = estado[1][1];
  estado[1][1] = estado[1][0];
  estado[1][0] = temp;

  temp = estado[2][0];
  estado[2][0] = estado[2][2];
  estado[2][2] = temp;
  temp = estado[2][1];
  estado[2][1] = estado[2][3];
  estado[2][3] = temp;

  temp = estado[3][0];
  estado[3][0] = estado[3][1];
  estado[3][1] = estado[3][2];
  estado[3][2] = estado[3][3];
  estado[3][3] = temp;
}

void LinearMisturarColunasInverso(unsigned char estado[4][4]) {
  unsigned char temp[4];
  for (int i = 0; i < 4; i++) {
    temp[0] = LinearMultiplicarGalois(estado[0][i], 0x0e) ^
              LinearMultiplicarGalois(estado[1][i], 0x0b) ^
              LinearMultiplicarGalois(estado[2][i], 0x0d) ^
              LinearMultiplicarGalois(estado[3][i], 0x09);
    temp[1] = LinearMultiplicarGalois(estado[0][i], 0x09) ^
              LinearMultiplicarGalois(estado[1][i], 0x0e) ^
              LinearMultiplicarGalois(estado[2][i], 0x0b) ^
              LinearMultiplicarGalois(estado[3][i], 0x0d);
    temp[2] = LinearMultiplicarGalois(estado[0][i], 0x0d) ^
              LinearMultiplicarGalois(estado[1][i], 0x09) ^
              LinearMultiplicarGalois(estado[2][i], 0x0e) ^
              LinearMultiplicarGalois(estado[3][i], 0x0b);
    temp[3] = LinearMultiplicarGalois(estado[0][i], 0x0b) ^
              LinearMultiplicarGalois(estado[1][i], 0x0d) ^
              LinearMultiplicarGalois(estado[2][i], 0x09) ^
              LinearMultiplicarGalois(estado[3][i], 0x0e);

    for (int j = 0; j < 4; j++) {
      estado[j][i] = temp[j];
    }
  }
}

void LinearDescriptografarAES(unsigned char *entrada, unsigned char *saida,
                              unsigned char *chaveExpandida) {
  unsigned char estado[4][4];

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      estado[j][i] = entrada[i * 4 + j];
    }
  }

  LinearAdicionarChaveRodada(estado, chaveExpandida + 14 * 16);

  for (int rodada = 13; rodada > 0; rodada--) {
    LinearDeslocarLinhasInverso(estado);
    LinearSubstituirBytesInverso(estado);
    LinearAdicionarChaveRodada(estado, chaveExpandida + (rodada * 16));
    LinearMisturarColunasInverso(estado);
  }

  LinearDeslocarLinhasInverso(estado);
  LinearSubstituirBytesInverso(estado);
  LinearAdicionarChaveRodada(estado, chaveExpandida);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      saida[i * 4 + j] = estado[j][i];
    }
  }
}

#endif