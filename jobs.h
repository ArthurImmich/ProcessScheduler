#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    char **programa;
    int data;
    int tam_memoria;
    int tam_programa;
    int **entrada;
    int **saida;
    int *posicao_entrada;
    int *posicao_saida;
    int qtd_saida;
    int qtd_entrada;
} t_jobs;

int count_program_lines(FILE *file)
{
    char c;
    int countword = 0;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            countword++;
        }
    }
    countword++;
    return countword;
}

t_jobs create_job(FILE *file)
{
    t_jobs job;
    int num;
    int tam_entrada;
    int tam_saida;
    int flag = 0;

    while (flag < 2)
    {
        fscanf(file, "%i\n", &num);
        if (flag == 0)
        {
            job.data = num;
        }
        if (flag == 1)
        {
            job.tam_memoria = num;
        }
        flag++;
    }

    //configura disp de entrada
    fscanf(file, "%i\n", &job.qtd_entrada);
    job.entrada = malloc(job.qtd_entrada * sizeof(int *));
    job.posicao_entrada = malloc(job.qtd_entrada * sizeof(int));
    for (int i = 0; i < job.qtd_entrada; i++)
    {
        fscanf(file, "%i\n", &tam_entrada);
        job.entrada[i] = malloc(tam_entrada * sizeof(int));
        for (int j = 0; j < tam_entrada; j++)
        {
            fscanf(file, "%i\n", &job.entrada[i][j]);
        }
    }

    //configura disp de saida
    fscanf(file, "%i\n", &job.qtd_saida);
    job.saida = malloc(job.qtd_saida * sizeof(int *));
    job.posicao_saida = malloc(job.qtd_saida * sizeof(int));
    for (int i = 0; i < job.qtd_saida; i++)
    {
        fscanf(file, "%i\n", &tam_saida);
        job.saida[i] = malloc(tam_saida * sizeof(int));
        fscanf(file, "%i\n", &job.saida[i][0]);
    }

    int ptr = ftell(file);
    job.tam_programa = count_program_lines(file);
    job.programa = malloc(job.tam_programa * sizeof(char *));
    if (!job.programa)
    {
        printf("not enough memory!");
        exit(EXIT_FAILURE);
    }
    fseek(file, ptr, SEEK_SET);
    char c;
    int i = 0;
    int word_size = 0;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            job.programa[i] = malloc((word_size + 1));
            i++;
            word_size = 0;
        }
        else
        {
            word_size++;
        }
    }
    job.programa[i] = malloc((word_size + 1));
    i = 0;
    int j = 0;
    fseek(file, ptr, SEEK_SET);
    while ((c = fgetc(file)) != EOF)
    {
        if (c == '\n')
        {
            job.programa[i][j] = '\0';
            i++;
            j = 0;
        }
        else
        {
            job.programa[i][j] = c;
            j++;
        }
    }
    job.programa[i][j] = '\0';

    return job;
}
