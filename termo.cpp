/*
DIFERENCAS:
Adicionado o arquivo com todas as palavras possiveis para o termo no arquivo palavras.txt, adicionado o random para selecionar qualquer palavra aleatoria do palavras.txt,
adicionado controle e numero de tentativas permitidas para cada palavra,
de resto a logica se manteve, so uma breve formatacao das tentativas e quando acertar faz algo mais bonitinho.
Normalização de acentos e cedilha integrada na leitura e no sorteio para compatibilidade total com o novo dicionário.
*/
#include <iostream>
#include <cstdlib> // Contém as funções rand() e srand()
#include <ctime>
#include <string>
#include <fstream>
#include <cctype>
#include <vector>
#include <chrono>
#include <thread>
using namespace std;

//tira acentos e cedilha, "normaliza" a palavra e a joga para MAIÚSCULO
string normalizar_palavra(string palavra) {
    string resultado = "";
    for (size_t i = 0; i < palavra.length(); i++) {
        unsigned char c = palavra[i];
        
        if (c < 128) {
            resultado += toupper(c);
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

int main() {
    ifstream arquivo("palavras.txt");
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo palavras.txt! Verifique se ele esta na mesma pasta do codigo." << endl;
        return 1;
    }
    
    vector<string> lista_palavras;
    string linha;
    while (getline(arquivo, linha)) {
        // Guarda na lista já normalizada (sem acentos e em maiúsculo)
        lista_palavras.push_back(normalizar_palavra(linha));
    }
    arquivo.close();
    
    srand(time(NULL));
    int total_de_palavras = lista_palavras.size(); 
    int indice_sorteado = rand() % total_de_palavras; 
    
    // CORREÇÃO AQUI: Garante que mesmo se a lógica anterior mudar, a palavra sorteada passa pelo filtro
    string certa = lista_palavras[indice_sorteado]; 

    string palpite; // CHUTE
    
    // --- MANUAL DO JOGO ---
    cout << "=========================================================\n";
    cout << "                    BEM-VINDO AO TERMO!                  \n";
    cout << "=========================================================\n";
    cout << "Descubra a palavra certa de 5 letras em ate 6 tentativas.\n\n";
    cout << "COMO JOGAR:\n";
    cout << "* Uma letra MAIUSCULA significa que ela esta CORRETA e no LUGAR CERTO.\n";
    cout << "* Uma letra entre PARENTESES (x) significa que ela EXISTE, mas no LUGAR ERRADO.\n";
    cout << "* Uma letra minuscula comum significa que ela NAO EXISTE na palavra.\n\n";
    cout << "EXEMPLO:\n";
    cout << "Se o palpite for J O G O S e o resultado for:\n";
    cout << "j (o) G o s\n";
    cout << "-> A letra 'G' esta no lugar certo.\n";
    cout << "-> A letra 'O' existe na palavra, mas nao nessa posicao.\n";
    cout << "-> As letras 'J', 'S' e o segundo 'O' nao fazem parte da palavra.\n";
    cout << "O jogo aceita caracteres especiais!\n";
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
                cout << (char)toupper(palpite[i]) << " ";
                numero_acertos++;
            }
            else if (status[i] == 1) {
                cout << "(" << (char)tolower(palpite[i]) << ")" << " ";
            }
            else {
                cout << (char)tolower(palpite[i]) << " ";
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

    // Contagem regressiva para fechar o terminal
    
    for (int i = 10; i > 0; i--) {
        this_thread::sleep_for(chrono::milliseconds(1000));
        cout << "\rSaindo em " << i << " segundos... ";
    }
    
    
    return 0;    
}