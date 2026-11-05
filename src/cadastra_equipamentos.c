#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cadastra_equipamentos.h"

int login_senha(){ // Estabelece restrições de acesso a funções do programa
    int nivel = 0;
    char login[MAX_STRING], senha[MAX_STRING], loginarq[MAX_STRING], senhaarq[MAX_STRING];

    FILE *arq2 = fopen("loginSenha.txt", "r"); // Tentativa de abrir o arquivo
    
    if (!arq2) fprintf(stderr, "Não há arquivo de login e senha. Deseja criar um? S/N\n");

    while (!arq2) {
        char confirmacao;
        scanf(" %c", &confirmacao);
        while (getchar() != '\n');

        if (confirmacao == 'n' || confirmacao == 'N') {
            fprintf(stdout, "Saindo do Cadastra-Equipamentos...\n");
            return -1;
        } else if (confirmacao == 's' || confirmacao == 'S') {
            arq2 = fopen("loginSenha.txt", "w");
            if (!arq2) {
                fprintf(stderr, "Erro ao criar arquivo loginSenha.txt.\n");
                return -1;
            }
            fprintf(stdout, "Arquivo criado com sucesso.\n");
            break;
        } else {
            fprintf(stderr, "Opção inválida.\n");
        }
    }

    fclose(arq2);

    fprintf(stdout, "Em suspeita de alteração não permitida, peça assistência a um administrador.\n");
    fprintf(stdout, "Insira seu login e senha. Digite 'c' em ambos para sair.\n");

    while (1) {
        fprintf(stdout, "\nLogin:\n");
        if (fgets(login, sizeof(login), stdin) == NULL) return -1;
        login[strcspn(login, "\r\n")] = 0;

        if (strcmp(login, LOGINSENHAMESTRES) == 0) {
            fprintf(stdout, "Bem vindo, administrador.\n");
            return 4;
        }

        fprintf(stdout, "\nSenha:\n");
        if (fgets(senha, sizeof(senha), stdin) == NULL) return -1;
        senha[strcspn(senha, "\r\n")] = 0;

        if (strcmp(senha, LOGINSENHAMESTRES) == 0) {
            fprintf(stdout, "Bem vindo, administrador.\n");
            return 4;
        }

        if ((strcmp(login, "c") == 0 || strcmp(login, "C") == 0) &&
            (strcmp(senha, "c") == 0 || strcmp(senha, "C") == 0)) {
            fprintf(stdout, "Saindo do Cadastra-Equipamentos...\n");
            return -1;
        }

        arq2 = fopen("loginSenha.txt", "r");
        if (!arq2) {
            fprintf(stderr, "Erro ao abrir loginSenha.txt.\n");
            return -1;
        }

        int autenticado = 0;
        while (fscanf(arq2, " %30[^,],%30[^,],%d", loginarq, senhaarq, &nivel) == 3) {
            if (strcmp(login, loginarq) == 0 && strcmp(senha, senhaarq) == 0) {
                fprintf(stdout, "\nLogin: %s\tNível de acesso atual: %d\n", loginarq, nivel);
                autenticado = 1;
                break;
            }
        }
        fclose(arq2);

        if (autenticado) {
            return nivel;
        }

        fprintf(stderr, "Login ou senha incorretos. Tente novamente.\n");
    }
}

