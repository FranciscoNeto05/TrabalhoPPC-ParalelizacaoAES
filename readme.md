# 💻 **Trabalho de Programação Paralela e Concorrente**

Bem-vindo ao repositório do projeto de **Programação Paralela e Concorrente**! Este projeto apresenta uma implementação paralela do **Algoritmo Criptográfico Advanced Encryption Standard (AES)**, focando especificamente na versão **AES-256**.

## 📜 **Descrição do Projeto**

Esta implementação explora o poder da paralelização para otimizar operações criptográficas usando o **AES-256**, um padrão amplamente utilizado por sua segurança robusta. O código foi desenvolvido para criptografar e descriptografar dados de maneira eficiente, utilizando técnicas tradicionais e paralelizadas com a biblioteca **OpenMP**.

## 🛠️ **Tecnologias Utilizadas**

- **C++**: Linguagem de programação principal utilizada no projeto.
- **OpenSSL**: Utilizada para geração e expansão de chaves AES.
- **OpenMP**: Empregada para paralelizar o processo de criptografia, aumentando a eficiência do algoritmo.
- **CMake**: Ferramenta de compilação para gerenciar o processo de construção do projeto.

## 🚀 **Como Executar o Projeto**

## **Clone o Repositório**:
- git clone https://github.com/FranciscoNeto05/TrabalhoPPC-ParalelizacaoAES.git
- cd TrabalhoPPC-ParalelizacaoAES
- mkdir build
- cd build
- cmake ..
- make
- make run
- **Não se esqueça de editar o CMakeLists.txt para o seu ambiente de trabalho**

## 📂 **Estrutura do Repositório**
- index.cpp: Arquivo principal com a lógica de execução.
- expansao.hpp: Cabeçalho que contém as funções para geração e expansão de chaves.
- encrypt.hpp & decrypt.hpp: Implementações para criptografia e descriptografia de dados.
- sbox.hpp: Arquivo contendo as tabelas S-Box e Inverse S-Box para o AES.

## 📖 **Referências**
Este projeto faz uso das bibliotecas **OpenSSL** e **OpenMP**. Para mais informações, consulte a documentação do OpenSSL e documentação do OpenMP.