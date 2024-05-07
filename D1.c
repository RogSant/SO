#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/*função que calcula o tamanho do intervalo a ser somado*/
int calculaIntervalo(int lista[2]){
    if(lista[0] == lista[1]) return -1;
    if(lista[0] > lista[1]) return lista[0] - lista[1] + 1;
    else return lista[1] - lista[0] + 1;
}

int main(){

    /*aqui obtemos o input de quantos processos desejamos criar*/
    int qntFilhos;
    printf("Digite quantos processos filhos devem ser criados: ");
    scanf("%d", &qntFilhos);

    int pids[qntFilhos];

    /*Aqui obtemos os extremos do intervalo que desejamos somar.
    Ele deve ser colocado com o menor numero primeiro e o maior depois
    ex: 0 10*/
    int intervalo[qntFilhos];
    printf("Digite o intevalo que deseja ser somado(dois numeros separados por espaç):");
    scanf("%d",&intervalo[0]);
    scanf("%d", &intervalo[1]);

   /*Chamada para calcular o tamanho do intervalo*/
    int qntNumIntervalo = calculaIntervalo(intervalo);

    /*Aqui estamos fazendo a divisão dos subconjuntos de soma para cada filho
    de forma a deixá-los o mais uniformes possível*/
    int numPorFilho[qntFilhos];
    int div = qntNumIntervalo / qntFilhos;
    for(int j = 0; j < qntFilhos; j++){
        numPorFilho[j] = div;
    }
    int resto = qntNumIntervalo % qntFilhos;
    for(int i = 0; i < resto; i++){
        numPorFilho[i]++;
    }

    /*Aqui estamos criando a lista com os números a serem somados
    com base no intervalo, ex: se o intervalo é de 0 a 3, criaremos
    uma lista com os valores 0,1,2,3*/
    int lista[qntNumIntervalo], start = intervalo[0];
    for(int k = 0; k < qntNumIntervalo; k++){
        lista[k] = start + k;
    }

    /*Aqui estamos criando as pipes para comunicação entre os processos
    filhos e o processo pai*/
    int pipes[qntFilhos][2];
    for(int i = 0; i < qntFilhos; i++){
        if(pipe(pipes[i]) == -1){
            printf("Error criando os pipes");
            return -1;
        }
    }

    /*Aqui é o looping principal onde criamos os processos filhos,
    ele funciona ao verificar se o processo que o esta executando é
    um filho ou um pai, se for filho, irá executar a soma dos valores
    presentes no seu subconjunto, colocar o valor no pipe e ser encerrado.
    Se for pai ele sairá do looping, de forma a não criar mais processos*/
    for(int i = 0; i < qntFilhos; i++){
        int pid = fork();
        if(pid == -1){
            printf("Erro ao criar processo filho");
            return -1;
        }
        if(pid == 0){
            /*Fechando os pipes não ultilizados*/
            for(int j = 0; j < qntFilhos; j++){
                close(pipes[j][0]);
                if(i != j) close(pipes[j][1]);
            }
        /*Calculando a posição aonde o processo filho
        irá começar a fazer a soma na lista de valores*/
        int start = 0;
        for(int f = 0; f < i; f++){
            start = start + numPorFilho[f];
        };
        /*Realizando a soma dos valores do subconjunto*/
        int somaParcial = 0;
        for(int r = 0; r < numPorFilho[i]; r++){
            somaParcial = somaParcial + lista[start + r];
        }
        /*Escrevendo no pipe para comunicar ao processo pai
        o valor da sua soma parcial*/
        if(write(pipes[i][1], &somaParcial, sizeof(int)) == -1){
            printf("erro escrevendo no pipe filho %d\n", i);
            return 4;
        }

        close(pipes[i][1]);
        exit(0);
        }
    }

    /*Aqui o processo pai está fechando os pipes de escrita,
    pois ele não os utilizará*/
    for(int i = 0; i < qntFilhos; i++){
        close(pipes[i][1]);
    }

    /*Aqui o processo pai irá ler os valores obtidos das somas realizadas
    por cada filho a partir dos pipes e irá realizar a soma final*/
    int somaFinal = 0;
    for(int i = 0; i < qntFilhos; i++){
        int somaParcial;
        if(read(pipes[i][0], &somaParcial, sizeof(int)) == -1){
            printf("Erro lendo do pipe %d\n", i);
            return 2;
        }
        somaFinal += somaParcial;
    }
    printf("Somatório final = %d\n", somaFinal);//printando o resultado final

    /*Laço para o processo pai esperar todos os filhos se encerrarem*/
    for(int i = 0; i < qntFilhos; i++){
        wait(NULL);
    }

    return 0;
}
