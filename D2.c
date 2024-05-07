#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar um processo
typedef struct {
    int chegada;
    int duracao;
} Processo;

// Estrutura para representar a lista de processos
typedef struct {
    Processo *processos;
    int tamanho;
} ListaProcessos;

// Estrutura para representar os resultados
typedef struct {
    float retorno_medio;
    float resposta_medio;
    float espera_medio;
} Resultado;

// Função para calcular as médias
Resultado calcularMedias(int retorno_total, int resposta_total, int espera_total, int num_processos) {
    Resultado resultado;
    resultado.retorno_medio = (float)retorno_total / num_processos;
    resultado.resposta_medio = (float)resposta_total / num_processos;
    resultado.espera_medio = (float)espera_total / num_processos;
    return resultado;
}

// Função para executar o algoritmo First Come First Served (FCFS)
Resultado fcfs(ListaProcessos processos) {
    int tempo_retorno_total = 0, tempo_resposta_total = 0, tempo_espera_total = 0;
    int tempo_inicio = processos.processos[0].chegada;
    int soma_duracao = 0;

    for (int i = 0; i < processos.tamanho; i++) {
        if (i != 0) {
            tempo_resposta_total += soma_duracao - (processos.processos[i].chegada - tempo_inicio);
            tempo_espera_total += soma_duracao - (processos.processos[i].chegada - tempo_inicio);
        }
        soma_duracao += processos.processos[i].duracao;
        tempo_retorno_total += soma_duracao - (processos.processos[i].chegada - tempo_inicio);
    }

    return calcularMedias(tempo_retorno_total, tempo_resposta_total, tempo_espera_total, processos.tamanho);
}

// Função para executar o algoritmo Shortest Job First (SJF)
Resultado sjf(ListaProcessos processos) {
    ListaProcessos procs;
    procs.processos = (Processo*)malloc(processos.tamanho * sizeof(Processo));
    for (int i = 0; i < processos.tamanho; i++) {
        procs.processos[i] = processos.processos[i];
    }
    int tempo_retorno_total = 0, tempo_resposta_total = 0, tempo_espera_total = 0;
    int tempo_inicio = processos.processos[0].chegada;
    int tempo_atual = tempo_inicio, soma_duracao = 0;

    while (procs.tamanho > 0) {
        int index_proximo = -1;
        int menor_duracao = -1;
        for (int i = 0; i < procs.tamanho; i++) {
            if (procs.processos[i].chegada <= tempo_atual) {
                if (menor_duracao == -1 || procs.processos[i].duracao < menor_duracao ||
                    (procs.processos[i].duracao == menor_duracao && procs.processos[i].chegada < procs.processos[index_proximo].chegada)) {
                    menor_duracao = procs.processos[i].duracao;
                    index_proximo = i;
                }
            }
        }
        if (index_proximo == -1) {
            tempo_atual = procs.processos[0].chegada;
        } else {
            tempo_resposta_total += soma_duracao - (procs.processos[index_proximo].chegada - tempo_inicio);
            tempo_espera_total += soma_duracao - (procs.processos[index_proximo].chegada - tempo_inicio);
            soma_duracao += procs.processos[index_proximo].duracao;
            tempo_retorno_total += soma_duracao - (procs.processos[index_proximo].chegada - tempo_inicio);
            tempo_atual += procs.processos[index_proximo].duracao;
            for (int i = 0; i < procs.tamanho; i++) {
                if (i != index_proximo && procs.processos[i].chegada < tempo_atual) {
                    tempo_espera_total += tempo_atual - procs.processos[i].chegada;
                }
            }
            for (int i = index_proximo; i < procs.tamanho - 1; i++) {
                procs.processos[i] = procs.processos[i + 1];
            }
            procs.tamanho--;
        }
    }

    free(procs.processos);
    return calcularMedias(tempo_retorno_total, tempo_resposta_total, tempo_espera_total, processos.tamanho);
}