void configurar_login_senha(int nivelAcesso){ // Oferece opções internas de configuração de usuários (a primeira entrada é feita usando LOGINSENHAMESTRES)
    if (nivelAcesso < 3) {
        fprintf(stderr, "Acesso negado. Nível de acesso insuficiente.\n");
        return;
    }

    int nivel = 0, i = 0;
    char login[MAX_STRING], senha[MAX_STRING], loginarq[MAX_STRING], senhaarq[MAX_STRING];
    LoginSenha *usuarios = (LoginSenha *) malloc(MAX_USUARIOS * sizeof(LoginSenha));
    if (!usuarios) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        return;
    }

    fprintf(stdout, "Para ter acesso a essa função, digite seu login e senha novamente.\n");
    fprintf(stdout, "Em suspeita de alteração não permitida, peça assistência a um administrador.\n");
    fprintf(stdout, "Insira seu login e senha. Digite 'c' em ambos para sair.\n");
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    int autenticado = 0;
    while (!autenticado) {
        fprintf(stdout, "\nLogin:\n");
        if (fgets(login, sizeof(login), stdin) == NULL) {
            free(usuarios);
            return;
        }
        login[strcspn(login, "\r\n")] = 0;
        
        fprintf(stdout, "\nSenha:\n");
        if (fgets(senha, sizeof(senha), stdin) == NULL) {
            free(usuarios);
            return;
        }
        senha[strcspn(senha, "\r\n")] = 0;

        if ((strcmp(login, "c") == 0 || strcmp(login, "C") == 0) &&
            (strcmp(senha, "c") == 0 || strcmp(senha, "C") == 0)) {
            free(usuarios);
            return;
        }

        if (strcmp(login, LOGINSENHAMESTRES) == 0 || strcmp(senha, LOGINSENHAMESTRES) == 0) {
            autenticado = 1;
            break;
        }

        FILE *arq2 = fopen("loginSenha.txt", "r");
        while (fscanf(arq2, " %30[^,],%30[^,],%d", loginarq, senhaarq, &nivel) == 3) {
            if (strcmp(login, loginarq) == 0 && strcmp(senha, senhaarq) == 0) {
                fprintf(stdout, "\nLogin: %s\tNível de acesso: %d\n", loginarq, nivel);
                autenticado = 1;
                break;
            }
        }
        fclose(arq2);


        if (!autenticado) {
            fprintf(stderr, "Login ou senha incorretos. Tente novamente.\n");
        }
    }

    // Carrega todos os usuários cadastrados no arquivo para o vetor em memória
    int total_usuarios = 0;
    FILE *arq2 = fopen("loginSenha.txt", "r");
    while (total_usuarios < MAX_USUARIOS &&
            fscanf(arq2, " %30[^,],%30[^,],%d",
                    usuarios[total_usuarios].login,
                    usuarios[total_usuarios].senha,
                    &usuarios[total_usuarios].nivelAcesso) == 3) {
        total_usuarios++;
    }
    fclose(arq2);

    fprintf(stdout, "Você deseja adicionar, editar ou remover um usuário? Digite 'a' para adicionar, 'e' para editar, 'r' para remover ou 'c' para cancelar.\n");
    char confirmacao;
    scanf(" %c", &confirmacao);
    while ((c = getchar()) != '\n' && c != EOF);

    if (confirmacao == 'a' || confirmacao == 'A') {
        if (total_usuarios >= MAX_USUARIOS) {
            fprintf(stderr, "Limite máximo de usuários atingido.\n");
            free(usuarios);
            return;
        }

        fprintf(stdout, "Digite o login do novo usuário:\n");
        fgets(login, sizeof(login), stdin);
        login[strcspn(login, "\r\n")] = 0;

        fprintf(stdout, "Digite a senha do novo usuário:\n");
        fgets(senha, sizeof(senha), stdin);
        senha[strcspn(senha, "\r\n")] = 0;

        nivel = 0;
        fprintf(stdout, "Digite o nível de acesso do novo usuário (entre 1 e 3):\n");
        while (nivel < 1 || nivel > 3) {
            scanf(" %d", &nivel);
            if (nivel < 1 || nivel > 3) fprintf(stderr, "Nível de acesso inválido. Tente novamente.\n");
        }

        arq2 = fopen("loginSenha.txt", "a");

        fprintf(arq2, "%s,%s,%d\n", login, senha, nivel);
        fclose(arq2);
        fprintf(stdout, "Usuário adicionado com sucesso.\n");

    } else if (confirmacao == 'e' || confirmacao == 'E') {
        int tam = listar_usuarios(nivelAcesso);
        if (tam == 0) {
            free(usuarios);
            return;
        }

        fprintf(stdout, "Digite o login do usuário que deseja editar:\n");
        fgets(login, sizeof(login), stdin);
        login[strcspn(login, "\r\n")] = 0;

        int encontrado = 0;
        for (i = 0; i < tam; i++) {
            if (strcmp(login, usuarios[i].login) == 0) {
                encontrado = 1;
                fprintf(stdout, "Digite o novo login do usuário:\n");
                fgets(usuarios[i].login, sizeof(usuarios[i].login), stdin);
                usuarios[i].login[strcspn(usuarios[i].login, "\r\n")] = 0;

                fprintf(stdout, "Digite a nova senha do usuário:\n");
                fgets(usuarios[i].senha, sizeof(usuarios[i].senha), stdin);
                usuarios[i].senha[strcspn(usuarios[i].senha, "\r\n")] = 0;

                nivel = 0;
                fprintf(stdout, "Digite o novo nível de acesso do usuário (entre 1 e 3):\n");
                while (nivel < 1 || nivel > 3) {
                    scanf(" %d", &nivel);
                    if (nivel < 1 || nivel > 3) fprintf(stderr, "Nível de acesso inválido. Tente novamente.\n");
                }
                usuarios[i].nivelAcesso = nivel;

                arq2 = fopen("loginSenha.txt", "w");

                for (int k = 0; k < tam; k++) {
                    fprintf(arq2, "%s,%s,%d\n", usuarios[k].login, usuarios[k].senha, usuarios[k].nivelAcesso);
                }
                fclose(arq2);
                fprintf(stdout, "Usuário editado com sucesso.\n");

                break;
            }
        }
        if (!encontrado) {
            fprintf(stderr, "Usuário não encontrado.\n");
        }
    } else if (confirmacao == 'r' || confirmacao == 'R') {
        int tam = listar_usuarios(nivelAcesso);
        if (tam == 0) {
            free(usuarios);
            return;
        }

        fprintf(stdout, "Digite o login do usuário que deseja remover:\n");
        fgets(login, sizeof(login), stdin);
        login[strcspn(login, "\r\n")] = 0;
        
        int encontrado = 0;
        for (i = 0; i < tam; i++) {
            if (strcmp(login, usuarios[i].login) == 0) {
                encontrado = 1;
                // Substitui o usuário removido pelo último da lista e reduz o tamanho
                strcpy(usuarios[i].login, usuarios[tam - 1].login);
                strcpy(usuarios[i].senha, usuarios[tam - 1].senha);
                usuarios[i].nivelAcesso = usuarios[tam - 1].nivelAcesso;
                tam--;

                arq2 = fopen("loginSenha.txt", "w");

                for (int k = 0; k < tam; k++) {
                        fprintf(arq2, "%s,%s,%d\n", usuarios[k].login, usuarios[k].senha, usuarios[k].nivelAcesso);
                    }
                    fclose(arq2);
                    fprintf(stdout, "Usuário removido com sucesso.\n");
                
                break;
            }
        }
        if (!encontrado) {
            fprintf(stderr, "Usuário não encontrado.\n");
        }
    } else if (confirmacao != 'c' && confirmacao != 'C') {
        fprintf(stderr, "Opção inválida. Tente novamente.\n");
    }

    free(usuarios);
    return;
}

