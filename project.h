/**
 * File: project.h
 * Author: Nuno Gonçalves
 * Description: Declarações e macros utilizadas em project.c
 * Date: 2022-04-20
 */

/* Constantes relativas aos aeroportos */
#define MAX_AEROPORTOS 40
#define MAX_AEROPORTO_ID 4
#define MAX_AEROPORTO_PAIS 31
#define MAX_AEROPORTO_CIDADE 51

/* Constantes relativas aos voos */
#define MAX_VOOS 30000
#define MAX_VOO_CODIGO 7
#define MIN_VOO_CAPACIDADE 10

/* Constantes relativas às reservas */
#define MAX_INPUT 65536
#define MIN_RESERVA_CODIGO 10
#define MIN_PASSAGEIROS 1

/* Número máximo de caracteres relativos à data e hora */
#define MAX_DATA 11
#define MAX_HORA 6

/* Mensagens de erro */
#define ERROR_INVALID_AIRPORT_ID "invalid airport ID\n"
#define ERROR_TOO_MANY_AIRPORTS "too many airports\n"
#define ERROR_DUPLICATE_AIRPORT "duplicate airport\n"
#define ERROR_NO_SUCH_AIRPORT_ID "%s: no such airport ID\n"
#define ERROR_INVALID_FLIGHT_CODE "invalid flight code\n"
#define ERROR_FLIGHT_ALREADY_EXISTS "flight already exists\n"
#define ERROR_TOO_MANY_FLIGHTS "too many flights\n"
#define ERROR_INVALID_DATE "invalid date\n"
#define ERROR_INVALID_DURATION "invalid duration\n"
#define ERROR_INVALID_CAPACITY "invalid capacity\n"
#define ERROR_INVALID_RESERVATION_CODE "invalid reservation code\n"
#define ERROR_FLIGHT_DOES_NOT_EXIST "%s: flight does not exist\n"
#define ERROR_RESERVATION_ALREADY_USED "%s: flight reservation already used\n"
#define ERROR_TOO_MANY_RESERVATIONS "too many reservations\n"
#define ERROR_INVALID_PASSAGER_NUMBER "invalid passenger number\n"
#define ERROR_NOT_FOUND "not found\n"

#define ERROR_NO_MEMORY "No memory\n"
#define EXIT_CODE_NO_MEMORY -1

/**
 * Funções SWAP
 * Recebem duas variáveis (Aeroporto ou Voo) e trocam o valor das mesmas.
 */
#define SWAP_AEROPORTOS(a, b) { Aeroporto tmp; tmp = a; a = b; b = tmp; }
#define SWAP_VOOS(a, b) { Voo tmp; tmp = a; a = b; b = tmp; }

/**
 * Funções ctoi(c) e itoc(i)
 * Convertem um caractere (algarismo) para o respetivo inteiro (ctoi)
 * ou o contrário (itoc).
 */
#define ctoi(c) (c - '0')
#define itoc(i) (i + '0')

/**
 * Funções NUMBER(c) e MAIUSCULA(c)
 * Retornam 1 se o caractere c for um número ou uma maiúscula, respetivamente.
 */
#define NUMBER(c) (c >= '0' && c <= '9')
#define MAIUSCULA(c) (c >= 'A' && c <= 'Z')


/* Estruturas Aeroporto, Voo, Reserva, node_voo e node_reserva */
typedef struct {
    char id[MAX_AEROPORTO_ID];
    char pais[MAX_AEROPORTO_PAIS];
    char cidade[MAX_AEROPORTO_CIDADE];
    int voos;
} Aeroporto;

typedef struct {
    char codigo[MAX_VOO_CODIGO];
    char partida[MAX_AEROPORTO_ID];
    char chegada[MAX_AEROPORTO_ID];
    char data[MAX_DATA];
    char hora[MAX_HORA];
    char duracao[MAX_HORA];
    int capacidade;
    int passageiros;
} Voo;

typedef struct {
    Voo *voo;
    char data[MAX_DATA];
    char *codigo_reserva;
    int passageiros;
} Reserva;

typedef struct node_voo {
    Voo voo;
    struct node_voo *next;
} node_voo;

typedef struct node_reserva {
    Reserva reserva;
    struct node_reserva *next;
} node_reserva;

/* Valores booleanos */
typedef enum { FALSE = 0, TRUE = 1 } Bool;


/* Protótipos de funções relativas aos aeroportos */
void adiciona_aeroporto();
void lista_aeroportos();
int aeroporto_invalido(Aeroporto aeroporto);
void sort_by_id();

/* Protótipos de funções relativas aos voos */
void comando_v();
void adiciona_voo();
void lista_voos_partida();
void lista_voos_chegada();
int voo_invalido(Voo voo);
int codigo_voo_invalido(char codigo[]);
int voo_duplicado(Voo voo);
int chegada_invalida(Voo voo);
void sort_by_date(Voo lista_voos[], int dim_lista);

/* Protótipos de funções relativas às datas */
void avanca_data();
int data_invalida(char data[]);
void data_hora_chegada(char data[], char hora[], char duracao[]);
int data_posterior(char data1[], char data2[]);
int hora_posterior(char hora1[], char hora2[]);
void dia_seguinte(char data[]);

/* Protótipos de funções relativas às reservas */
void comando_r();
void adiciona_reserva(char codigo_voo[], char data[]);
int reserva_invalida(char cod_v[], char data[], char cod_r[], Reserva *r,int p);
int codigo_reserva_invalido(char codigo_reserva[]);

/* Protótipos de funções de eliminação */
void comando_e();
void elimina_voos(char codigo_voo[], Bool *encontrado);
void elimina_reservas_voo(char codigo_voo[]);
void elimina_reserva(char codigo_reserva[], Bool *encontrado);
int verifica_codigo_voo(char codigo[]);

/* Protótipos de funções auxiliares */
void* safe_malloc(int size);
void free_voos();
void free_reservas();
int str_upper(char string[]);


/* Variáveis globais */

/* Armazena os aeroportos do sistema */
static Aeroporto sistema_aeroportos[MAX_AEROPORTOS];
/* Armazena o número de aeroportos do sistema */
int nr_aeroportos = 0;

/* Armazena os voos do sistema (linked list) */
static node_voo *head_voos = NULL;
static node_voo *last_voos = NULL;
/* Armazena o número de voos do sistema */
int nr_voos = 0;

/* Armazena as reservas do sistema (linked list) */
static node_reserva *head_reservas = NULL;
static node_reserva *last_reservas = NULL;

/* Data atual do sistema */
char sistema_data[] = "01-01-2022";
