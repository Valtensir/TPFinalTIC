#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <conio.h>

typedef struct
{
    int qtdCidades;
    float **matrizDistancia;
    int *vetSolucao;
    float distTotal;
    int *vetSolucaoStar;
    float *vetPremio;
    float *vetPenalidade;

} Dados;

int i,j,k = 0;
FILE *outFile;

void lerDados(Dados dados, FILE* arq, int aux){
    for (i = 0; i < dados.qtdCidades; i++)
    {

        fscanf(arq, "%f", &dados.vetPremio[i]);
    }
    for (i = 0; i < dados.qtdCidades; i++)
    {
        fscanf(arq, "%f", &dados.vetPenalidade[i]);
    }
    for (i = 0; i < dados.qtdCidades; i++)
    {
        dados.matrizDistancia[i] = (float *)malloc(dados.qtdCidades * sizeof(float));
        for (j = 0; j < dados.qtdCidades; ++j)
        {
            fscanf(arq, "%f", &dados.matrizDistancia[i][j]);
        }
    }
}

int vizinhoGRASP(int inicio, int l, Dados dados, int* selecionada, float FOStar, float alfa, float premioMinimo){
    int distancia, aux, distanciaMaior = -1, aux2, index, escolhido;
    int* candidatos;
    float premioAlcancado = 0;
    inicio = l;
    index = 1;
    dados.vetSolucao[0] = inicio;
    selecionada[inicio] = 1;

    srand(time(NULL));

    candidatos = (int *)malloc(dados.qtdCidades * sizeof(int));

    dados.distTotal = 0;

    // faz a busca pela solução atráves do vizinho mais próximo
    k = 0;
    while(premioAlcancado < premioMinimo)
    {

        distancia = 1000000;
        distanciaMaior = -1;
        for (j = 0; j < dados.qtdCidades; j++)
        {

            if (dados.matrizDistancia[dados.vetSolucao[k]][j] < distancia && dados.vetSolucao[k] != j && selecionada[j] == 0)
            {
                distancia = dados.matrizDistancia[dados.vetSolucao[k]][j];
                aux = j;
            }
            if (dados.matrizDistancia[dados.vetSolucao[k]][j] > distanciaMaior && dados.vetSolucao[k] != j && selecionada[j] == 0)
            {
                distanciaMaior = dados.matrizDistancia[dados.vetSolucao[k]][j];
                aux2 = j;
            }
        }

        candidatos[0] = aux;

        distanciaMaior /= 4;

        if (distanciaMaior < distancia)
        {
            distanciaMaior *= 4;
        }

        for (j = 0; j < dados.qtdCidades; ++j)
        {
            if ((distancia + alfa*(distanciaMaior - distancia)) > dados.matrizDistancia[dados.vetSolucao[k]][j] && selecionada[j] == 0 && j != aux)
            {
                candidatos[index] = j;
                index++;
            }
        }

        escolhido = rand() % index;

        dados.distTotal += dados.matrizDistancia[dados.vetSolucao[k]][candidatos[escolhido]];
        k++;
        dados.vetSolucao[k] = candidatos[escolhido];

        selecionada[candidatos[escolhido]] = 1;
        premioAlcancado = 0;
        for (j = 0; j <= k; j++)
        {
            premioAlcancado += dados.vetPremio[dados.vetSolucao[j]];
        }


        for (j = 0; j < dados.qtdCidades; j++)
        {
            candidatos[j] = 0;
        }
        index = 1;
    }

    // distancia entre a última e a primeira cidade
    dados.distTotal += dados.matrizDistancia[dados.vetSolucao[0]][dados.vetSolucao[k]];

    for (i = 0; i < dados.qtdCidades; i++)
    {
        if (selecionada[i] == 0)
        {
            dados.distTotal += dados.vetPenalidade[i];
        }
    }

    FOStar = dados.distTotal;

    return k;
}