int modificar_equipamento(Equipamentos vetor[], int index, int nivelAcesso){
    Equipamentos *cadastro = (Equipamentos *) malloc (sizeof(Equipamentos));
    char confirmacao;
    int ano, utilizacao, codigo;
    float valor, tensao, corrente;
    float Potencia;
    char tipo[MAX_STRING], nome[MAX_STRING], fabricante[MAX_STRING], serie[MAX_STRING];

    fprintf(stdout, "Código do equipamento, entre 0 e 99999: ");
    scanf(" %d", &codigo);
    if (codigo < 0 || codigo > 99999) {
        fprintf(stderr, "Código inválido!\n");
        free(cadastro);
        return -1;
    }
    getchar();

    fprintf(stdout, "Nome do equipamento: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\r\n")] = 0;

    fprintf(stdout, "Fabricante: ");
    fgets(fabricante, sizeof(fabricante), stdin);
    fabricante[strcspn(fabricante, "\r\n")] = 0;

    fprintf(stdout, "Ano de fabricação (entre -9999 e 9999): ");
    scanf(" %d", &ano);
    if (ano < -9999 || ano > 9999) {
        fprintf(stderr, "Ano inválido!\n");
        free(cadastro);
        return -1;
    }
    getchar();

    fprintf(stdout, "Utilização (digite 0 caso desocupado ou 1 caso ocupado): ");
    scanf(" %d", &utilizacao);
    if (utilizacao != 0 && utilizacao != 1) {
        fprintf(stderr, "Valor inválido!\n");
        free(cadastro);
        return -1;
    }
    while(getchar() != '\n');

    fprintf(stdout, "Valor de mercado (em Reais, com ponto): ");
    scanf(" %f", &valor);
    while(getchar() != '\n');

    fprintf(stdout, "Número de série: ");
    fgets(serie, sizeof(serie), stdin);
    serie[strcspn(serie, "\r\n")] = 0;
    
    fprintf(stdout, "Tipo (exemplo: multímetro): ");
    fgets(tipo, sizeof(tipo), stdin);
    tipo[strcspn(tipo, "\r\n")] = 0;

    fprintf(stdout, "Tensão (em Volts): "); 
    scanf(" %f", &tensao);

    fprintf(stdout, "Corrente (em Amperes): ");
    scanf(" %f", &corrente);

    fprintf(stdout, "Potência (em Watts): ");
    scanf(" %f", &Potencia);

    fprintf(stdout, "\nConfirma? S/N\n");

    scanf("  %c", &confirmacao);

    if (confirmacao == 's' || confirmacao == 'S') {
        cadastro->codigo = codigo;
        cadastro->ano = ano;
        cadastro->t_specs.correnteMaxima = corrente;
        cadastro->t_specs.Potencia = Potencia;
        strcpy(cadastro->nome, nome);
        strcpy(cadastro->t_specs.tipo, tipo);
        cadastro->utilizacao = utilizacao;
        strcpy(cadastro->fabricante, fabricante);
        strcpy(cadastro->serie, serie);
        cadastro->valor = valor;
        cadastro->t_specs.tensaoNominal = tensao;
        vetor[index] = *cadastro; // Define a posição index do vetor como o equipamento salvo.
    } else if (confirmacao == 'n' || confirmacao == 'N') {
        free(cadastro);
        return -1;
    } else {
        fprintf(stderr, "Opção inválida. Tente novamente.\n");
        free(cadastro);
        return -1;
    }

    free(cadastro);
    return 0;
}

