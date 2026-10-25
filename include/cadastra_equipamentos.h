#pragma once
#define MAX_STRING 31
#define MAX_EQUIPAMENTOS 256
#define MAX_USUARIOS 32
#define LOGINSENHAMESTRES "ICE CREAM"

typedef struct {
    char login[MAX_STRING];
    char senha[MAX_STRING];
    int nivelAcesso; 
} LoginSenha;

typedef struct {
    char tipo[MAX_STRING]; // Categoria do equipamento. Exemplos: "Fonte", "Multímetro"
    float tensaoNominal; // Tensão nominal do equipamento, em Volts
    float correnteMaxima; // Corrente de curto-circuito do aparelho, em Amperes
    float Potencia; // Consumo estimado de energia do aparelho, em Watts
} EspecificacoesTecnicas;

typedef struct {
    int ano, utilizacao, quantidade, codigo;
    char serie[MAX_STRING]; // Número de série
    float valor; // Valor de mercado (ajustado para inflação no ano corrente...)
    char nome[MAX_STRING], fabricante[MAX_STRING];
    EspecificacoesTecnicas t_specs;
} Equipamentos;

int login_senha();
void configurar_login_senha(int nivelAcesso);
int modificar_equipamento(Equipamentos vetor[], int index, int nivelAcesso);
void adicionar_equipamento(Equipamentos vetor[], int *n, int nivelAcesso);
void editar_equipamento(Equipamentos vetor[], int *n, int nivelAcesso);
void recursao(Equipamentos vetor[], int index, int n);
void remover_equipamento(Equipamentos vetor[], int *n, int nivelAcesso);
int lista(Equipamentos vetor[], int j);
int contar_equipamentos(Equipamentos vetor[]);
void listar_equipamentos(Equipamentos vetor[], int *n, int nivelAcesso);
void buscar_por_fabricante(Equipamentos vetor[], int *n, int nivelAcesso);
void listar_equipamentos_por_uso(Equipamentos vetor[], int *n, int nivelAcesso);
void salvar_dados_em_arquivo(Equipamentos vetor[], int *n, int nivelAcesso);
void carregar_dados_do_arquivo(Equipamentos vetor[], int *n, int nivelAcesso);
void abrir_outros_formatos(Equipamentos vetor[], int *n, int nivelAcesso);
void verificar_alteracoes(Equipamentos vetor[], int *n, int nivelAcesso);
void listar_usuarios(int nivelAcesso);
