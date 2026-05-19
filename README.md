# Termo em C++ (Console Game)

Uma versão para terminal do famoso jogo **Termo** (inspirado no *Wordle*), desenvolvida inteiramente em C++. O projeto conta com sorteio de palavras dinâmico através de um arquivo de dicionário externo e tratamento avançado de caracteres da língua portuguesa.

---

## 🕹️ Como Jogar

O objetivo é adivinhar uma palavra secreta de 5 letras em no máximo 6 tentativas. A cada palpite, o jogo analisa as letras e devolve um feedback visual:

* **Letras MAIÚSCULAS:** Indicam que a letra está correta e na posição exata (Ex: `T`).
* **Letras entre PARÊNTESES (x):** Indicam que a letra existe na palavra secreta, mas está na posição errada.
* **Letras minúsculas comuns:** Indicam que a letra não faz parte da palavra secreta.

> 💡 **Exemplo:** Se a palavra certa for `JOGOS` e o seu palpite for `TERMO`, as letras que não pertencem à palavra ficarão minúsculas, ajudando a guiar o seu próximo chute.

---

## ⚙️ Características e Funcionalidades

1. **Dicionário Dinâmico:** O jogo lê um arquivo externo chamado `palavras.txt`, permitindo expandir ou alterar o banco de palavras do jogo facilmente sem mexer no código-fonte.
2. **Sorteio Aleatório:** Utiliza o relógio do sistema como semente (`srand(time(NULL))`) para garantir que a palavra secreta seja diferente a cada nova partida.
3. **Validação de Palpites:** O jogo valida se a palavra digitada possui exatamente 5 letras e se ela realmente existe no dicionário. Caso seja inválida, o jogador é alertado e não perde a tentativa.
4. **Normalizador UTF-8 nativo:** O sistema aceita palavras com acentos ou cedilha (ex: "maçã") e as normaliza automaticamente para o padrão do jogo (ex: "MACA"), evitando bugs de tamanho de string comuns no terminal do Windows.
5. **Efeito de Saída:** Utiliza manipulação de fluxo com `\r` (carriage return) e as biblioteca `<chronos>` e `<thread>`para criar uma contagem regressiva visual antes de fechar o prompt de comando.

---

## 🧠 Processo de Desenvolvimento

O projeto foi construído seguindo etapas claras de evolução de software:

* **Fase 1 (Algoritmo Principal):** Desenvolvimento da lógica central do jogo de forma independente. O maior desafio técnico foi criar o sistema de checagem em duas etapas (etapa de prioridade absoluta para letras na posição correta e etapa para letras na posição errada), o que blindou o jogo contra falsos positivos no caso de letras repetidas no palpite.
* **Fase 2 (Estrutura de Dados):** Transição de uma palavra estática para o carregamento dinâmico de arquivos. Foi implementada a leitura de fluxo de arquivos (`ifstream`) e o armazenamento dinâmico das strings em memória utilizando a estrutura de vetores (`std::vector`).
* **Fase 3 (Refinamento e Engenharia de Prompt):** Para transformar a lógica robusta em um produto final polido, utilizei ferramentas de Inteligência Artificial como assistentes de codificação. Através de engenharia de prompt, integrei ao meu motor principal funções de tratamento de cache e tabelas de mapeamento de bytes UTF-8 para normalizar acentos e cedilhas diretamente no terminal.

Este processo permitiu focar os esforços na arquitetura lógica do jogo e utilizar tecnologias modernas para acelerar a entrega de recursos complexos de infraestrutura e interface de usuário.

---

## 🛠️ Tecnologias Utilizadas

* **Linguagem:** C++
* **Paradigma:** Programação Estruturada / Funcional
* **Compilador Utilizado:** GCC/G++ através do VS Code
* **Bibliotecas Padrão:** `<iostream>`, `<vector>`, `<string>`, `<fstream>`, `<cstdlib>`, `<ctime>`, `<chronos>`, `<thread>`.

---

## 🚀 Como Rodar o Projeto

### Opção 1: Executar Direto (Para Jogadores)
1. Certifique-se de baixar o arquivo `termo.exe` e o arquivo `palavras.txt`.
2. Coloque os dois arquivos **obrigatoriamente na mesma pasta**.
3. Dê dois cliques em `termo.exe` para abrir o terminal do Windows e começar a jogar.

### Opção 2: Compilar o Código (Para Desenvolvedores)
1. Certifique-se de ter um compilador C++ instalado (como o MinGW para Windows).
2. Baixe os arquivos `termo.cpp` e `palavras.txt` e coloque-os na mesma pasta.
3. Abra o terminal na pasta e compile o código com o comando:
   ```bash
   g++ termo.cpp -o termo.exe
4. Execute o codigo compilado:
  ```bash
  ./termo.exe