int insercaoMaisBarata(Dados dados, int* selecionada, int indice){

    float distanciaInsercao, distancia = 9999999;
    int aux, aux2, flag = 0;
    k = indice;

    do
    {
        flag = 0;
        aux = -1;
        aux2 = -1;
        for(j = 0; j <= k; j++)
        {

            for(i = 0; i < dados.qtdCidades; i++)
            {
                if (selecionada[i] == 0)
                {

                    if (j == 0)
                    {
                        distanciaInsercao = ((dados.matrizDistancia[i][dados.vetSolucao[k]] + dados.matrizDistancia[i][dados.vetSolucao[j]]) - dados.matrizDistancia[dados.vetSolucao[k]][dados.vetSolucao[j]]) - dados.vetPenalidade[i];
                        if (distancia > distanciaInsercao && distanciaInsercao < 0)
                        {
                            flag = 1;
                            distancia = distanciaInsercao;
                            aux = j;
                            aux2 = i;
                        }

                    } else
                    {
                        distanciaInsercao = (dados.matrizDistancia[i][dados.vetSolucao[j-1]] + dados.matrizDistancia[i][dados.vetSolucao[j]]) - dados.matrizDistancia[dados.vetSolucao[j-1]][dados.vetSolucao[j]] - dados.vetPenalidade[i];
                        if (distancia > distanciaInsercao && distanciaInsercao < 0)
                        {
                            flag = 1;
                            distancia = distanciaInsercao;
                            aux = j;
                            aux2 = i;
                        }

                    }
                }
            }
        }

        if(selecionada[aux2] == 0 && flag == 1)
        {
            dados.distTotal += distancia;
            selecionada[aux2] = 1;
            k++;
            for(j = k; j > aux; j--)
            {
                dados.vetSolucao[j] = dados.vetSolucao[j-1];
            }
            dados.vetSolucao[aux] = aux2;
            distancia = 999999;
        }

    }while(flag == 1);

    return k;
}

float doisOptBest(Dados dados, float FOStar){
        //2opt - bestimprovements
        int *solucaoAtual = (int *)malloc(dados.qtdCidades * sizeof(int));
        float FOvizinho, FOatual = FOStar, FOmelhorvizinho = FOStar;
        for (i = 0; i <= k; i++)
        {
          solucaoAtual[i] = dados.vetSolucao[i]; //salvando melhor vizinho
        }
        int ref1, ref2;
        int melhoria = 0;
        float retirada, inserida;
        while(melhoria == 0){
            melhoria = 1;
            for(int i = 1; i< k-1; i++){
                for(int j = i+1; j< k-1; j++){
                    retirada = dados.matrizDistancia[solucaoAtual[i-1]][solucaoAtual[i]] + dados.matrizDistancia[solucaoAtual[j]][solucaoAtual[j+1]];
                    inserida = dados.matrizDistancia[solucaoAtual[i]][solucaoAtual[j+1]] + dados.matrizDistancia[solucaoAtual[i-1]][solucaoAtual[j]];
                    FOvizinho = FOatual - retirada + inserida;
                    if(FOvizinho < FOmelhorvizinho){
                        melhoria = 0;
                        FOmelhorvizinho = FOvizinho;
                        ref1 = i;
                        ref2 = j;

                    }
                }
            }

            if(melhoria == 0){
                int aux2 = 0;
                int n_trocas = ceil((ref2-ref1)/2);
                for(int aux2 = 0; aux2 <= n_trocas; aux2++){
                    int aux = solucaoAtual[ref1+aux2];
                    solucaoAtual[ref1+aux2] = solucaoAtual[ref2-aux2];
                    solucaoAtual[ref2-aux2] = aux;
                }
                FOatual = FOmelhorvizinho;
            }
        }

        FOStar = FOatual;
        for (i = 0; i <= k; i++)
        {
            dados.vetSolucao[i] = solucaoAtual[i];

        }
        return FOStar;
}

