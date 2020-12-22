#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    char **programa;
    int data;
    int tam_memoria;
    int tam_programa;
    int entrada;
    int saida;
} t_jobs;

int count_progam_lines(FILE *file)
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
    countword -= 3;
    rewind(file);
    return countword;
}

t_jobs create_job(FILE *file)
{
    t_jobs job;
    job.tam_programa = count_progam_lines(file);
    job.programa = malloc(job.tam_programa * sizeof(char *));
    if (!job.programa)
    {
        printf("not enough memory!");
        exit(EXIT_FAILURE);
    }

    int num;
    int flag = 0;
    while (flag < 4)
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
        if (flag == 2)
        {
            job.entrada = num;
        }
        if (flag == 3)
        {
            job.saida = num;
        }
        flag++;
    }
    char c;
    int ptr = ftell(file);
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