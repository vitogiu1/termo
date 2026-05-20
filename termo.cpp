/*ativar ansi em WINDOWS para ir em texto colorido*/
#include <iostream>
#include <cstdlib> // Contém as funções rand() e srand()
#include <ctime>
#include <string>
#include <fstream>
#include <cctype>
#include <vector>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

//tira acentos e cedilha, "normaliza" a palavra e a joga para MAIÚSCULO
string normalizar_palavra(string palavra) {
    string resultado = "";
    for (size_t i = 0; i < palavra.length(); i++) {
        unsigned char c = palavra[i];
        
        if (c < 128) {
            if(c != '\r' && c != '\n') { // Ignora caracteres de nova linha e carriage return
                resultado += toupper(c);
            } 
        } 
        else if (c == 195 || c == 196 || c == 197) {
            if (i + 1 >= palavra.length())
                break;
            i++; 
            unsigned char proximo = palavra[i];
            
            // Variantes do A
            if ((proximo >= 128 && proximo <= 133) || (proximo >= 160 && proximo <= 165)) {
                resultado += 'A';
            }
            // Variantes do E
            else if ((proximo >= 136 && proximo <= 139) || (proximo >= 168 && proximo <= 171)) {
                resultado += 'E';
            }
            // Variantes do I
            else if ((proximo >= 140 && proximo <= 143) || (proximo >= 172 && proximo <= 175)) {
                resultado += 'I';
            }
            // Variantes do O
            else if ((proximo >= 146 && proximo <= 150) || (proximo >= 178 && proximo <= 182)) {
                resultado += 'O';
            }
            // Variantes do U
            else if ((proximo >= 154 && proximo <= 157) || (proximo >= 186 && proximo <= 189)) {
                resultado += 'U';
            }
            // Cedilha
            else if (proximo == 135 || proximo == 167) {
                resultado += 'C';
            }
            else {
                resultado += toupper(c); 
            }
        }
    }
    return resultado;
}

// se for windows, para dar cor no terminal, tem que ativar o modo ANSI
void ativarANSI() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    SetConsoleMode(hOut, dwMode);

    // Força o interpretador a usar UTF-8 no CIn e COut para evitar problemas com acentos 
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
}

int main() {
    ativarANSI();
    ifstream arquivo("palavras.txt");
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo palavras.txt! Verifique se ele esta na mesma pasta do executavel." << endl;
        return 1;
    }
    
    
    vector<string> lista_palavras;
    string linha;
    while (getline(arquivo, linha)) {
        // Guarda na lista já normalizada (sem acentos e em maiúsculo)
        lista_palavras.push_back(normalizar_palavra(linha));
    }
    
    arquivo.close();

    if (lista_palavras.empty()) {
        cout << "Erro! O arquivo palavras.txt esta vazio.\n";
        return 1;
    }
    
    srand(time(NULL));
    size_t total_de_palavras = lista_palavras.size(); 
    int indice_sorteado = rand() % total_de_palavras; 
    
    string certa = lista_palavras[indice_sorteado]; //palavra certa sorteada

    string palpite; // CHUTE
    
    // --- MANUAL DO JOGO ---
    cout << "=========================================================\n";
    cout << "                    BEM-VINDO AO TERMO!                  \n";
    cout << "=========================================================\n";
    cout << "Descubra a palavra certa de 5 letras em ate 6 tentativas.\n\n";
    cout << "COMO JOGAR:\n";
    cout << "* Uma letra \033[32mVERDE\033[0m significa que ela esta \033[32mCORRETA\033[0m e no \033[32mLUGAR CERTO\033[0m.\n";
    cout << "* Uma letra \033[33mAMARELA\033[0m significa que ela \033[32mEXISTE\033[0m, mas no \033[33mLUGAR ERRADO\033[0m.\n";
    cout << "* Uma letra \033[31mVERMELHA\033[0m significa que ela \033[31mNAO EXISTE\033[0m na palavra.\n\n";
    cout << "EXEMPLO:\n";
    cout << "Se o palpite for J O G O S e o resultado for:\n";
    cout << "\033[31mJ\033[0m \033[33mO\033[0m \033[32mG\033[0m \033[31mO\033[0m \033[31mS\033[0m\n";
    cout << "-> A letra 'G' esta no lugar certo.\n";
    cout << "-> A letra 'O' existe na palavra, mas nao nessa posicao.\n";
    cout << "-> As letras 'J', 'S' e o segundo 'O' nao fazem parte da palavra.\n";
    cout << "=========================================================\n\n";

    int k = 1; // sentinela controladora de tentativas
    
    while (k <= 6) {
        int status[5] = {0,0,0,0,0};
        // 0 = nao existe
        // 1 = existe lugar errado
        // 2 = lugar certo

        int usados[5] = {0,0,0,0,0};
        // controla letras da palavra secreta ja consumidas
        cout << "\nTentativa " << k << " de 6 - Insira seu chute: ";
        cin >> palpite;

        // Normaliza o palpite antes da validação de tamanho
        palpite = normalizar_palavra(palpite);

        // validacao de tamanho
        if (palpite.length() != 5) {
            cout << "Erro! Seu palpite precisa ter exatamente 5 letras.\n";
            continue;
        }

        // Validador de palavra existente no dicionário
        int palavra_valida = 0;
        for (size_t i = 0; i < lista_palavras.size(); i++) {
            if (palpite == lista_palavras[i]) {
                palavra_valida = 1;
                break;
            }
        }

        if (palavra_valida == 0) {
            cout << "Palavra invalida! Essa palavra nao existe no dicionario do jogo.\n";
            continue; 
        }

        // primeiro teste para ver se a letra esta na posicao correta da palavra referencia e do palpite
        for (int i = 0; i < 5; i++) {
            if (palpite[i] == certa[i]) {
                status[i] = 2;
                usados[i] = 1; 
            }
        }

        // o proximo laco eh pra verificar as letras certas na posicao errada
        for (int i = 0; i < 5; i++) {
            if (status[i] == 2) {
                continue; 
            }
            for (int j = 0; j < 5; j++) {
                if (palpite[i] == certa[j] && usados[j] == 0) {
                    status[i] = 1;
                    usados[j] = 1;
                    break;
                }
                
            }
        }

        int numero_acertos = 0;
        for (int i = 0; i < 5; i++) {
            if (status[i] == 2) {
                cout << "\033[32m" <<(char)toupper(palpite[i]) << "\033[0m" << " ";
                numero_acertos++;
            }
            else if (status[i] == 1) {
                cout << "\033[33m" << (char)toupper(palpite[i]) << "\033[0m" << " ";
            }
            else {
                cout << "\033[31m" << (char)toupper(palpite[i]) << "\033[0m" << " ";
            }
        }
        cout << endl;

        if (numero_acertos == 5) {
            cout << endl << "=========================================================" << endl;
            cout << "      PARABENS! Voce acertou a palavra em " << k << " tentativas! " << endl;
            cout << "=========================================================" << endl;
            break;
        }
        k++; 
    }

    if (k == 7) {
        cout << endl << "Fim de jogo! Suas tentativas acabaram. A palavra era: " << certa << endl;
    }

    
    cout << "\nPressione Enter para sair...";
    cin.ignore(); // limpa o buffer de entrada
    cin.get(); // pressionar enter fecha o programa
    
    
    return 0;    
}