float doisOptFirst(Dados dados, float FOStar){
        //2opt - bestimprovements
        int *solucaoAtual = (int *)malloc(dados.qtdCidades * sizeof(int));
        float FOvizinho, FOatual = FOStar, FOmelhorvizinho = FOStar;
        for (i = 0; i <= k; i++)
        {
          solucaoAtual[i] = dados.vetSolucao[i]; //salvando melhor vizinho
        }
        int ref1, ref2;
        int melhoria = 0;
        float retirada, inserida;
        while(melhoria == 0){
            melhoria = 1;
            for(int i = 1; i< k-1; i++){
                for(int j = i+1; j< k-1; j++){
                    retirada = dados.matrizDistancia[solucaoAtual[i-1]][solucaoAtual[i]] + dados.matrizDistancia[solucaoAtual[j]][solucaoAtual[j+1]];
                    inserida = dados.matrizDistancia[solucaoAtual[i]][solucaoAtual[j+1]] + dados.matrizDistancia[solucaoAtual[i-1]][solucaoAtual[j]];
                    FOvizinho = FOatual - retirada + inserida;
                    if(FOvizinho<FOmelhorvizinho){
                        melhoria = 0;
                        FOmelhorvizinho = FOvizinho;
                        ref1 = i;
                        ref2 = j;
                        i = dados.qtdCidades;
                        j = dados.qtdCidades;
                    }
                }
            }

            if(melhoria == 0){
                int aux2 = 0;
                int n_trocas = ceil((ref2-ref1)/2);
                for(int aux2 = 0; aux2 <= n_trocas; aux2++){
                    int aux = solucaoAtual[ref1+aux2];
                    solucaoAtual[ref1+aux2] = solucaoAtual[ref2-aux2];
                    solucaoAtual[ref2-aux2] = aux;
                }
                FOatual = FOmelhorvizinho;
            }
        }

        FOStar = FOatual;
        for (i = 0; i <= k; i++)
        {
            dados.vetSolucao[i] = solucaoAtual[i];

        }
        return FOStar;
}

float orOptFirst (Dados dados, float FOStar){
    int *solucaoAtual = (int *)malloc(dados.qtdCidades * sizeof(int));
    float FOvizinho = 0, FOatual = FOStar, FOmelhorvizinho = FOStar;
    int melhoria = 0, retirada, inserida, ref1, ref2, aux;
    for (i = 0; i <= k; i++)
    {
        solucaoAtual[i] = dados.vetSolucao[i]; //salvando melhor vizinho
    }

    while(melhoria == 0)
    {
        melhoria = 1;
        for (i = 1; i < k; ++i)
        {
            for (j = i+1; j <= k; ++j)
            {
                aux = solucaoAtual[i];
                solucaoAtual[i] = solucaoAtual[j];
                solucaoAtual[j] = aux;

                for (int l = 0; l < k; l++)
                {
                    FOvizinho += dados.matrizDistancia[solucaoAtual[l]][solucaoAtual[l+1]];
                }

                FOvizinho += dados.matrizDistancia[solucaoAtual[0]][solucaoAtual[k]];


                if(FOvizinho < FOmelhorvizinho)
                {
                        melhoria = 0;
                        FOmelhorvizinho = FOvizinho;
                        i = dados.qtdCidades;
                        j = dados.qtdCidades;
                } else {
                        aux = solucaoAtual[i];
                        solucaoAtual[i] = solucaoAtual[j];
                        solucaoAtual[j] = aux;
                }
                FOvizinho = 0;
            }
        }
        if(melhoria == 0)
        {
            FOatual = FOmelhorvizinho;
        }
    }

    FOStar = FOatual;
    for (i = 0; i <= k; i++)
    {
        dados.vetSolucao[i] = solucaoAtual[i];
    }
    return FOStar;
}