void adicionar_equipamento(Equipamentos vetor[], int *n, int nivelAcesso){
    if (nivelAcesso < 2) {
        fprintf(stderr, "Acesso negado. Nível de acesso insuficiente.\n");
        return;
    }

    if (*n >= MAX_EQUIPAMENTOS) {
        fprintf(stderr, "Limite máximo de equipamentos (%d) atingido.\n", MAX_EQUIPAMENTOS);
        return;
    }

    fprintf(stdout, "\nPara adicionar o equipamento, informe:\n");

    int j = modificar_equipamento(vetor, *n, nivelAcesso);
    if (j == -1) return;
    
    (*n)++; // Soma um ao n, indicando que outro equipamento foi adicionado ao sistema.
    fprintf(stdout, "\nVocê adicionou o equipamento %d.\n", *n);

    return;
}

void editar_equipamento(Equipamentos vetor[], int *n, int nivelAcesso){
    if (nivelAcesso < 2) {
        fprintf(stderr, "Acesso negado. Nível de acesso insuficiente.\n");
        return;
    }

    if (*n == 0) {
        fprintf(stdout, "\nSem equipamentos cadastrados.\n");
        return;
    }

    int numero;
    char confirmacao;
    
    listar_equipamentos(vetor, n, nivelAcesso);
    
    fprintf(stdout, "Você escolheu a função para editar equipamentos. Tem certeza que quer fazer isso? S/N ");
    scanf(" %c", &confirmacao);
    switch (confirmacao){
        case 's':
        case 'S':
            break;

        case 'n':
        case 'N':
            return;

        default: 
            fprintf(stderr, "Opção inválida.\n");
            return;
    }

    fprintf(stdout, "\nQual o número do equipamento que você quer editar?\n");
    scanf(" %i", &numero);
    if (numero < 1 || numero > *n) {
        fprintf(stderr, "Número de equipamento inválido.\n");
        return;
    }
    numero--; // Atualiza o índice do equipamento no vetor

    fprintf(stdout, "\nPara editar o equipamento, informe:\n");

    if (modificar_equipamento(vetor, numero, nivelAcesso) == 0) {
        fprintf(stdout, "Você editou o equipamento %d.\n", numero + 1);
    }
    return;
}

void recursao(Equipamentos vetor[], int num, int n){ // Função recursiva usada em remover_equipamento().
    if (num >= n - 1) return;
    vetor[num] = vetor[num+1];
    recursao(vetor, num+1, n); // Desloca, para a esquerda, os valores a partir do index escolhido, assim, alterando a disposição dos elementos.
}

void remover_equipamento(Equipamentos vetor[], int *n, int nivelAcesso){
    if (nivelAcesso < 2) {
        fprintf(stderr, "Acesso negado. Nível de acesso insuficiente.\n");
        return;
    }

    int numero;
    char confirma;

    listar_equipamentos(vetor, n, nivelAcesso);
    
    if (*n > 0) {
        fprintf(stdout, "\nQual dos equipamentos você quer excluir?\n");
        scanf(" %i", &numero); // Define o equipamento a ser excluído
        if (numero < 1 || numero > *n) {
            fprintf(stderr, "Número de equipamento inválido.\n");
            return;
        }
        fprintf(stdout, "\nTem certeza de que quer excluir o equipamento %d? Digite 's' caso positivo, 'n' caso negativo. ", numero);
        scanf(" %c", &confirma);

        if (confirma == 'n' || confirma == 'N') return;

        if (confirma == 's' || confirma == 'S') {
            numero--; // Ajusta o valor para o index à esquerda no vetor
            recursao(vetor, numero, *n);
            (*n)--; // Elemento excluído.
            fprintf(stdout, "\nEquipamento %d excluído.\n", numero + 1);
        } else {
            fprintf(stderr, "Opção inválida.\n");
        }
    } else fprintf(stdout, "\nNão há equipamentos cadastrados.\n");

    return;
}

