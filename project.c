/**
 * File: proj2.c
 * Author: Nuno Gonçalves (ist1103392)
 * Description: Sistema de gestão de voos entre aeroportos
 * Date: 2022-04-20
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proj2.h"


/**
 * Executa um sistema de gestão de voos entre aeroportos.
 * Permite a deﬁnição de aeroportos e voos, assim como a sua consulta.
 */
int main() {
    char comando;

    do {
        comando = getchar();

        switch (comando) {
            case 'q':
                break;
            case 'a':
                adiciona_aeroporto();
                break;
            case 'l':
                lista_aeroportos();
                break;
            case 'v':
                comando_v();
                break;
            case 'p':
                lista_voos_partida();
                break;
            case 'c':
                lista_voos_chegada();
                break;
            case 't':
                avanca_data();
                break;
            case 'r':
                comando_r();
                break;
            case 'e':
                comando_e();
                break;
        }
    } while (comando != 'q');

    /* Libertar a memória reservada para voos e reservas */
    free_voos();
    free_reservas();

    return 0;
}


/* Funções relativas aos aeroportos */

/**
 * Executa o comando 'a'.
 * Adiciona um novo aeroporto ao sistema, verificando a existência de erros.
 */
void adiciona_aeroporto() {
    int i;
    Aeroporto input_aeroporto;

    scanf("%s%s%*c", input_aeroporto.id, input_aeroporto.pais);
    fgets(input_aeroporto.cidade, MAX_AEROPORTO_CIDADE, stdin);

    /* Remover o caractere '\n' de input_aeroporto.cidade */
    for (i = 0; i < MAX_AEROPORTO_CIDADE; i++) {
        if (input_aeroporto.cidade[i] == '\n') {
            input_aeroporto.cidade[i] = '\0';
            break;
        }
    }

    /* Verificar se o input_aeroporto é inválido */
    if (aeroporto_invalido(input_aeroporto)) {
        return;
    }
    /* Adicionar o input_aeroporto ao sistema */
    else {
        input_aeroporto.voos = 0;
        sistema_aeroportos[nr_aeroportos] = input_aeroporto;
        nr_aeroportos++;

        printf("airport %s\n", input_aeroporto.id);
    }
}


/**
 * Executa o comando 'l'.
 * Lista os aeroportos do sistema, verificando a existência de erros.
 * Os aeroportos são listados por ordem alfabética do código (se for invocado
 * sem argumentos) ou pela ordem dos respetivos identiﬁcadores no comando.
 */
void lista_aeroportos() {
    int i, j;
    int nr_argumentos = 0;
    Bool aeroporto_invalido;
    Bool argumentos_opcionais = FALSE;
    char c;
    char argumentos[MAX_AEROPORTOS][MAX_AEROPORTO_ID];
    char aeroporto_invalido_id[MAX_AEROPORTO_ID];

    /* Preencher a tabela com os argumentos opcionais (se existirem) */
    while ((c = getchar()) != '\n') {
        if (c != ' ' && c != '\t') {
            argumentos[nr_argumentos][0] = c;
            argumentos[nr_argumentos][1] = (c = getchar());
            argumentos[nr_argumentos][2] = (c = getchar());
            argumentos[nr_argumentos][3] = '\0';

            nr_argumentos++;
            argumentos_opcionais = TRUE;
        }
    }

    if (!argumentos_opcionais) {
        /* Ordenar os aeroportos do sistema por ordem alfabética do ID */
        sort_by_id();

        for (i = 0; i < nr_aeroportos; i++) {
            printf("%s %s %s %d\n", sistema_aeroportos[i].id,
                                    sistema_aeroportos[i].cidade, 
                                    sistema_aeroportos[i].pais,
                                    sistema_aeroportos[i].voos);
        }
    }
    else {
        for (i = 0; i < nr_argumentos; i++) {
            aeroporto_invalido = TRUE;
            strcpy(aeroporto_invalido_id, argumentos[i]);

            for (j = 0; j < nr_aeroportos; j++) {
                /* Verificar se os argumentos são válidos */
                if (!strcmp(argumentos[i], sistema_aeroportos[j].id)) {
                    printf("%s %s %s %d\n", sistema_aeroportos[j].id,
                                            sistema_aeroportos[j].cidade,
                                            sistema_aeroportos[j].pais,
                                            sistema_aeroportos[j].voos);
                    
                    aeroporto_invalido = FALSE;
                    break;
                }
            }

            if (aeroporto_invalido) {
                printf(ERROR_NO_SUCH_AIRPORT_ID, aeroporto_invalido_id);
            }
        }
    }
}


