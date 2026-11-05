#include <stdio.h>

#include "cadastra_equipamentos.h"


int listar_usuarios(int nivelAcesso){
    int niv = 0, j = 0;
    char login[MAX_STRING], senha[MAX_STRING];

    if (nivelAcesso < 1) {
        fprintf(stderr, "\nAcesso negado. Nível de acesso insuficiente.\n");
        return 0;
    }

    FILE *arq2 = fopen("loginSenha.txt", "r");

    if (!arq2) {
        fprintf(stderr, "\nErro ao abrir o arquivo. Peça assistência a um administrador.\n");
        return 0;
    }

    while (fscanf(arq2, " %30[^,],%30[^,],%d", login, senha, &niv) == 3) {
        if (nivelAcesso < niv) {
            fprintf(stdout, "\nLogin: %s\tSenha: *\t Nível: *\n", login);
        } else {
            fprintf(stdout, "\nLogin: %s\tSenha: %s\t Nível: %d\n", login, senha, niv);
        }
        j++;
    }

    if (j == 0) {
        fprintf(stdout, "\nNenhum usuário cadastrado.\n");
    }

    fclose(arq2);
    return j;
}

int lista(Equipamentos vetor[], int j){
        fprintf(stdout, "\n-------- Equipamento %d --------\n\n", j+1);
        fprintf(stdout, "\tCodigo: %d\n", vetor[j].codigo);
        fprintf(stdout, "\tNome: %s\n", vetor[j].nome);
        fprintf(stdout, "\tFabricante: %s\n", vetor[j].fabricante);
        fprintf(stdout, "\tAno: %d\n", vetor[j].ano);
        fprintf(stdout, "\tUtilização: %d\n", vetor[j].utilizacao);
        fprintf(stdout, "\tValor: %.2f\n", vetor[j].valor);
        fprintf(stdout, "\tNumero de serie: %s\n", vetor[j].serie);
        fprintf(stdout, "\tTipo: %s\n", vetor[j].t_specs.tipo);
        fprintf(stdout, "\tVoltagem de operacao: %.2f V\n", vetor[j].t_specs.tensaoNominal);
        fprintf(stdout, "\tCorrente maxima: %.2f A\n", vetor[j].t_specs.correnteMaxima);
        fprintf(stdout, "\tConsumo: %.2f W\n", vetor[j].t_specs.Potencia);

    return j;
}

void listar_equipamentos(Equipamentos vetor[], int *n, int nivelAcesso){
    if (nivelAcesso < 1) {
        fprintf(stderr, "Acesso negado. Nível de acesso insuficiente.\n");
        return;
    }

    if (*n == 0) {
        fprintf(stdout, "\nSem equipamentos cadastrados.\n");
        return;
    }

    fprintf(stdout, "\n-Lista de equipamentos:-\n");

    for (int i = 0; i < *n; i++) lista(vetor, i);
}