float orOptBest (Dados dados, float FOStar){
    int *solucaoAtual = (int *)malloc(dados.qtdCidades * sizeof(int));
    float FOvizinho = 0, FOatual = FOStar, FOmelhorvizinho = FOStar;
    int melhoria = 0, retirada, inserida, ref1, ref2, aux;
    for (i = 0; i <= k; i++)
    {
        solucaoAtual[i] = dados.vetSolucao[i]; //salvando melhor vizinho
    }

    while(melhoria == 0)
    {
        melhoria = 1;
        for (int i = 1; i < k; ++i)
        {
            for (int j = i+1; j <= k; ++j)
            {
                aux = solucaoAtual[i];
                solucaoAtual[i] = solucaoAtual[j];
                solucaoAtual[j] = aux;

                for (int l = 0; l < k; l++)
                {
                    FOvizinho += dados.matrizDistancia[solucaoAtual[l]][solucaoAtual[l+1]];
                }

                FOvizinho += dados.matrizDistancia[solucaoAtual[0]][solucaoAtual[k]];


                if(FOvizinho < FOmelhorvizinho)
                {
                        melhoria = 0;
                        FOmelhorvizinho = FOvizinho;
                } else {
                        aux = solucaoAtual[i];
                        solucaoAtual[i] = solucaoAtual[j];
                        solucaoAtual[j] = aux;
                }
                FOvizinho = 0;
            }
        }
        if(melhoria == 0)
        {
            FOatual = FOmelhorvizinho;
        }
    }

    FOStar = FOatual;
    for (i = 0; i <= k; i++)
    {
        dados.vetSolucao[i] = solucaoAtual[i];
    }
    return FOStar;
}

float VND(Dados dados, float FOStarMulti, int* solucaoUm)
{
    int buscaLocal = 1;
    float FOVND = 999999;
    while (buscaLocal <= 4)
    {
        switch (buscaLocal)
        {
            case 1:

                for (i = 0; i <= k; i++)
                {
                    solucaoUm[i] = dados.vetSolucao[i];
                }
                FOVND = doisOptFirst(dados,FOStarMulti);
                if (FOVND < FOStarMulti)
                {
                    FOStarMulti = FOVND;
                    buscaLocal = 1;
                } else {
                    for (i = 0; i <= k; i++)
                    {
                        dados.vetSolucao[i] = solucaoUm[i];
                    }
                    buscaLocal++;
                }
                break;

            case 2:

                for (i = 0; i <= k; i++)
                {
                    solucaoUm[i] = dados.vetSolucao[i];
                }
                FOVND = orOptFirst(dados,FOStarMulti);
                if (FOVND < FOStarMulti)
                {
                    FOStarMulti = FOVND;
                    buscaLocal = 1;
                } else {
                    for (i = 0; i <= k; i++)
                    {
                        dados.vetSolucao[i] = solucaoUm[i];
                    }
                    buscaLocal++;
                }
                break;

            case 3:

                for (i = 0; i <= k; i++)
                {
                    solucaoUm[i] = dados.vetSolucao[i];
                }
                FOVND = doisOptBest(dados,FOStarMulti);
                if (FOVND < FOStarMulti)
                {
                    FOStarMulti = FOVND;
                    buscaLocal = 1;
                } else {
                    for (i = 0; i <= k; i++)
                    {
                        dados.vetSolucao[i] = solucaoUm[i];
                    }
                    buscaLocal++;
                }
                break;

            case 4:

                for (i = 0; i <= k; i++)
                {
                    solucaoUm[i] = dados.vetSolucao[i];
                }
                FOVND = orOptBest(dados,FOStarMulti);
                if (FOVND < FOStarMulti)
                {
                    FOStarMulti = FOVND;
                    buscaLocal = 1;
                } else {
                    for (i = 0; i <= k; i++)
                    {
                        dados.vetSolucao[i] = solucaoUm[i];
                    }
                    buscaLocal++;
                }
                break;

            default:
                break;
        }
    }
    return FOStarMulti;
}

