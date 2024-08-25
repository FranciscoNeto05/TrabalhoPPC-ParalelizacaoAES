#ifndef EXPANSAO_HPP
#define EXPANSAO_HPP

#include <openssl/aes.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <string.h>

#define TAMANHO_CHAVE 32
#define TAMANHO_EXPANDIDO 240
#define TAMANHO_BITS_CHAVE 256

unsigned char chave[TAMANHO_CHAVE];
unsigned char chaveExpandida[TAMANHO_EXPANDIDO];

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
void mostrarChaveGerada () {
    printf("Chave Gerada: ");
    for (unsigned long i=0; i<sizeof(chave); i++) {
        printf("%02x", chave[i]);
    }
    printf("\n\n");
}

void mostrarChaveExpandida () {
    printf("Chave Expandida:\n");
    for (int i = 0; i < TAMANHO_EXPANDIDO; i++) {
        printf("%02x", chaveExpandida[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
}

void copiarChaveExpandida (AES_KEY& chaveAES) {
    for (unsigned long i=0; i<TAMANHO_EXPANDIDO / sizeof(unsigned int); i++) {
        chaveExpandida[i * 4]     = (unsigned char)((chaveAES.rd_key[i] >> 24) & 0xFF);
        chaveExpandida[i * 4 + 1] = (unsigned char)((chaveAES.rd_key[i] >> 16) & 0xFF);
        chaveExpandida[i * 4 + 2] = (unsigned char)((chaveAES.rd_key[i] >> 8) & 0xFF);
        chaveExpandida[i * 4 + 3] = (unsigned char)(chaveAES.rd_key[i] & 0xFF);
    }
}

void gerarChaveAES() {
    if (!RAND_bytes(chave, TAMANHO_CHAVE)) {
        fprintf(stderr, "Erro ao gerar chave AES.\n");
        return;
    }
}

void expandirChaveAES(AES_KEY& chaveAES) {
    if (AES_set_encrypt_key(chave, TAMANHO_BITS_CHAVE, &chaveAES)) {
        fprintf(stderr, "Erro ao expandir chave AES.\n");
        return;
    }
}

void gerarChaves () {
    AES_KEY chaveAES;
    gerarChaveAES();
    //mostrarChaveGerada();
    expandirChaveAES(chaveAES);
    copiarChaveExpandida(chaveAES);
    //mostrarChaveExpandida();
}
#pragma GCC diagnostic pop
#endif