int contar_equipamentos(Equipamentos vetor[]){
    int i = 0;

    FILE *arqconta = fopen("Equipamentos.txt", "r");
    if (!arqconta) return 0;
    while (i < MAX_EQUIPAMENTOS &&
           fscanf(arqconta, " %d,%30[^,],%30[^,],%d,%d,%f,%30[^,],%30[^,],%f,%f,%f",
                  &vetor[i].codigo, vetor[i].nome, vetor[i].fabricante, &vetor[i].ano, &vetor[i].utilizacao, &vetor[i].valor,
                  vetor[i].serie, vetor[i].t_specs.tipo, &vetor[i].t_specs.tensaoNominal, &vetor[i].t_specs.correnteMaxima, &vetor[i].t_specs.Potencia) == 11) {
        i++;
    }

    fclose(arqconta);
    return i;
}

void buscar_por_fabricante(Equipamentos vetor[], int *n, int nivelAcesso){
    if (nivelAcesso < 1) {
        fprintf(stderr, "Acesso negado. Nível de acesso insuficiente.\n");
        return;
    }
    
    int i=0;
    char fab[MAX_STRING];
    
    if (*n == 0) {
        fprintf(stdout, "\nSem equipamentos cadastrados.\n");
        return;
    }

    getchar();
    fprintf(stdout, "\nDigite o fabricante: ");
    fgets(fab, sizeof(fab), stdin);
    fab[strcspn(fab, "\r\n")] = 0;

    for (int j = 0; j < *n; j++) {
        if (strcmp(fab, vetor[j].fabricante) == 0) {
            lista(vetor, j);
            i++;
        }
    }
    
    if (i == 0) {
        fprintf(stdout, "\nNenhum produto encontrado.");
        return;
    }

    fprintf(stdout, "\n%d produtos de %d produtos achado(s).\n", i,*n);
}

void listar_equipamentos_por_uso(Equipamentos vetor[], int *n, int nivelAcesso) {
    if (nivelAcesso < 1) {
        fprintf(stderr, "Acesso negado. Nível de acesso insuficiente.\n");
        return;
    }

    int j = 0; // j é controle, caso seja 0 não há elementos encontrados
    int i, input;

    if (*n == 0) {
        fprintf(stdout, "\nSem equipamentos cadastrados\n");
        return;
    }

    fprintf(stdout, "\nDeseja listar quais equipamentos?\n0) Disponíveis\n1) Ocupados\n");
    scanf(" %d", &input);

    if (input == 0) {
        for (i = 0; i < *n; i++) {
            if (vetor[i].utilizacao == 0) {
                if (j == 0) fprintf(stdout, "\n- Produto(s) disponíveis: -\n");
                lista(vetor, i);
                j++;
            }
        }
        if (j == 0) fprintf(stdout, "\nNenhum produto disponível.\n");
    } else if (input == 1) {
        for (i = 0; i < *n; i++) {
            if (vetor[i].utilizacao == 1) {
                if (j == 0) fprintf(stdout, "\n- Produto(s) em uso: -\n");
                lista(vetor, i);
                j++;
            }
        }
        if (j == 0) fprintf(stdout, "\nNenhum produto em uso.\n");
    } else {
        fprintf(stderr, "Opção inválida.\n");
    }

    return;
}

void salvar_dados_em_arquivo(Equipamentos vetor[], int *n, int nivelAcesso){
    if (nivelAcesso < 2) {
        fprintf(stderr, "Acesso negado. Nível de acesso insuficiente.\n");
        return;
    }

    char *nome = "Equipamentos.txt";
    FILE *arq = fopen(nome, "w");

    if (!arq) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return;
    }

    // Salva todos os equipamentos do vetor no arquivo em questão
    for (int i = 0; i < *n; i++) { 
        fprintf(arq, "%d,%s,%s,%d,%d,%.2f,%s,%s,%.2f,%.2f,%.2f\n", vetor[i].codigo, vetor[i].nome, vetor[i].fabricante, vetor[i].ano, vetor[i].utilizacao, 
            vetor[i].valor, vetor[i].serie, vetor[i].t_specs.tipo, vetor[i].t_specs.tensaoNominal, vetor[i].t_specs.correnteMaxima, vetor[i].t_specs.Potencia);
    }

    fclose(arq);
    fprintf(stdout, "\nProdutos salvos em um arquivo .txt no diretório do programa.\n");
}

