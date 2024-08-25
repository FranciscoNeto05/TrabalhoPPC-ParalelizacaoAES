#include "encrypt.hpp"
#include "decrypt.hpp"
#include "encryptlinear.hpp"
#include "decryptlinear.hpp"
#include "expansao.hpp"
#include <cstring>
#include <iostream>
#include <omp.h>
#define TAMANHO_BLOCO 16
using namespace std;

void coletarTexto(string &textoEntrada, size_t &tamanhoTexto,
                  unsigned char *entrada) {
  cout << "Digite o texto para criptografar: ";
  getline(cin, textoEntrada);
  tamanhoTexto = textoEntrada.size();
  if (tamanhoTexto > TAMANHO_BLOCO) {
    throw runtime_error(
        "Erro: o texto de entrada deve ter no máximo 16 bytes.");
  }
  memset(entrada, 0, TAMANHO_BLOCO);
  memcpy(entrada, textoEntrada.c_str(), tamanhoTexto);
}

void mostrarCriptografia(unsigned char *saida) {
  cout << "Saída criptografada: ";
  for (int i = 0; i < TAMANHO_BLOCO; i++) {
    printf("%02x", saida[i]);
  }
  cout << endl;
}

void mostrarDescriptografia(unsigned char *saidaDescriptografada) {
  cout << "Saída descriptografada: ";
  for (int i = 0; i < TAMANHO_BLOCO; i++) {
    if (saidaDescriptografada[i] != '\0') {
      cout << saidaDescriptografada[i];
    }
  }
  cout << endl;
}

int main() {
  omp_set_num_threads(4);
  string textoEntrada;
  size_t tamanhoTexto;
  unsigned char entrada[TAMANHO_BLOCO];
  unsigned char saida[TAMANHO_BLOCO];
  unsigned char saidaDescriptografada[TAMANHO_BLOCO];

  try {
    coletarTexto(textoEntrada, tamanhoTexto, entrada);
    cout << "\nExecução Paralela:\n";
    gerarChaves();
    criptografarAES(entrada, saida, chaveExpandida);
    mostrarCriptografia(saida);
    descriptografarAES(saida, saidaDescriptografada, chaveExpandida);
    mostrarDescriptografia(saidaDescriptografada);
  } catch (const runtime_error &e) {
    cerr << e.what() << endl;
    return 1;
  }

  cout << "\nExecução Linear:\n";
  try {
    gerarChaves();
    LinearCriptografarAES(entrada, saida, chaveExpandida);
    mostrarCriptografia(saida);
    LinearDescriptografarAES(saida, saidaDescriptografada, chaveExpandida);
    mostrarDescriptografia(saidaDescriptografada);
  } catch (const runtime_error &e) {
    cerr << e.what() << endl;
    return 1;
  }
  return 0;
}