/**
 * Recebe um aeroporto.
 * Retorna 1 se o aeroporto for inválido.
 * Retorna 0 em caso contrário.
 */
int aeroporto_invalido(Aeroporto aeroporto) {
    int i;
    Bool id_duplicado = FALSE;

    /* Verificar se o input_aeroporto.id é duplicado */
    for (i = 0; i < nr_aeroportos; i++) {
        if (!strcmp(sistema_aeroportos[i].id, aeroporto.id)) {
            id_duplicado = TRUE;
            break;
        }
    }

    if (!str_upper(aeroporto.id)) {
        printf(ERROR_INVALID_AIRPORT_ID);
    }
    /* Verificar se o sistema não excede o número máximo de aeroportos */
    else if (nr_aeroportos == MAX_AEROPORTOS) {
        printf(ERROR_TOO_MANY_AIRPORTS);
    }
    else if (id_duplicado) {
        printf(ERROR_DUPLICATE_AIRPORT);
    }
    else {
        return FALSE;
    }

    return TRUE;
}


/**
 * Ordena os aeroportos do sistema por ordem alfabética do ID.
 */
void sort_by_id() {
    int i;
    Bool sorted;

    /* Verificar se o sistema tem mais que um aeroporto */
    sorted = (nr_aeroportos >= 2 ? FALSE : TRUE);

    while (!sorted) {
        sorted = TRUE;
        for (i = 0; i < nr_aeroportos - 1; i++) {
            if (strcmp(sistema_aeroportos[i].id, sistema_aeroportos[i+1].id)>0){
                SWAP_AEROPORTOS(sistema_aeroportos[i], sistema_aeroportos[i+1]);
                sorted = FALSE;
            }
        }
    }
}


/* Funções relativas aos voos */

/**
 * Executa o comando 'v'.
 * Adiciona um voo ao sistema, verificando a existência de erros, ou lista
 * todos os voos.
 */
void comando_v() {
    char c;
    node_voo *node_i = NULL;

    if ((c = getchar()) != '\n') {
        adiciona_voo();
    }
    else {
        /* Listar todos os voos do sistema */
        for (node_i = head_voos; node_i != NULL; node_i = node_i->next) {
            printf("%s %s %s %s %s\n", node_i->voo.codigo,
                                        node_i->voo.partida,
                                        node_i->voo.chegada,
                                        node_i->voo.data,
                                        node_i->voo.hora);
        }
    }
}


/**
 * Função complementar a comando_v().
 * Adiciona um voo ao sistema, verificando a existência de erros.
 */
void adiciona_voo() {
    Voo input_voo;
    Bool partida_invalida = TRUE;
    int i;
    int aeroporto_partida = 0;

    scanf("%s%s%s%s%s%s%d", input_voo.codigo, input_voo.partida,
                            input_voo.chegada, input_voo.data,
                            input_voo.hora, input_voo.duracao,
                            &input_voo.capacidade);

    /* Verificar a validade do aeroporto de partida */
    for (i = 0; i < nr_aeroportos; i++) {       
        if (partida_invalida &&
                (!strcmp(input_voo.partida, sistema_aeroportos[i].id))) {
            aeroporto_partida = i;
            partida_invalida = FALSE;
        }
    }

    if (codigo_voo_invalido(input_voo.codigo)) {
        printf(ERROR_INVALID_FLIGHT_CODE);
        return;
    }
    else if (voo_duplicado(input_voo)) {
        printf(ERROR_FLIGHT_ALREADY_EXISTS);
        return;
    }
    else if (partida_invalida) {
        printf(ERROR_NO_SUCH_AIRPORT_ID, input_voo.partida);
        return;
    }
    else if (voo_invalido(input_voo)) {
        return;
    }
    /* Adicionar o input_voo ao sistema */
    else {
        input_voo.passageiros = 0;

        if (head_voos == NULL) {
            head_voos = (node_voo *) safe_malloc(sizeof(node_voo));
            head_voos->voo = input_voo;
            head_voos->next = NULL;
            last_voos = head_voos;
        }
        else {
            last_voos->next = (node_voo *) safe_malloc(sizeof(node_voo));
            last_voos = last_voos->next;
            last_voos->voo = input_voo;
            last_voos->next = NULL;
        }
        
        nr_voos++;
        sistema_aeroportos[aeroporto_partida].voos++;
    }
}