int remocaoMaisBarata(Dados dados, int* selecionada, float premioAlcancado, float premioMinimo, int k)
{
    float distanciaRemocao, distancia = 9999999;
    int *copiaSolucao = (int *)malloc(dados.qtdCidades * sizeof(int));
    int aux, aux2, flag = 0;
    do
    {
        flag = 0;
        aux = -1;
        aux2 = -1;

        for(i = 0; i < k; i++)
        {
            if (selecionada[dados.vetSolucao[i]] == 1)
            {

                if (i == 0)
                {
                    distanciaRemocao = (dados.matrizDistancia[dados.vetSolucao[k]][dados.vetSolucao[i+1]] + dados.vetPenalidade[i]) - (dados.matrizDistancia[i][dados.vetSolucao[k]] + dados.matrizDistancia[i][dados.vetSolucao[i+1]]);
                    if (distancia > distanciaRemocao && distanciaRemocao < 0)
                    {
                        flag = 1;
                        distancia = distanciaRemocao;
                        aux = i;
                        aux2 = dados.vetSolucao[i];
                    }

                } else
                {
                    distanciaRemocao = (dados.matrizDistancia[dados.vetSolucao[i-1]][dados.vetSolucao[i+1]] + dados.vetPenalidade[i]) - (dados.matrizDistancia[i][dados.vetSolucao[i-1]] + dados.matrizDistancia[i][dados.vetSolucao[i+1]]);
                    if (distancia > distanciaRemocao && distanciaRemocao < 0)
                    {
                        flag = 1;
                        distancia = distanciaRemocao;
                        aux = i;
                        aux2 = dados.vetSolucao[i];
                    }

                }
            }
        }

        if(selecionada[aux2] == 1 && flag == 1 && (premioAlcancado -= dados.vetPremio[aux2] > premioMinimo))
        {
            premioAlcancado -= dados.vetPremio[aux2];
            dados.distTotal += distancia;
            selecionada[aux2] = 0;
            k--;
            for(j = aux; j <= k; j++)
            {
                dados.vetSolucao[j] = dados.vetSolucao[j+1];
            }
            distancia = 999999;
        }

    }while(flag == 1);

    return k;
}