void carregar_dados_do_arquivo(Equipamentos vetor[], int *n, int nivelAcesso){
    if (nivelAcesso < 1) {
        fprintf(stderr, "Acesso negado. Nível de acesso insuficiente.\n");
        return;
    }

    char *nome = "Equipamentos.txt";
    FILE *arq = fopen(nome, "r");

    if (!arq) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return;
    } else {
        int i = contar_equipamentos(vetor);
        for (int j = 0; j < i; j++) lista(vetor, j);
        *n = i; //define o número de equipamentos presentes no sistema
    }
    
    fclose(arq);
    return;
}

void abrir_outros_formatos(Equipamentos vetor[], int *n, int nivelAcesso){
    if (nivelAcesso < 2) {
        fprintf(stderr, "Acesso negado. Nível de acesso insuficiente.\n");
        return;
    }
    
    fprintf(stdout, "\nDeseja abrir/exportar arquivos em formatos diferentes? Digite '1' para abrir, '2' para exportar ou 'c' para cancelar.\n");
    char input;
    scanf(" %c", &input);
    switch(input){
        case '1': {
            fprintf(stdout, "\nDeseja abrir os dados de qual formato?\n1) CSV\n2) JSON\n3) XML\nc) Sair\n");
            char confirmacao;
            scanf(" %c", &confirmacao);
            switch (confirmacao){
                case '1': {
                    FILE *csv = fopen("Equipamentos.csv", "r");
                    if (!csv) {
                        fprintf(stderr, "Erro ao abrir o arquivo. Verifique se o arquivo existe e tente novamente.\n");
                        return;
                    }

                    int i = 0;
                    fscanf(csv, "%*[^\n]\n"); // Pula a linha de cabeçalho do CSV
                    while (i < MAX_EQUIPAMENTOS &&
                           fscanf(csv, "%d,%30[^,],%30[^,],%d,%d,%f,%30[^,],%30[^,],%f,%f,%f",
                                  &vetor[i].codigo, vetor[i].nome, vetor[i].fabricante, &vetor[i].ano, &vetor[i].utilizacao,
                                  &vetor[i].valor, vetor[i].serie, vetor[i].t_specs.tipo, &vetor[i].t_specs.tensaoNominal,
                                  &vetor[i].t_specs.correnteMaxima, &vetor[i].t_specs.Potencia) == 11) {
                        i++;
                    }

                    fclose(csv);
                    *n = i; // Define o número de equipamentos
                    fprintf(stdout, "\nDados importados de CSV.\n");
                    break; 
                }
                
                case '2': {
                    FILE *json = fopen("Equipamentos.json", "r");
                    if (!json) {
                        fprintf(stderr, "Erro ao abrir o arquivo. Verifique se o arquivo existe e tente novamente.\n");
                        return;
                    }

                    int i = 0;
                    fscanf(json, " [");
                    while (i < MAX_EQUIPAMENTOS &&
                           fscanf(json,
                                  " { \"codigo\": %d ,"
                                  " \"nome\": \"%30[^\"]\" ,"
                                  " \"fabricante\": \"%30[^\"]\" ,"
                                  " \"ano\": %d ,"
                                  " \"utilizacao\": %d ,"
                                  " \"valor\": %f ,"
                                  " \"serie\": \"%30[^\"]\" ,"
                                  " \"tipo\": \"%30[^\"]\" ,"
                                  " \"tensaoNominal\": %f ,"
                                  " \"correnteMaxima\": %f ,"
                                  " \"potencia\": %f }",
                                  &vetor[i].codigo,
                                  vetor[i].nome,
                                  vetor[i].fabricante,
                                  &vetor[i].ano,
                                  &vetor[i].utilizacao,
                                  &vetor[i].valor,
                                  vetor[i].serie,
                                  vetor[i].t_specs.tipo,
                                  &vetor[i].t_specs.tensaoNominal,
                                  &vetor[i].t_specs.correnteMaxima,
                                  &vetor[i].t_specs.Potencia) == 11) {
                        i++;
                        fscanf(json, " ,");
                    }

                    fclose(json);
                    *n = i; // Define o número de equipamentos
                    fprintf(stdout, "\nDados importados de JSON.\n");
                    break;
                }

                case '3': {
                    FILE *xml = fopen("Equipamentos.xml", "r");
                    if (!xml) {
                        fprintf(stderr, "Erro ao abrir o arquivo. Verifique se o arquivo existe e tente novamente.\n");
                        return;
                    }

                    int i = 0;
                    fscanf(xml, " <Equipamentos>");
                    while (i < MAX_EQUIPAMENTOS &&
                           fscanf(xml,
                                  " <Equipamento>"
                                  " <Codigo>%d</Codigo>"
                                  " <Nome>%30[^<]</Nome>"
                                  " <Fabricante>%30[^<]</Fabricante>"
                                  " <Ano>%d</Ano>"
                                  " <Utilizacao>%d</Utilizacao>"
                                  " <Valor>%f</Valor>"
                                  " <Serie>%30[^<]</Serie>"
                                  " <Tipo>%30[^<]</Tipo>"
                                  " <TensaoNominal>%f</TensaoNominal>"
                                  " <CorrenteMaxima>%f</CorrenteMaxima>"
                                  " <Potencia>%f</Potencia>"
                                  " </Equipamento>",
                                  &vetor[i].codigo,
                                  vetor[i].nome,
                                  vetor[i].fabricante,
                                  &vetor[i].ano,
                                  &vetor[i].utilizacao,
                                  &vetor[i].valor,
                                  vetor[i].serie,
                                  vetor[i].t_specs.tipo,
                                  &vetor[i].t_specs.tensaoNominal,
                                  &vetor[i].t_specs.correnteMaxima,
                                  &vetor[i].t_specs.Potencia) == 11) {
                        i++;
                    }

                    fclose(xml);
                    *n = i; // Define o número de equipamentos
                    fprintf(stdout, "\nDados importados de XML.\n");
                    break;
                }
                
                case 'c':
                case 'C':
                    return;

                default:
                    fprintf(stderr, "Opção inválida. Tente novamente.\n");
                    break;
            }

            break;
        }

        case '2': {
            char confirmacao2;

            fprintf(stdout, "\nDeseja exportar os dados para qual formato?\n1) CSV\n2) JSON\n3) XML\nc) Sair\n");
            scanf(" %c", &confirmacao2);
            switch (confirmacao2){
                case '1': {
                    FILE *csv = fopen("Equipamentos.csv", "w");
                    if (!csv) {
                        fprintf(stderr, "Erro ao criar o arquivo CSV.\n");
                        return;
                    }
                    fprintf(csv, "Codigo,Nome,Fabricante,Ano,Utilizacao,Valor,Serie,Tipo,TensaoNominal,CorrenteMaxima,Potencia\n");
                    for (int i = 0; i < *n; i++) { 
                        fprintf(csv, "%d,%s,%s,%d,%d,%f,%s,%s,%f,%f,%f\n", vetor[i].codigo, vetor[i].nome, vetor[i].fabricante, vetor[i].ano, vetor[i].utilizacao, 
                        vetor[i].valor, vetor[i].serie, vetor[i].t_specs.tipo, vetor[i].t_specs.tensaoNominal, vetor[i].t_specs.correnteMaxima, vetor[i].t_specs.Potencia);
                    }

                    fclose(csv);
                    fprintf(stdout, "\nDados exportados para CSV.\n");
                    break;
                }

                case '2': {
                    FILE *json = fopen("Equipamentos.json", "w");
                    if (!json) {
                        fprintf(stderr, "Erro ao criar o arquivo JSON.\n");
                        return;
                    }
                    fprintf(json, "[\n");
                    for (int i = 0; i < *n; i++) {
                        fprintf(json, "  {\n");
                        fprintf(json, "    \"codigo\": %d,\n", vetor[i].codigo);
                        fprintf(json, "    \"nome\": \"%s\",\n", vetor[i].nome);
                        fprintf(json, "    \"fabricante\": \"%s\",\n", vetor[i].fabricante);
                        fprintf(json, "    \"ano\": %d,\n", vetor[i].ano);
                        fprintf(json, "    \"utilizacao\": %d,\n", vetor[i].utilizacao);
                        fprintf(json, "    \"valor\": %.2f,\n", vetor[i].valor);
                        fprintf(json, "    \"serie\": \"%s\",\n", vetor[i].serie);
                        fprintf(json, "    \"tipo\": \"%s\",\n", vetor[i].t_specs.tipo);
                        fprintf(json, "    \"tensaoNominal\": %.2f,\n", vetor[i].t_specs.tensaoNominal);
                        fprintf(json, "    \"correnteMaxima\": %.2f,\n", vetor[i].t_specs.correnteMaxima);
                        fprintf(json, "    \"potencia\": %.2f\n", vetor[i].t_specs.Potencia);
                        if (i < *n - 1) {
                            fprintf(json, "  },\n");
                        } else {
                            fprintf(json, "  }\n");
                        }
                    }
                    fprintf(json, "]\n");
                    fclose(json);
                    fprintf(stdout, "\nDados exportados para JSON.\n");
                    break;
                }

                case '3': {
                    FILE *xml = fopen("Equipamentos.xml", "w");
                    if (!xml) {
                        fprintf(stderr, "Erro ao criar o arquivo XML.\n");
                        return;
                    }
                    fprintf(xml, "<Equipamentos>\n");
                    for (int i = 0; i < *n; i++) {
                        fprintf(xml, "  <Equipamento>\n");
                        fprintf(xml, "    <Codigo>%d</Codigo>\n", vetor[i].codigo);
                        fprintf(xml, "    <Nome>%s</Nome>\n", vetor[i].nome);
                        fprintf(xml, "    <Fabricante>%s</Fabricante>\n", vetor[i].fabricante);
                        fprintf(xml, "    <Ano>%d</Ano>\n", vetor[i].ano);
                        fprintf(xml, "    <Utilizacao>%d</Utilizacao>\n", vetor[i].utilizacao);
                        fprintf(xml, "    <Valor>%.2f</Valor>\n", vetor[i].valor);
                        fprintf(xml, "    <Serie>%s</Serie>\n", vetor[i].serie);
                        fprintf(xml, "    <Tipo>%s</Tipo>\n", vetor[i].t_specs.tipo);
                        fprintf(xml, "    <TensaoNominal>%.2f</TensaoNominal>\n", vetor[i].t_specs.tensaoNominal);
                        fprintf(xml, "    <CorrenteMaxima>%.2f</CorrenteMaxima>\n", vetor[i].t_specs.correnteMaxima);
                        fprintf(xml, "    <Potencia>%.2f</Potencia>\n", vetor[i].t_specs.Potencia);
                        fprintf(xml, "  </Equipamento>\n");
                    }
                    fprintf(xml, "</Equipamentos>\n");
                    fclose(xml);
                    fprintf(stdout, "\nDados exportados para XML.\n");
                    break;
                }
                
                case 'C':
                case 'c':
                    return;

                default:
                    fprintf(stderr, "Opção inválida. Tente novamente.\n");
                    break;
            }
            break;
        }

        case 'C':
        case 'c':
            return;

        default:
            fprintf(stderr, "Opção inválida. Tente novamente.\n");
    }
    
    return;
}