/**
 * Executa o comando 'p'.
 * Lista os voos com partida de um aeroporto, verificando a existência de erros.
 * Os voos são ordenados por data e hora.
 */
void lista_voos_partida() {
    int i;
    int lista_voos_i = 0;
    Bool id_invalido = TRUE;
    char aeroporto_id[MAX_AEROPORTO_ID];
    /* Este vetor irá ser utilizado para ordenar os voos com partida de um
    aeroporto */
    Voo lista_voos[MAX_VOOS];
    node_voo *node_i = NULL;

    scanf("%s", aeroporto_id);

    /* Verificar a validade do ID do aeroporto */
    for (i = 0; i < nr_aeroportos; i++) {
        if (!strcmp(sistema_aeroportos[i].id, aeroporto_id)) {
            id_invalido = FALSE;
            break;
        }
    }

    if (id_invalido) {
        printf(ERROR_NO_SUCH_AIRPORT_ID, aeroporto_id);
    }
    else {
        /* Preencher o vetor lista_voos */
        for (node_i = head_voos; node_i != NULL; node_i = node_i->next) {
            if (!strcmp(node_i->voo.partida, aeroporto_id)) {
                lista_voos[lista_voos_i] = node_i->voo;
                lista_voos_i++;
            }
        }

        /* Ordenar lista_voos por data e hora de partida */
        sort_by_date(lista_voos, lista_voos_i);

        for (i = 0; i < lista_voos_i; i++) {
            if (!strcmp(lista_voos[i].partida, aeroporto_id)) {
                printf("%s %s %s %s\n", lista_voos[i].codigo,
                                        lista_voos[i].chegada,
                                        lista_voos[i].data,
                                        lista_voos[i].hora);
            }
        }
    }
}


/**
 * Executa o comando 'c'.
 * Lista os voos com chegada a um aeroporto, verificando a existência de erros.
 * Os voos são ordenados por data e hora.
 */
void lista_voos_chegada() {
    int i;
    int lista_voos_i = 0;
    Bool id_invalido = TRUE;
    char aeroporto_id[MAX_AEROPORTO_ID];
    /* Este vetor irá ser utilizado para ordenar os voos com chegada a um
    aeroporto */
    Voo lista_voos[MAX_VOOS];
    node_voo *node_i = NULL;

    scanf("%s", aeroporto_id);

    /* Verificar a validade do ID do aeroporto */
    for (i = 0; i < nr_aeroportos; i++) {
        if (!strcmp(sistema_aeroportos[i].id, aeroporto_id)) {
            id_invalido = FALSE;
            break;
        }
    }

    if (id_invalido) {
        printf(ERROR_NO_SUCH_AIRPORT_ID, aeroporto_id);
    }
    else {
        /* Preencher o vetor lista_voos */
        for (node_i = head_voos; node_i != NULL; node_i = node_i->next) {
            if (!strcmp(node_i->voo.chegada, aeroporto_id)) {
                lista_voos[lista_voos_i] = node_i->voo;

                /* Calcular a data e hora de chegada dos voos */
                data_hora_chegada(lista_voos[lista_voos_i].data,
                                    lista_voos[lista_voos_i].hora,
                                    lista_voos[lista_voos_i].duracao);

                lista_voos_i++;
            }
        }

        /* Ordenar lista_voos por data e hora de chegada */
        sort_by_date(lista_voos, lista_voos_i);

        for (i = 0; i < lista_voos_i; i++) {
            if (!strcmp(lista_voos[i].chegada, aeroporto_id)) {
                printf("%s %s %s %s\n", lista_voos[i].codigo,
                                        lista_voos[i].partida,
                                        lista_voos[i].data,
                                        lista_voos[i].hora);
            }
        }

    }
}


/**
 * Recebe um voo.
 * Retorna 1 se o voo for inválido.
 * Retorna 0 em caso contrário.
 */
