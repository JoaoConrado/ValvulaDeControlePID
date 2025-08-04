# Controle PID para Válvula de Processo em C++

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![GitHub language count](https://img.shields.io/github/languages/count/JoaoConrado/ValvulaDeControlePID)
![GitHub top language](https://img.shields.io/github/languages/top/JoaoConrado/ValvulaDeControlePID)

## 📖 Sobre o Projeto

Este projeto apresenta a implementação e simulação de um controlador Proporcional-Integral-Derivativo (PID) em C++ para o controle de uma válvula de processo industrial. O objetivo é demonstrar os fundamentos da teoria de controle em malha fechada, onde o sistema busca ativamente corrigir o erro entre uma variável de processo (PV), como vazão ou nível, e um ponto de ajuste desejado (Setpoint - SP).

A simulação, escrita em C++ puro, modela a dinâmica de um sistema de primeira ordem com atraso (FOPDT). O programa gera os dados da simulação em um arquivo `.csv`, permitindo a análise e visualização posterior dos resultados para a sintonia dos ganhos Kp, Ki e Kd.

---

## 🚀 Funcionalidades

* [x] **Algoritmo PID:** Implementação do controlador PID digital em C++ orientado a objetos.
* [x] **Simulação de Processo:** Modelo matemático do processo industrial para testes do controlador.
* [x] **Sintonia de Ganhos:** Permite a fácil alteração dos parâmetros Kp, Ki e Kd via constantes no código ou argumentos de linha de comando.
* [x] **Saída de Dados:** Exporta os resultados da simulação (tempo, PV, SP, MV) para um arquivo `.csv` para fácil visualização.
* [x] **Análise de Distúrbios:** Capacidade de simular o efeito de distúrbios externos no processo.

---

## 🛠️ Tecnologias Utilizadas

Este projeto foi desenvolvido utilizando as seguintes tecnologias:

* **Linguagem:** C++ (Padrão C++11 ou superior)
* **Bibliotecas Padrão:**
    * `<iostream>` para I/O de console.
    * `<vector>` para armazenamento de dados da simulação.
    * `<fstream>` para escrita dos resultados em arquivo.
    * `<cmath>` para operações matemáticas.
* **Ferramentas de Build:** `g++` (GNU Compiler) ou `CMake` (recomendado).
* **Visualização:** Os dados de saída (`.csv`) podem ser plotados com qualquer ferramenta de sua preferência (Excel, Google Sheets, Gnuplot, Python/Matplotlib).
* **Conceitos:** Teoria de Controle, Controle em Malha Fechada, Modelagem de Sistemas, Programação Orientada a Objetos.

---

## ⚙️ Instalação e Compilação

Para compilar e executar esta simulação em seu ambiente local, siga os passos abaixo.

1.  **Clone o repositório:**
    ```bash
    git clone [https://github.com/JoaoConrado/ValvulaDeControlePID.git](https://github.com/JoaoConrado/ValvulaDeControlePID.git)
    ```

2.  **Acesse o diretório do projeto:**
    ```bash
    cd ValvulaDeControlePID
    ```

3.  **Compile o código:**
    Você pode usar o `g++` diretamente ou o `CMake` (se o projeto estiver configurado para isso).

    * **Opção A: Compilando com g++**
        ```bash
        # Compila os arquivos-fonte e gera o executável 'pid_simulation'
        g++ -std=c++11 -o pid_simulation src/*.cpp 
        ```

    * **Opção B: Compilando com CMake (Recomendado)**
        ```bash
        # Cria um diretório de build
        mkdir build && cd build

        # Gera os arquivos de build
        cmake ..

        # Compila o projeto
        make
        ```

---

## 🕹️ Como Usar

Após a compilação, o executável estará pronto para ser usado.

```bash
# No Linux/macOS
./pid_simulation

# No Windows
.\pid_simulation.exe
