/*
    Encontra a rota ideal em uma cidade, dado um arquivo no formato:

    destino
    nº de esquinas
    origem, destino, tempo
    ...
    origem, destino, tempo
    0

    Tito Guidotti

    1/11/2022
*/
#include <stdlib.h>
#include <stdio.h>

// aloca uma matriz de tamanho especificado preenchida com zeros
int **AlocarMatriz(int altura, int largura)
{
    // cria um vetor de pointers
    int **pointerVector = malloc(sizeof(int *) * altura);
    // aponta cada um deles para um vetor de inteiros
    for (int i = 0; i < largura; i++)
        pointerVector[i] = calloc(largura, sizeof(int));
    return pointerVector;
}

// libera matriz da altura especificada
void LiberarMatriz(int **matriz, int altura)
{
    for (int i = 0; i < altura; i++)
        free(matriz[i]);
    free(matriz);
}

// retorna a quantidade de posicoes ocupadas em resposta
int DecidirMenorCaminho(int *resposta, int *tempoTotal, int **custos, int destino, int esquinas)
{
    for (int i = 0; i < esquinas; i++)
    {
        for (int j = 0; j < esquinas; j++)
        {
            printf("%d ", custos[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // cria os vetores usados no método
    int *presente = malloc(sizeof(int) * esquinas);
    int *tempo = malloc(sizeof(int) * esquinas);
    int *r = malloc(sizeof(int) * esquinas);

    // preenche eles com os valores iniciais
    for (int i = 0; i < esquinas; i++)
    {
        presente[i] = 1;    // todos estão presentes
        tempo[i] = INT_MAX; // todos demoram tempo máx
        r[i] = -1;          // nenhum foi alcançado
    }

    tempo[0] = 0;

    int v;
    // roda uma vez para cada esquina
    for (int a = 0; a < esquinas; a++)
    {
        // mantém registro da menor distancia possível
        int menor = INT_MAX;
        // escolhe a esquina ainda não visitada mais próxima
        for (int i = 0; i < esquinas; i++)
            if (menor > tempo[i] && presente[i])
            {
                menor = tempo[i];
                v = i;
            }
        // marca que visitou ela
        presente[v] = 0;

        for (int e = 0; e < esquinas; e++)
        {
            // se pode ir de v para i
            if (custos[v][e] != 0)
            {
                // tempo de ir de v para i
                int tempoDeViagem = tempo[v] + custos[v][e];
                if (tempoDeViagem < tempo[e])
                {
                    tempo[e] = tempoDeViagem;
                    // registra a passagem
                    r[e] = v;
                }
            }
        }
    }

    int index = esquinas - 1;
    int no = destino - 1;
    // preenche o tempo para acesso externo
    *tempoTotal = tempo[no];

    // se não é possível
    if (r[no] == -1)
        return 0;

    printf("R = [ ");
    for (int i = 0; i < esquinas; i++)
    {
        printf("%d ", r[i]);
    }
    printf("]\n");

    // segue o no até o inicio
    while (no > 0)
    {
        resposta[index--] = no + 1;
        no = r[no];
    }
    // sempre começa no 1
    resposta[index] = 1;

    // desloca a resposta para o inicio do vetor
    for (int i = index; i < esquinas; i++)
        resposta[i - index] = resposta[i];

    // libera os vetores
    free(presente);
    free(tempo);
    free(r);

    // retorna a quantidade de etapas
    return esquinas - index;
}

int main()
{
    FILE *arq = fopen("entrada2.txt", "r");

    if (!arq)
    {
        perror("Erro ao abrir arquivo x");
        exit(1); // encerra o programa com status 1
    }

    int esquinaIncendio;
    int totalEsquinas;

    int origem;
    int tempo;
    int destino;

    fscanf(arq, "%d", &esquinaIncendio);
    fscanf(arq, "%d", &totalEsquinas);

    // aloca a matriz de custos (ou distancias)
    int **custos = AlocarMatriz(totalEsquinas, totalEsquinas);

    // lê todos os caminhos disponives
    fscanf(arq, "%d", &origem);
    while (origem != 0)
    {
        fscanf(arq, "%d", &destino);
        fscanf(arq, "%d", &tempo);
        // preenche o custo na posição correspondente
        custos[origem - 1][destino - 1] = tempo;
        fscanf(arq, "%d", &origem);
    }

    int tempoDaRota;
    // aloca vetor-resposta
    int *rota = malloc(totalEsquinas * sizeof(int));

    int etapas = DecidirMenorCaminho(rota, &tempoDaRota, custos, esquinaIncendio, totalEsquinas);

    printf("rota até a esquina #%d: ", esquinaIncendio);
    if (etapas == 0)
        printf("impossível");
    else
    {
        for (int i = 0; i < etapas; i++)
            printf("%d ", rota[i]);
        printf("\ntempo calculado para rota = %d min.", tempoDaRota);
    }

    // libera o vetor-resposta
    free(rota);
    // libera matriz
    LiberarMatriz(custos, totalEsquinas);
}