int voo_invalido(Voo voo) {
    int duracao_hor = 0, duracao_min = 0;

    /* Converter a duração para horas e minutos */
    if (voo.duracao[1] == ':') {
        /* Suporte para hora mal formatada */
        duracao_hor = ctoi(voo.duracao[0]);
        duracao_min = ctoi(voo.duracao[2]) * 10 + ctoi(voo.duracao[3]);
    }
    else {
        duracao_hor = ctoi(voo.duracao[0]) * 10 + ctoi(voo.duracao[1]);
        duracao_min = ctoi(voo.duracao[3]) * 10 + ctoi(voo.duracao[4]);
    }


    if (chegada_invalida(voo)) {
        printf(ERROR_NO_SUCH_AIRPORT_ID, voo.chegada);
    }
    /* Verificar se o sistema não excede o número máximo de voos */
    else if (nr_voos == MAX_VOOS) {
        printf(ERROR_TOO_MANY_FLIGHTS);
    }
    else if (data_invalida(voo.data)) {
        printf(ERROR_INVALID_DATE);
    }
    /* Verificar se a duração é válida */
    else if (duracao_hor * 60 + duracao_min > 12 * 60) {
        printf(ERROR_INVALID_DURATION);
    }
    /* Verificar se a capacidade é válida */
    else if (voo.capacidade < MIN_VOO_CAPACIDADE) {
        printf(ERROR_INVALID_CAPACITY);
    }
    else {
        return FALSE;
    }

    return TRUE;
}


/**
 * Recebe um código de voo.
 * Retorna 1 se o código for inválido.
 * Retorna 0 em caso contrário.
 */
int codigo_voo_invalido(char codigo[]) {
    int i;

    if (!MAIUSCULA(codigo[0]) || !MAIUSCULA(codigo[1])) {
        return TRUE;
    }
    else if (codigo[2] < '1' || codigo[2] > '9') {
        return TRUE;
    }
    else {
        for (i = 3; codigo[i] != '\0'; i++) {
            if (!NUMBER(codigo[i])) {
                return TRUE;
            }
        }
    }

    return FALSE;
}


/**
 * Recebe um voo.
 * Retorna 1 se o voo for duplicado.
 * Retorna 0 em caso contrário.
 */
int voo_duplicado(Voo voo) {
    node_voo *node_i = NULL;

    for (node_i = head_voos; node_i != NULL; node_i = node_i->next) {
        if ((!strcmp(node_i->voo.codigo, voo.codigo)) &&
                (!strcmp(node_i->voo.data, voo.data))) {
            return TRUE;
        }
    }

    return FALSE;
}


/**
 * Recebe um voo.
 * Retorna 1 se o aeroporto de chegada for inválido.
 * Retorna 0 em caso contrário.
 */
int chegada_invalida(Voo voo) {
    int i;

    for (i = 0; i < nr_aeroportos; i++) {
        if (!strcmp(voo.chegada, sistema_aeroportos[i].id)) {
            return FALSE;
        }
    }

    return TRUE;
}


/**
 * Recebe um vetor de voos (lista_voos).
 * Ordena os voos do vetor por data e hora (mais antiga para a mais recente).
 */
void sort_by_date(Voo lista_voos[], int dim_lista) {
    Bool sorted;
    int i;

    /* Verificar se o vetor tem mais que um voo */
    sorted = (dim_lista >= 2 ? FALSE : TRUE);

    while (!sorted) {
        sorted = TRUE;
        for (i = 0; i < dim_lista - 1; i++) {
            if (data_posterior(lista_voos[i].data, lista_voos[i + 1].data)) {
                SWAP_VOOS(lista_voos[i], lista_voos[i + 1]);
                sorted = FALSE;
            }
            else if (!strcmp(lista_voos[i].data, lista_voos[i + 1].data) &&
                    hora_posterior(lista_voos[i].hora, lista_voos[i + 1].hora)){
                SWAP_VOOS(lista_voos[i], lista_voos[i + 1]);
                sorted = FALSE;
            }
        }
    }
}


/* Funções relativas às datas */

/**
 * Executa o comando 't'.
 * Avança a data do sistema, verificando a existência de erros.
 */
void avanca_data() {
    char input_data[MAX_DATA];

    scanf("%s", input_data);

    if (data_invalida(input_data)) {
        printf(ERROR_INVALID_DATE);
    }
    else {
        strcpy(sistema_data, input_data);
        printf("%s\n", sistema_data);
    }
}


/**
 * Recebe uma data.
 * Retorna 1 se a data for inválida.
 * Retorna 0 em caso contrário.
 */
int data_invalida(char data[]) {
    /* Data atual do sistema no ano seguinte (incremento do ano) */
    char data_futuro[MAX_DATA];
    strcpy(data_futuro, sistema_data);
    data_futuro[MAX_DATA - 2] = sistema_data[MAX_DATA - 2] + 1;

    if(data_posterior(sistema_data, data) || data_posterior(data, data_futuro)){
        return TRUE;
    }
    else {
        return FALSE;
    }
}


/**
 * Recebe três vetores de caracteres (data e hora de partida, e duração).
 * Calcula a data e hora de chegada de um voo.
 */