int main(int argc, char *argv[ ])
{

    FILE *arq;
    FILE *arq2;
    Dados dados;
    int aux, inicio,  k, c = 0, iter = 0, buscaLocal;
    float distancia, FOStar = 99999999, solucaoOtima,x, y, FOStarMulti = 9999999, alfa = 0.1, FOdeS;
    float FOPerturbacao, premioMinimo = 0, premioAlcancado = 0;
    int *melhorSolucaoMulti;
    int *solucaoUm;
    int *selecionada;

    clock_t comeco, fim;
    double tempo;

    arq = fopen("Instancias/v20.txt", "r");

    if (arq == NULL)
    {
        printf("Erro ao ler arquivo \n");
    }
    else
    {
        // lê o arquivo com a quantidade de cidades e a solução ótima
        fscanf(arq, "%d", &dados.qtdCidades);

        dados.matrizDistancia = (float **)malloc(dados.qtdCidades * sizeof(float *));
        dados.vetSolucao = (int *)malloc(dados.qtdCidades * sizeof(int));
        selecionada = (int *)malloc(dados.qtdCidades * sizeof(int));
        dados.vetSolucaoStar = (int *)malloc(dados.qtdCidades * sizeof(int));
        melhorSolucaoMulti = (int *)malloc(dados.qtdCidades * sizeof(int));
        solucaoUm = (int *)malloc(dados.qtdCidades * sizeof(int));
        dados.vetPremio = (float *)malloc(dados.qtdCidades * sizeof(float));
        dados.vetPenalidade = (float *)malloc(dados.qtdCidades * sizeof(float));
        dados.distTotal = 0;

        // lê as coordenadas e preenche a matriz de distância entre as cidades
       lerDados(dados,arq,aux);

       for (i=0; i< dados.qtdCidades;i++)
       {
           selecionada[i] = 0;
           premioMinimo += dados.vetPremio[i];
       }
       premioMinimo *= 0.75; ///Premio minimo é 75% do premio total
        comeco = clock();
        // for que percorre todas as cidades em busca da melhor solução
        for (int l = 0; l < dados.qtdCidades; l++){
            k = vizinhoGRASP(inicio,l,dados,selecionada,FOStar,alfa,premioMinimo);
            dados.distTotal = 0;

            for (i = 0; i < k; ++i)
            {
                dados.distTotal += dados.matrizDistancia[dados.vetSolucao[i]][dados.vetSolucao[i+1]];
            }

            dados.distTotal += dados.matrizDistancia[dados.vetSolucao[0]][dados.vetSolucao[k]];

            for (i = 0; i < dados.qtdCidades; ++i)
            {
                if (selecionada[i] == 0)
                {
                    dados.distTotal += dados.vetPenalidade[i];
                }
            }

            k = remocaoMaisBarata(dados,selecionada,premioAlcancado,premioMinimo,k);

            dados.distTotal = 0;

            for (i = 0; i < k; ++i)
            {
                dados.distTotal += dados.matrizDistancia[dados.vetSolucao[i]][dados.vetSolucao[i+1]];
                premioAlcancado += dados.vetPremio[dados.vetSolucao[i]];
            }

            dados.distTotal += dados.matrizDistancia[dados.vetSolucao[0]][dados.vetSolucao[k]];

            for (i = 0; i < dados.qtdCidades; ++i)
            {
                if (selecionada[i] == 0)
                {
                    dados.distTotal += dados.vetPenalidade[i];
                }
            }

            k = insercaoMaisBarata(dados,selecionada,k);
            dados.distTotal = 0;

            for (i = 0; i < k; ++i)
            {
                dados.distTotal += dados.matrizDistancia[dados.vetSolucao[i]][dados.vetSolucao[i+1]];
                premioAlcancado += dados.vetPremio[dados.vetSolucao[i]];
            }

            dados.distTotal += dados.matrizDistancia[dados.vetSolucao[0]][dados.vetSolucao[k]];

            for (i = 0; i < dados.qtdCidades; ++i)
            {
                if (selecionada[i] == 0)
                {
                    dados.distTotal += dados.vetPenalidade[i];
                }
            }

            dados.distTotal = VND(dados,dados.distTotal,solucaoUm);

            dados.distTotal = 0;

            for (i = 0; i < k; ++i)
            {
                dados.distTotal += dados.matrizDistancia[dados.vetSolucao[i]][dados.vetSolucao[i+1]];
            }

            dados.distTotal += dados.matrizDistancia[dados.vetSolucao[0]][dados.vetSolucao[k]];

            for (i = 0; i < dados.qtdCidades; ++i)
            {
                if (selecionada[i] == 0)
                {
                    dados.distTotal += dados.vetPenalidade[i];
                }
            }

            if (dados.distTotal < FOStar)
            {
                FOStar = dados.distTotal;
                for (i = 0; i <= k; i++)
                {
                    dados.vetSolucaoStar[i] = dados.vetSolucao[i];
                }
            }

            for (i = 0; i < dados.qtdCidades; i++)
            {
                selecionada[i] = 0;
                dados.vetSolucao[i] = 0;
            }
            dados.distTotal = 0;
            premioAlcancado = 0;
        }

        fim = clock();
        tempo = (double)(fim - inicio)/(double)CLOCKS_PER_SEC;
    }
    //printf("\n***********************************************************************\n");
    printf("Solucao encontrada: %20.2f\n", FOStar);
    //printf("Solucao Otima: %f \n", solucaoOtima);
    //printf("GAP: %f \n", (100*(FOStarMulti-solucaoOtima)/solucaoOtima));
    printf("Tempo: %f\n", tempo);
    /*for (i = 0; i <= k; ++i)
    {
        printf("%d ", dados.vetSolucaoStar[i]);
    }*/

    fclose(arq);
    free(selecionada);
    free(dados.matrizDistancia);
    free(dados.vetSolucao);
    free(dados.vetSolucaoStar);
    free(dados.vetPenalidade);
    free(dados.vetPremio);

    return 0;
}
