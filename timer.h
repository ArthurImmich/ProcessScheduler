#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct interr
{
    char *tipo;
    unsigned int time;
    struct interr *next;
} interr_t;

typedef struct
{
    unsigned int count;
    bool termina;
    interr_t *interr;
} timer_t;

//TO-DO

void timer_inicializa(timer_t *timer)
{
    timer->count = 0;
    timer->termina = false;
    timer->interr = NULL;
}

void timer_int_de(timer_t *timer, unsigned int time, char *tipo)
{
    interr_t *aux = malloc(sizeof(interr_t));
    if (!aux)
    {
        printf("Falta de memoria");
        exit(1);
    }
    aux->tipo = tipo;
    aux->time = timer->count + time;
    aux->next = timer->interr;
    timer->interr = aux;
}

void timer_int_as(timer_t *timer, unsigned int time, char *tipo)
{
    interr_t *aux = malloc(sizeof(interr_t));
    if (!aux)
    {
        printf("Falta de memoria");
        exit(1);
    }
    aux->tipo = tipo;
    aux->time = time;
    aux->next = timer->interr;
    timer->interr = aux;
}

void timer_tictac(timer_t *timer)
{
    timer->count++;
};

char *timer_interrupcao(timer_t *timer)
{
    interr_t *aux;
    for (aux = timer->interr; !!aux; aux = aux->next)
    {
        if (timer->count == aux->time)
        {
            return aux->tipo;
        }
    }
    return "nenhum";
}

void timer_destroi(timer_t *timer)
{
    interr_t *aux;
    for (aux = timer->interr; !!timer->interr; aux = timer->interr)
    {
        timer->interr = timer->interr->next;
        free(aux);
    }
    timer->count = 0;
}

unsigned int timer_agora(timer_t *timer)
{
    return timer->count;
}