// Função para executar o algoritmo Round Robin (RR)
Resultado rr(ListaProcessos processos, int quantum) {
    ListaProcessos procs;
    procs.processos = (Processo*)malloc(processos.tamanho * sizeof(Processo));
    for (int i = 0; i < processos.tamanho; i++) {
        procs.processos[i] = processos.processos[i];
    }
    int tempo_retorno_total = 0, tempo_resposta_total = 0, tempo_espera_total = 0;
    int tempo_inicio = processos.processos[0].chegada;
    int tempo_atual = tempo_inicio, soma_duracao = 0;

    while (procs.tamanho > 0) {
        int i = 0;
        while (i < procs.tamanho) {
            if (procs.processos[0].chegada < tempo_atual || procs.processos[0].chegada == tempo_inicio) {
                break;
            } else {
                Processo p = procs.processos[0];
                for (int j = 0; j < procs.tamanho - 1; j++) {
                    procs.processos[j] = procs.processos[j + 1];
                }
                procs.processos[procs.tamanho - 1] = p;
            }
        }

        if (procs.processos[0].chegada != -1) {
            procs.processos[0].chegada = -1;
            tempo_resposta_total += tempo_atual - procs.processos[0].chegada;
        }

        if (procs.processos[0].duracao <= quantum) {
            tempo_atual += procs.processos[0].duracao;
            tempo_retorno_total += tempo_atual - procs.processos[0].chegada;
            for (int j = 1; j < procs.tamanho; j++) {
                if (procs.processos[j].chegada < tempo_atual) {
                    tempo_espera_total += (quantum < tempo_atual - procs.processos[j].chegada ? quantum : tempo_atual - procs.processos[j].chegada);
                }
            }
            for (int j = 0; j < procs.tamanho - 1; j++) {
                procs.processos[j] = procs.processos[j + 1];
            }
            procs.tamanho--;
        } else {
            procs.processos[0].duracao -= quantum;
            tempo_atual += quantum;
            for (int j = 1; j < procs.tamanho; j++) {
                if (procs.processos[j].chegada < tempo_atual) {
                    tempo_espera_total += (quantum < tempo_atual - procs.processos[j].chegada ? quantum : tempo_atual - procs.processos[j].chegada);
                }
            }
            Processo p = procs.processos[0];
            for (int j = 0; j < procs.tamanho - 1; j++) {
                procs.processos[j] = procs.processos[j + 1];
            }
            procs.processos[procs.tamanho - 1] = p;
        }
    }

    free(procs.processos);
    return calcularMedias(tempo_retorno_total, tempo_resposta_total, tempo_espera_total, processos.tamanho);
}

int main() {
    int num_processos;
    printf("Insira o número de processos: ");
    scanf("%d", &num_processos);

    ListaProcessos processos;
    processos.processos = (Processo*)malloc(num_processos * sizeof(Processo));
    processos.tamanho = num_processos;

    printf("Insira os processos no formato 'chegada1 duracao1 chegada2 duracao2 ...':\n");
    for (int i = 0; i < num_processos; i++) {
        scanf("%d %d", &processos.processos[i].chegada, &processos.processos[i].duracao);
    }

    Resultado resultado_fcfs = fcfs(processos);
    Resultado resultado_sjf = sjf(processos);
    Resultado resultado_rr = rr(processos, 2);

    printf("FCFS %.1f %.1f %.1f\n", resultado_fcfs.retorno_medio, resultado_fcfs.resposta_medio, resultado_fcfs.espera_medio);
    printf("SJF %.1f %.1f %.1f\n", resultado_sjf.retorno_medio, resultado_sjf.resposta_medio, resultado_sjf.espera_medio);
    printf("RR %.1f %.1f %.1f\n", resultado_rr.retorno_medio, resultado_rr.resposta_medio, resultado_rr.espera_medio);

    free(processos.processos);
    return 0;
}
