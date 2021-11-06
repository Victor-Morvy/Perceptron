#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <stdlib.h>

#define SIZE 4 //tamanho do input
#define DATASIZE 5 //quantidade de dados treinados
#define taxaAprendizagem 0.01 //Taxa de aprendizagem do perceptron
#define FILE_SIZE 30 //Tamanho de dados de entrada

typedef struct validador Validador; //Validador inputs & output
typedef struct perceptron Perceptron; //Perceptron neurônio
Perceptron* createPerceptron(); //Cria o perceptron e gera valor aleatorio para os inputs
void iniciaVetor(float vector[], int size); // inicia os vetores com valores aleatórios
void recalculaRepetido(float vector[], int size); // muda o valor para um valor aleatório caso seja repetido de um único vetor

typedef struct Amostra Amostra; //Amostras existentes para treinar a rede neural
void setdados(Amostra* p, float x1, float x2, float x3, int resultado); //Define os valores de uma amostra
Perceptron* getResultados(Amostra* am); //Retorna os resultados de peso inicial, peso final e época

void cleanTwice_(Perceptron* p, int size); //cria novos valores para valores repetidos
void copyValues(float* from, float* to, int size); //copia vetor de float
void convergir(Perceptron* p, Amostra* am); //converge neurônio para os pesos sinápticos com output de interesse

float rndValue(); //gera um valor aleatório de 0 a 1
float somarAmostra(float* x, float* w, int vecSize); //somatória de xi*wi
int FuncaoAtv(float resNeuronio); //função de ativação retorna -1 ou 1
void treinar(float* inputs, float* weights, int vecSize, int target, int y); //treina o perceptron

typedef struct validador{
    float inputs[SIZE];
    float y;
}Validador ;

//Inivia os valores do validador
void initValidator(Validador* v, float x1, float x2, float x3)
{
    v->inputs[0] = -1;
    v->inputs[1] = x1; v->inputs[2] = x2; v->inputs[3] = x3;

//    v->inputs[0] = x1; v->inputs[1] = x2; v->inputs[2] = x3;
    for(int i = 0; i < DATASIZE; i++)
        v->y = 0;
}

//Amostras para treinar a rede neural
typedef struct Amostra{
    float x0;
    float x1;
    float x2;
    float x3;
    int result; //d
}Amostra;

//Define os dados da amostra.
void setdados(Amostra* p, float x1, float x2, float x3, int resultado)
{
    p->x0 = -1;
    p->x1 = x1;
    p->x2 = x2;
    p->x3 = x3;
    p->result = resultado;
}

//Neurônio
typedef struct perceptron{ //treinamento
    float weights[SIZE];
    float init_weights[SIZE];
    int epocas;
} Perceptron;

//Função para criar e inicializar o neurônio
Perceptron* createPerceptron()
{
    //Alocand o neurônio
    Perceptron* p = (Perceptron*) malloc(sizeof (Perceptron));
    //Iniciando vetor de pesos
    iniciaVetor(p->weights, SIZE);
    int i;
    for( i = 0; i < SIZE; i ++)
    {
        //Copiando valores de pesos iniciais.
        p->init_weights[i] = p->weights[i];
    }

    p->epocas = 0;

    return p;
}

//retorna um valor aleatório de 0 a 1
float rndValue()
{
    float decimals = 100.f;

    float val = (float)((rand() % (int)decimals) / decimals);
//    printf("%f\n", val);
    return val;
}

//muda o valor para um valor aleatório caso seja repetido de um único vetor
void recalculaRepetido(float vector[], int size)
{
    int i = 0;
    int repetido = 1;
    //Enquanto existir número repetido no vetor...
    while(repetido > 0)
    {
        if(i >= size)
            i = 0;

        repetido = 0;
        int j;
        for( j = i; j < size; j++)
        {
            if(vector[i] == vector[j] && i != j)
            {
                repetido = 1;
                //Gerando outro numero
                vector[i] = rndValue();
            }
        }
        i++;
    }
}

//Inicia os vetores de peso com valores aleatórios
void iniciaVetor(float vector[], int size)
{
    int i;
    for(i = 0; i < size; i++)
        vector[i] = rndValue();

    //Testa se existe valores repetidos.
    recalculaRepetido(vector, size);
}

//Função de ativação, também chamada de sinal/signal
int FuncaoAtv(float resNeuronio) //sinal/signal
{
    return resNeuronio >= 0 ? 1 : -1;
}

