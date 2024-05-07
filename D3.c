#include <stdio.h>
#include <stdbool.h>

#define N_PROCESSOS 3
#define N_RECURSOS 4

typedef struct {
    int recursos_alocados[N_RECURSOS];
    int recursos_necessarios[N_RECURSOS];
} Processo;

typedef struct {
    Processo processos[N_PROCESSOS];
    int recursos_disponiveis[N_RECURSOS];
} bank;

// Função para copiar recursos de um array para outro
void copia_recursos(int destino[], const int origem[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        destino[i] = origem[i];
    }
}

// Função que executa o algoritmo do banqueiro para verificar se o sistema está em estado seguro
bool executa(bank *bank) {
    // Inicializações
    int n = N_PROCESSOS; // Número de processos
    int m = N_RECURSOS;  // Número de tipos de recursos
    int recursos_disponiveis[m];    // Recursos disponíveis no sistema
    int recursos_alocados[n][m];    // Recursos alocados para cada processo
    int recursos_necessarios[n][m];  // Recursos necessários para cada processo
    bool processos_finalizados[n];   // Marca quais processos já terminaram
    int processos_finalizados_count = 0; // Contador de processos finalizados

    // Copia os recursos disponíveis do bank para a variável local
    copia_recursos(recursos_disponiveis, bank->recursos_disponiveis, m);

    // Copia os recursos alocados e necessários de cada processo para as variáveis locais
    for (int i = 0; i < n; i++) {
        copia_recursos(recursos_alocados[i], bank->processos[i].recursos_alocados, m);
        copia_recursos(recursos_necessarios[i], bank->processos[i].recursos_necessarios, m);
        processos_finalizados[i] = false; // Inicializa todos os processos como não finalizados
    }

    // Loop principal do algoritmo
    while (processos_finalizados_count < n) {
        bool processo_encontrado = false; // Flag para indicar se um processo foi encontrado
        for (int i = 0; i < n; i++) {
            if (!processos_finalizados[i]) { // Verifica se o processo i ainda não foi finalizado
                int j = 0;
                for (; j < m; j++) {
                    // Verifica se os recursos necessários do processo i podem ser atendidos pelos recursos disponíveis
                    if (recursos_necessarios[i][j] > recursos_disponiveis[j]) {
                        break; // Se não, interrompe o loop
                    }
                }
                if (j == m) {
                    // Se todos os recursos necessários do processo i podem ser atendidos, o processo pode ser finalizado
                    processos_finalizados_count++; // Incrementa o contador de processos finalizados
                    processos_finalizados[i] = true; // Marca o processo como finalizado
                    processo_encontrado = true; // Indica que um processo foi encontrado e finalizado
                    // Libera os recursos alocados pelo processo, adicionando-os aos recursos disponíveis
                    for (int k = 0; k < m; k++) {
                        recursos_disponiveis[k] += recursos_alocados[i][k];
                    }
                }
            }
        }
        // Se nenhum processo foi encontrado para execução, sai do loop
        if (!processo_encontrado) {
            break;
        }
    }
    // Retorna true se todos os processos foram finalizados (estado seguro), senão retorna false (deadlock)
    return processos_finalizados_count == n;
}

int main() {
    // Definição dos processos e recursos para o teste 1
    Processo processos[N_PROCESSOS] = {
        {{0, 0, 1, 0}, {2, 0, 0, 1}},
        {{2, 0, 0, 1}, {1, 0, 1, 0}},
        {{0, 1, 2, 0}, {2, 1, 0, 0}}
    };

    int recursos_disponiveis[N_RECURSOS] = {2, 1, 0, 0};

    // Inicializa o bank com os processos e recursos
    bank b1 = {{0}};
    for (int i = 0; i < N_PROCESSOS; i++) {
        b1.processos[i] = processos[i];
    }
    copia_recursos(b1.recursos_disponiveis, recursos_disponiveis, N_RECURSOS);

    // Executa o algoritmo e verifica se ocorreu deadlock
    if (executa(&b1)) {
        printf("Não ocorreu deadlock, logo o sistema está em estado seguro\n");
    } else {
        printf("Ocorreu deadlock\n");
    };

    // Definição dos processos e recursos para o teste 2
    Processo processos2[N_PROCESSOS] = {
        {{0, 0, 1, 0}, {2, 0, 0, 1}},
        {{2, 0, 0, 1}, {1, 0, 1, 0}},
        {{0, 1, 2, 0}, {2, 1, 0, 1}}
    };

    int recursos_disponiveis2[N_RECURSOS] = {2, 1, 0, 0};

    // Inicializa o bank com os processos e recursos
    bank b2 = {{0}};
    for (int i = 0; i < N_PROCESSOS; i++) {
        b2.processos[i] = processos2[i];
    }
    copia_recursos(b2.recursos_disponiveis, recursos_disponiveis2, N_RECURSOS);

    // Executa o algoritmo e verifica se ocorreu deadlock
    if (executa(&b2)) {
        printf("Não ocorreu deadlock\n");
    } else {
        printf("Ocorreu deadlock\n");
    }

    return 0;
}