void data_hora_chegada(char data[], char hora[], char duracao[]) {
    int horas = 0, minutos = 0, horas_duracao = 0, minutos_duracao = 0;

    /* Processamento das horas (para inteiros) */
    horas = ctoi(hora[0]) * 10 + ctoi(hora[1]);
    minutos = ctoi(hora[3]) * 10 + ctoi(hora[4]);

    /* Converter a duração para horas e minutos */
    if (duracao[1] == ':') {
        /* Suporte para hora mal formatada */
        horas_duracao = ctoi(duracao[0]);
        minutos_duracao = ctoi(duracao[2]) * 10 + ctoi(duracao[3]);
    }
    else {
        horas_duracao = ctoi(duracao[0]) * 10 + ctoi(duracao[1]);
        minutos_duracao = ctoi(duracao[3]) * 10 + ctoi(duracao[4]);  
    }

    horas += horas_duracao;
    minutos += minutos_duracao;

    if (minutos > 59) {
        minutos -= 60;
        horas++;
    }

    if (horas > 23) {
        horas -= 24;
        /* Incrementar o dia na data de chegada */
        dia_seguinte(data);
    }

    /* Processamento das horas (para vetor de caracteres) */
    hora[0] = itoc(horas / 10);
    hora[1] = itoc(horas % 10);
    hora[3] = itoc(minutos / 10);
    hora[4] = itoc(minutos % 10);
}


/**
 * Recebe dois vetores de caracteres, data1 e data2.
 * Retorna 1 se a data1 for posterior à data2.
 * Retorna 0 em caso contrário.
 */
int data_posterior(char data1[], char data2[]) {
    int i;
    int data1_ano = 0, data1_mes = 0, data1_dia = 0;
    int data2_ano = 0, data2_mes = 0, data2_dia = 0;

    /* Processamento das datas (para inteiros) */
    for (i = 0; i < MAX_DATA - 1; i++) {
        if (i < 2) {
            data1_dia = data1_dia * 10 + ctoi(data1[i]);
            data2_dia = data2_dia * 10 + ctoi(data2[i]);
        }
        else if (i > 2 && i < 5) {
            data1_mes = data1_mes * 10 + ctoi(data1[i]);
            data2_mes = data2_mes * 10 + ctoi(data2[i]);
        }
        else if (i > 5) {
            data1_ano = data1_ano * 10 + ctoi(data1[i]);
            data2_ano = data2_ano * 10 + ctoi(data2[i]);
        }
    }

    if (data1_ano > data2_ano) {
        return TRUE;
    }
    else if (data1_ano == data2_ano) {
        if (data1_mes > data2_mes) {
            return TRUE;
        }
        else if (data1_mes == data2_mes && data1_dia > data2_dia) {
            return TRUE;
        }
    }

    return FALSE;
}


/**
 * Recebe dois vetores de caracteres, hora1 e hora2.
 * Retorna 1 se a hora1 for posterior à hora2.
 * Retorna 0 em caso contrário.
 */
int hora_posterior(char hora1[], char hora2[]) {
    int i;
    int horas1 = 0, horas2 = 0, minutos1 = 0, minutos2 = 0;

    /* Processamento das horas (para inteiros) */
    for (i = 0; i < MAX_HORA - 1; i++) {
        if (i < 2) {
            horas1 = horas1 * 10 + ctoi(hora1[i]);
            horas2 = horas2 * 10 + ctoi(hora2[i]);
        }
        else if (i > 2) {
            minutos1 = minutos1 * 10 + ctoi(hora1[i]);
            minutos2 = minutos2 * 10 + ctoi(hora2[i]);
        }
    }

    if (horas1 > horas2) {
        return TRUE;
    }
    else if (horas1 == horas2 && minutos1 > minutos2) {
        return TRUE;
    }

    return FALSE;
}


/**
 * Recebe um vetor de caracteres (data).
 * Calcula a data do dia seguinte.
 */