void verificar_alteracoes(Equipamentos vetor[], int *n, int nivelAcesso){ 
    if (nivelAcesso < 3) {
        fprintf(stderr, "Acesso negado. Nível de acesso insuficiente.\n");
        return;
    }

    // COMPARAR BACKUP COM ARQUIVO ATUAL, DAR OPÇÕES PARA VERIFICAR ARQUIVO CSV OU OUTROS
    /*fprintf(stdout, "Deseja verificar alterações a partir de que data? (formato: dd/mm/aaaa)\n"); 

    IMPLEMENTAR <SYS/STAT.H>
    COMPARAR E EXIBIR METADADOS, ESPECIALMENTE DATAS DE ALTERAÇÃO, DOS TXT/BIN E O BACKUP SELECIONADO (BACKUP AUTOMÁTICO?); CASO DIFERENTES, COMPARAR E LISTAR DIFERENÇAS ENTRE 
    EQUIPAMENTOS E USUÁRIOS, TOMANDO CUIDADO PARA NÃO GERAR FALHAS DE SEGURANÇA OU DE AUTENTICAÇÃO, POIS O USUÁRIO PODE TER ACESSO A 
    INFORMAÇÕES SENSÍVEIS, COMO EQUIPAMENTOS, LOGIN, SENHA E OUTROS ARQUIVOS, QUE PODEM ESTAR PRESENTES EM BACKUPS ANTIGOS.
    */

    fprintf(stdout, "\nFuncionalidade em desenvolvimento. Peça assistência a um administrador.\n");
    return;
}

/*
void verificarSegurançaSenha(char senha[])
    alguma parte que usa hash com salt
    return;
*/
