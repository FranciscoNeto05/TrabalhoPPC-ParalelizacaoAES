#ifndef ENCRYPT_HPP
#define ENCRYPT_HPP

#include "sbox.hpp"
#include <cstdio>
#include <cstring>
#include <omp.h>

void SubstituirBytes(unsigned char estado[4][4]) {
#pragma omp parallel for collapse(2)
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      estado[i][j] = AES_Sbox[estado[i][j]];
    }
  }
}

void DeslocarLinhas(unsigned char estado[4][4]) {
  unsigned char temp;

#pragma omp parallel sections
  {
#pragma omp section
    {
      temp = estado[1][0];
      estado[1][0] = estado[1][1];
      estado[1][1] = estado[1][2];
      estado[1][2] = estado[1][3];
      estado[1][3] = temp;
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
      estado[3][0] = estado[3][3];
      estado[3][3] = estado[3][2];
      estado[3][2] = estado[3][1];
      estado[3][1] = temp;
    }
  }
}

unsigned char multiplicarGalois(unsigned char a, unsigned char b) {
  unsigned char p = 0;
  unsigned char bitMaisAltoSetado;
  for (int contador = 0; contador < 8; contador++) {
    if (b & 1) {
      p ^= a;
    }
    bitMaisAltoSetado = (a & 0x80);
    a <<= 1;
    if (bitMaisAltoSetado) {
      a ^= 0x1b;
    }
    b >>= 1;
  }
  return p;
}

void MisturarColunas(unsigned char estado[4][4]) {
  unsigned char temp[4];
#pragma omp parallel for private(temp)
  for (int i = 0; i < 4; i++) {
    temp[0] = multiplicarGalois(estado[0][i], 2) ^
              multiplicarGalois(estado[1][i], 3) ^ estado[2][i] ^ estado[3][i];
    temp[1] = estado[0][i] ^ multiplicarGalois(estado[1][i], 2) ^
              multiplicarGalois(estado[2][i], 3) ^ estado[3][i];
    temp[2] = estado[0][i] ^ estado[1][i] ^ multiplicarGalois(estado[2][i], 2) ^
              multiplicarGalois(estado[3][i], 3);
    temp[3] = multiplicarGalois(estado[0][i], 3) ^ estado[1][i] ^ estado[2][i] ^
              multiplicarGalois(estado[3][i], 2);

    for (int j = 0; j < 4; j++) {
      estado[j][i] = temp[j];
    }
  }
}

void AdicionarChaveRodada(unsigned char estado[4][4],
                          unsigned char *chaveRodada) {
#pragma omp parallel for collapse(2)
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      estado[i][j] ^= chaveRodada[i + 4 * j];
    }
  }
}

void criptografarAES(unsigned char *entrada, unsigned char *saida,
                     unsigned char *chaveExpandida) {
  unsigned char estado[4][4];

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      estado[j][i] = entrada[i * 4 + j];
    }
  }

  AdicionarChaveRodada(estado, chaveExpandida);

  for (int rodada = 1; rodada < 14; rodada++) {
    SubstituirBytes(estado);
    DeslocarLinhas(estado);
    MisturarColunas(estado);
    AdicionarChaveRodada(estado, chaveExpandida + (rodada * 16));
  }

  SubstituirBytes(estado);
  DeslocarLinhas(estado);
  AdicionarChaveRodada(estado, chaveExpandida + 14 * 16);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      saida[i * 4 + j] = estado[j][i];
    }
  }
}

#endif