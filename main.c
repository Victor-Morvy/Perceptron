#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Perceptron.h"

//COMPILADO no QT 5.15.2 Pacote MSVC 2019

//Escreve no arquivo o resultado dos pesos.
void escreverLinha(char* arquivo, Perceptron* p)
{
    FILE* file = fopen(arquivo, "a");

    //print para poder ver o resultado.
    printf("%0.3f %0.3f %0.3f %0.3f | %0.3f %0.3f %0.3f %0.3f | %d\n",
            p->init_weights[0], p->init_weights[1], p->init_weights[2], p->init_weights[3],
            p->weights[0], p->weights[1], p->weights[2], p->weights[3], p->epocas);

    //Salvando os valores como na tabela 3.2 do livro Artificial Neural Networks for Engineering Application | ISBN 10:0128182474
    fprintf(file, "%0.3f %0.3f %0.3f %0.3f | %0.3f %0.3f %0.3f %0.3f | %d\n",
            p->init_weights[0], p->init_weights[1], p->init_weights[2], p->init_weights[3],
            p->weights[0], p->weights[1], p->weights[2], p->weights[3], p->epocas);

    fclose(file);
}

int main()
{
    srand(time(NULL));
    //Local onde será salvo os pesos treinados, referente ao ex 2
    char* file = "./Output.txt";

    /////////////////////////////////////
    ///----- FASE de treinamento -----///
    /////////////////////////////////////
    printf("\nFase de treinamento\n");

    //limpando arquivo...
    FILE* fp = fopen(file, "w");
    fclose(fp);

    //Alocando vetor de amostras
    Amostra* dados = (Amostra*) malloc(sizeof (Amostra) * FILE_SIZE );

    //tabela de treinamento do perceptron
    FILE* fp2 = fopen("./t.txt", "r");

    int k = 0;
    while(!feof(fp2))
    {
        if(k == 30)
            break;

        float tmpValue[3];
        float tmpRes;

        fscanf(fp2, "%f %f %f %f\n",
               &tmpValue[0],
                &tmpValue[1],
                &tmpValue[2],
                &tmpRes);

        //Define dados das amostras lido no arquivo
        setdados(&dados[k], tmpValue[0], tmpValue[1],
                            tmpValue[2], (int)tmpRes);

        k++;
    }

    //Gera o resultado dos neuronios
    Perceptron* neuronio2 = getResultados(dados);

    for(int i = 0; i < DATASIZE; i++)
    {
        //Escreve as linhas no arquivo .txt
        escreverLinha(file, &neuronio2[i]);
    }

    //////////////////////////////////
    ///----- FASE de operação -----///
    //////////////////////////////////

    printf("\n---------------------\n\nFase de operacao\n");

    //Alocando vetor
    Validador* validators = (Validador*) malloc(sizeof (Validador) * 10);

    //Valores que serão validados
    FILE* fTests = fopen("testValues.txt", "r");

    //Carregando valores a serem validados
    int j = 0;
    while(!feof(fTests))
    {
        float tmpValues[3];
        fscanf(fTests, "%f %f %f\n",
               &tmpValues[0],
                &tmpValues[1],
                &tmpValues[2]);

        //Inicia os valores do validador.
        initValidator(&validators[j], tmpValues[0],
                    tmpValues[1], tmpValues[2]);
        j++;
    }

    fclose(fTests);

    //Salvando arquivo de resultados de valores validados
    FILE* fsave = fopen("resultados.txt", "w");

    for(int i = 0; i < 10; i++)
    {

        printf("%f %f %f", validators[i].inputs[1],
                validators[i].inputs[2], validators[i].inputs[3]);

        fprintf(fsave, "%f %f %f", validators[i].inputs[1],
                validators[i].inputs[2], validators[i].inputs[3]);

        for(int k = 0; k < DATASIZE; k++)
        {
            //faz o processo inverso, utilizando o neurônio treinado na fase anterior
            float u = somarAmostra(validators[i].inputs,
                                   &neuronio2[k].weights[0], SIZE);
            //sinal
            int y = FuncaoAtv(u);
            printf(" %d", y);

            //valores gerados por cada perceptron
            fprintf(fsave, " %d", y);

        }

        printf("\n");
        fprintf(fsave, "\n");

    }

    fclose(fsave);

    return 0;
}