//Somatória dos pesos sinápticos
float somarAmostra(float* x, float* w, int vecSize)
{
    float somatoria = 0;
    int i;
    for(i = 0; i < vecSize; i++)
        somatoria += (x[i]*w[i]); //Somatória dos inputs, multiplicado pelo peso

    return somatoria;
}

//Treina os pesos sinápticos baseando-se no valor de interesse y.
void treinar(float* inputs, float* weights, int vecSize, int target, int y)
{
    //valor de saida atual
    int valorPossivel = y;

    //calculando diferença de erro
    int error = target - valorPossivel;

    //Aproximando valores para o resultado esperado.
    int i;
    for( i = 0; i < vecSize; i++)
        weights[i] += error * inputs[i] * taxaAprendizagem;

}

//Convergir valores de amostra
void convergir(Perceptron* p, Amostra* am)
{
    Amostra* dados = am;

    int erro = 1;

    int epoca = 0;

    //Enquanto houver erro, não está treinado...
    while(erro)
    {
        erro = 0;

        int i;
        //Para cada dado de input
        for(i = 0; i < FILE_SIZE; i++)
        {
            float input[SIZE] = {dados[i].x0, dados[i].x1,
                             dados[i].x2, dados[i].x3,};

            //soma os pesos sinápticos
            float soma = somarAmostra(input, p->weights, SIZE);

            //Chama a função de ativação (sinal)
            int y = FuncaoAtv(soma);

            //Testa se o resultado é esperado ou não.
            if(y != dados[i].result)// se não for, treina de novo e repete até estar treinado
            {
                //se o resultado não for o esperado, é treinado novamente até não existir mais nenhum erro.
                treinar(input, p->weights, SIZE, dados[i].result, y);
                erro = 1;
            }

        }
        //Soma a época
        epoca++;
    }

    //Define a quantidade de epocas
    p->epocas = epoca;
}

//Retorna o perceptron já criado e treinado.
Perceptron* getResultados(Amostra* am)
{
    //alocar o vetor de Perceptron
    Perceptron* neurArray = (Perceptron*) malloc(sizeof (Perceptron) * DATASIZE);

    for(int i = 0; i < DATASIZE; i ++)
    {
        //Cria e inicializa valores do perceptron
        Perceptron* p = createPerceptron();

        //copia para a posição
        neurArray[i] = *p;

        //libera memória
        free(p);
    }

    //Chama a função para criar novos valores aleatórios para valores iguais em todo o vetor de perceptron
    //e em cada vetor de input
    cleanTwice_(neurArray, DATASIZE);

    //Faz a magia do treinamento
    for(int i = 0; i < DATASIZE; i ++)
    {
        convergir(&neurArray[i], am);
    }

    return neurArray;
}

//Copia valores de um vetor para outro
void copyValues(float* from, float* to, int size)
{
    for(int i = 0; i < size; i++)
    {
        to[i] = from[i];
    }
}

//percorre os vetor de neurônio garantindo que os valores dos pesos
//não sejam iguais
void cleanTwice_(Perceptron* p, int size)
{
    int repeat = 1;
    float* pivo;
    //Repetir até não existir valores iguais, custoso, mas garantido.
    while(repeat)
    {
        repeat = 0;
        for(int i = 0; i < size; i++)
        {
            for(int j = 0; i < SIZE; i++)
            {
                //Define o pivo
                pivo = &p[i].weights[j];
                for(int k = 0; k < size; k++)
                {//Percorre novamente o vetor para procurar valores repetidos
                    for(int l = 0; l < SIZE; l++)
                    {
                        //float são valores imprecisos, portanto para compara-los é convertido para inteiro
                        if(pivo != &p[k].weights[l] && (int)(*pivo * 1000.f) == (int)(p[k].weights[l] * 1000.f))
                        {//ao encontrar um valor repetido, é passado para o próximo
                         //valor, por isto foi colocado a variável para repetir e
                         //realmente garantir que não haverá valores repetidos.
                            repeat = 1;
                            printf("valor repetido %f\n", *pivo);

                            //Cria valor aleatorio entre 0 e 1
                            p[k].weights[l] = rndValue();
                            //copia os valores para o vetor inicial
                            copyValues(&p[k].weights[l], &p[k].init_weights[l], 1);
                        }
                    }
                }
            }

        }
    }
}

#endif // PERCEPTRON_H