void dia_seguinte(char data[]) {
    int i;
    int dia = 0, mes = 0, ano = 0;
    /* Número de dias do mês indice + 1 */
    int dias_mes[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    /* Processamento da data (para inteiros) */
    for (i = 0; i < MAX_DATA - 1; i++) {
        if (i < 2) {
            dia = dia * 10 + ctoi(data[i]);
        }
        else if (i > 2 && i < 5) {
            mes = mes * 10 + ctoi(data[i]);
        }
        else if (i > 5) {
            ano = ano * 10 + ctoi(data[i]);
        }
    }

    dia++;

    if (dia > dias_mes[mes - 1]) {
        dia = 1;
        mes++;

        if (mes > 12) {
            mes = 1;
            ano++;
        }
    }

    /* Processamento da data (para vetor de caracteres) */
    if (dia >= 10) {
        data[0] = itoc(dia / 10);
        data[1] = itoc(dia % 10);
    }
    else {
        data[0] = '0';
        data[1] = itoc(dia);
    }

    if (mes >= 10) {
        data[3] = itoc(mes / 10);
        data[4] = itoc(mes % 10);
    }
    else {
        data[3] = '0';
        data[4] = itoc(mes);
    }

    for (i = 9; i > 5; i--) {
        data[i] = itoc(ano % 10);
        ano /= 10;
    }
}


/* Funções relativas às reservas */

/**
 * Executa o comando 'r'.
 * Adiciona uma reserva ou lista as reserva de um voo, verificando a existência
 * de erros.
 */
void comando_r() {
    char input_codigo_voo[MAX_VOO_CODIGO];
    char input_data[MAX_DATA];
    char c;
    node_reserva *node_r = NULL;
    node_voo *node_v = NULL;
    Bool voo_invalido = TRUE;

    scanf("%s%s", input_codigo_voo, input_data);

    if ((c = getchar()) != '\n') {
        adiciona_reserva(input_codigo_voo, input_data);
    }
    else {
        /* Verificar se existe um voo com o código na data indicada */
        for (node_v = head_voos; node_v != NULL; node_v = node_v->next) {
            if (!strcmp(node_v->voo.codigo, input_codigo_voo) &&
                    !strcmp(node_v->voo.data, input_data)) {
                voo_invalido = FALSE;
                break;
            }
        }

        if (voo_invalido) {
            printf(ERROR_FLIGHT_DOES_NOT_EXIST, input_codigo_voo);
            return;
        }
        else if (data_invalida(input_data)) {
            printf(ERROR_INVALID_DATE);
            return;
        }

        for (node_r = head_reservas; node_r != NULL; node_r = node_r->next) {
            if (!strcmp(node_r->reserva.voo->codigo, input_codigo_voo) &&
                    !strcmp(node_r->reserva.data, input_data)) {
                printf("%s %d\n", node_r->reserva.codigo_reserva,
                                    node_r->reserva.passageiros);
            }
        }
    }
}


/**
 * Função complementar a comando_r().
 * Adiciona uma reserva ao sistema, verificando a existência de erros.
 */
void adiciona_reserva(char codigo_voo[], char data[]) {
    node_reserva *novo_node = NULL, *node_r = NULL, *prev_r = head_reservas;
    Reserva nova_reserva;
    char codigo_r[MAX_INPUT];
    int passageiros;
    Bool changed = FALSE;

    scanf("%s%d", codigo_r, &passageiros);

    if(reserva_invalida(codigo_voo, data, codigo_r, &nova_reserva,passageiros)){
        return;
    }

    /* Alocar e preencher os campos da nova_reserva */
    nova_reserva.codigo_reserva = (char *) safe_malloc(sizeof(char) *
        (strlen(codigo_r) + 1));
    strcpy(nova_reserva.codigo_reserva, codigo_r);
    strcpy(nova_reserva.data, data);
    nova_reserva.passageiros = passageiros;
    nova_reserva.voo->passageiros += nova_reserva.passageiros;

    /* Alocar e preencher os campos do novo_node */
    novo_node = (node_reserva *) safe_malloc(sizeof(node_reserva));
    novo_node->reserva = nova_reserva;
    novo_node->next = NULL;

    /* Adicionar a reserva ao sistema (ordem lexicográﬁca) */
    if (head_reservas == NULL) {
        head_reservas = novo_node;
        last_reservas = novo_node;
    }
    else if (strcmp(nova_reserva.codigo_reserva,
            head_reservas->reserva.codigo_reserva) < 0) {
        novo_node->next = head_reservas;
        head_reservas = novo_node;
    }
    else {
        for (node_r = head_reservas->next; node_r != NULL;
                prev_r = node_r, node_r = node_r->next) {
            if (strcmp(nova_reserva.codigo_reserva,
                    node_r->reserva.codigo_reserva) < 0) {
                novo_node->next = node_r;
                prev_r->next = novo_node;
                changed = TRUE;
                break;
            }
        }

        if (!changed) {
            prev_r->next = novo_node;
            last_reservas = novo_node;
        }
    }
}


/**
 * Recebe uma reserva.
 * Retorna 1 se a reserva for inválida.
 * Retorna 0 em caso contrário.
 */
int reserva_invalida(char cod_v[], char data[], char cod_r[], Reserva *r,int p){
    node_reserva *node_r = NULL;
    node_voo *node_v = NULL;
    Bool voo_invalido = TRUE, duplicado = FALSE, excede_capacidade = FALSE;

    /* Verificar se existe um voo com o código na data indicada */
    for (node_v = head_voos; node_v != NULL; node_v = node_v->next) {
        if(!strcmp(node_v->voo.codigo,cod_v) && !strcmp(node_v->voo.data,data)){
            voo_invalido = FALSE;
            r->voo = &(node_v->voo);

            if (node_v->voo.passageiros + p > node_v->voo.capacidade){
                excede_capacidade = TRUE;
            }

            break;
        }
    }

    /* Verificar se código da reserva é duplicado */
    for (node_r = head_reservas; node_r != NULL; node_r = node_r->next) {
        if (!strcmp(node_r->reserva.codigo_reserva, cod_r)) {
            duplicado = TRUE;
            break;
        }
    }

    if (codigo_reserva_invalido(cod_r)) {
        printf(ERROR_INVALID_RESERVATION_CODE);
    }
    else if (voo_invalido) {
        printf(ERROR_FLIGHT_DOES_NOT_EXIST, cod_v);
    }
    else if (duplicado) {
        printf(ERROR_RESERVATION_ALREADY_USED, cod_r);
    }
    else if (excede_capacidade) {
        printf(ERROR_TOO_MANY_RESERVATIONS);
    }
    else if (data_invalida(data)) {
        printf(ERROR_INVALID_DATE);
    }
    /* Verificar se o número de passageiros é válido */
    else if (p < MIN_PASSAGEIROS) {
        printf(ERROR_INVALID_PASSAGER_NUMBER);
    }
    else {
        return FALSE;
    }

    return TRUE;
}


/**
 * Recebe um código de reserva.
 * Retorna 1 se o código for inválido.
 * Retorna 0 em caso contrário.
 */
int codigo_reserva_invalido(char codigo_reserva[]) {
    int i;
    int cont = 0;

    for (i = 0; codigo_reserva[i] != '\0'; i++) {
        if (!(MAIUSCULA(codigo_reserva[i]) || NUMBER(codigo_reserva[i]))) {
            return TRUE;
        }

        cont++;
    }

    if (cont < MIN_RESERVA_CODIGO) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}


/* Funções de eliminação */

/**
 * Executa o comando 'e'.
 * Elimina voos ou uma reserva, verificando a existência de erros.
 */
void comando_e() {
    char input_codigo[MAX_INPUT];
    Bool encontrado = FALSE;

    scanf("%s", input_codigo);

    /* Verificar se o código corresponde a um voo ou a uma reserva */
    if (verifica_codigo_voo(input_codigo)) {
        elimina_voos(input_codigo, &encontrado);
    }
    else {
        elimina_reserva(input_codigo, &encontrado);
    }

    if (!encontrado) {
        printf(ERROR_NOT_FOUND);
    }
}


/**
 * Função complementar a comando_e().
 * Elimina os voos do sistema com o codigo_voo.
 */
void elimina_voos(char codigo_voo[], Bool *encontrado) {
    node_voo *node_v = NULL, *prev_v = NULL, *first_v = head_voos;
    node_voo *head_eliminados = NULL; /* Linked list de voos a eliminar */
    Bool changed_v = TRUE;

    /* Remover os voos com o código indicado da linked list */
    while (changed_v) {
        changed_v = FALSE;
        for (node_v = first_v; node_v != NULL;
                prev_v = node_v, node_v = node_v->next) {
            if (!strcmp(codigo_voo, node_v->voo.codigo)) {
                if (node_v == head_voos) {
                    head_voos = node_v->next;
                    if (node_v == last_voos) {
                        last_voos = NULL;
                    }
                }
                else {
                    prev_v->next = node_v->next;
                    if (node_v == last_voos) {
                        last_voos = prev_v;
                    }
                }
                first_v = node_v->next;

                /* Adicionar à linked list de voos a eliminar */
                if (head_eliminados == NULL) {
                    head_eliminados = node_v;
                    head_eliminados->next = NULL;
                }
                else {
                    node_v->next = head_eliminados;
                    head_eliminados = node_v;
                }
                nr_voos--;
                *encontrado = TRUE;
                changed_v = TRUE;
                break;
            }
        }
    }

    if (*encontrado) {
        elimina_reservas_voo(codigo_voo);
    }

    /* Libertar a memória reservada para os voos eliminados */
    while (head_eliminados != NULL) {
        node_v = head_eliminados;
        head_eliminados = head_eliminados->next;
        free(node_v);
    }
}


/**
 * Função complementar a elimina_voos().
 * Elimina as reservas do sistema correspondentes ao codigo_voo.
 */
void elimina_reservas_voo(char codigo_voo[]) {
    node_reserva *node_r = NULL, *prev_r = NULL, *first_r = head_reservas;
    Bool changed_r = TRUE;

    while (changed_r) {
        changed_r = FALSE;
        for (node_r = first_r; node_r != NULL;
                prev_r = node_r, node_r = node_r->next) {
            if (!strcmp(codigo_voo, node_r->reserva.voo->codigo)) {
                if (node_r == head_reservas) {
                    head_reservas = node_r->next;
                    if (node_r == last_reservas) {
                        last_reservas = NULL;
                    }
                }
                else {
                    prev_r->next = node_r->next;
                    if (node_r == last_reservas) {
                        last_reservas = prev_r;
                    }
                }
                first_r = node_r->next;
                changed_r = TRUE;
                
                free(node_r->reserva.codigo_reserva);
                free(node_r);
                break;
            }
        }
    }
}


/**
 * Função complementar a comando_e().
 * Elimina a reserva do sistema correspondente ao codigo_reserva.
 */
void elimina_reserva(char codigo_reserva[], Bool *encontrado) {
    node_reserva *node_r = NULL, *prev_r = NULL;
    int str_cmp;

    /* Remover a reserva com o código indicado da linked list */
    if (head_reservas == NULL) {}
    else if(strcmp(codigo_reserva, last_reservas->reserva.codigo_reserva) > 0){}
    else {
        for (node_r = head_reservas; node_r != NULL;
                prev_r = node_r, node_r = node_r->next) {
            str_cmp = strcmp(codigo_reserva, node_r->reserva.codigo_reserva);
            if (str_cmp < 0) {
                break;
            }
            else if (!str_cmp) {
                if (node_r == head_reservas) {
                    head_reservas = node_r->next;
                    if (node_r == last_reservas) {
                        last_reservas = NULL;
                    }
                }
                else {
                    prev_r->next = node_r->next;
                    if (node_r == last_reservas) {
                        last_reservas = prev_r;
                    }
                }
                node_r->reserva.voo->passageiros -= node_r->reserva.passageiros;
                free(node_r->reserva.codigo_reserva);
                free(node_r);
                *encontrado = TRUE;
                break;
            }
        }
    }
}


/**
 * Função complementar a comando_e().
 * Retorna 1 se o código corresponder a um voo.
 * Retorna 0 em caso contrário (reserva).
 */
int verifica_codigo_voo(char codigo[]) {
    int i;

    for (i = 0; i < MAX_VOO_CODIGO; i++) {
        if (codigo[i] == '\0') {
            return 1;
        }
    }

    return 0;
}


/* Funções auxiliares */

/**
 * Aloca memória dinamicamente (size bytes).
 * Se a memória se esgotar, liberta a memória reservada e termina o programa.
 */
void* safe_malloc(int size) {
	void *ptr = malloc(size);

	if (ptr == NULL) {
        printf(ERROR_NO_MEMORY);

        /* Libertar a memória reservada para voos e reservas */
        free_voos();
        free_reservas();

        exit(EXIT_CODE_NO_MEMORY);
    }

	return ptr;
}

/**
 * Liberta a memória reservada para os voos.
 */
void free_voos() {
    node_voo *aux_v = NULL;

    while (head_voos != NULL) {
        aux_v = head_voos;
        head_voos = head_voos->next;
        free(aux_v);
    }
}


/**
 * Liberta a memória reservada para as reservas.
 */
void free_reservas() {
    node_reserva *aux_r = NULL;

    while (head_reservas != NULL) {
        aux_r = head_reservas;
        head_reservas = head_reservas->next;
        free(aux_r->reserva.codigo_reserva);
        free(aux_r);
    }
}


/**
 * Recebe um vetor de caracteres (string).
 * Retorna 1 se a string apenas tiver letras maiúsculas.
 * Retorna 0 em caso contrário.
 */
int str_upper(char string[]) {
    int i = 0;

    while (string[i] != '\0') {
        if (!(string[i] >= 'A' && string[i] <= 'Z')) {
            return FALSE;
        }
        i++;
    }

    return TRUE;
}
