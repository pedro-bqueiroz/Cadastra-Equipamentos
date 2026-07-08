#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <sys/stat.h>

#include "cadastra_equipamentos.h"

int login_senha(){ // Estabelece restrições de acesso a funções do programa
    int nivel = 0;
    char login[MAX_STRING], senha[MAX_STRING], loginarq[MAX_STRING], senhaarq[MAX_STRING];

    FILE *arq2 = fopen("loginSenha.txt", "r"); // Tentativa de abrir o arquivo
    
    if (!arq2) fprintf(stderr, "Não há arquivo de login e senha. Deseja criar um?\n");

    while (!arq2) {
        char confirmacao;
        scanf(" \n%c", &confirmacao);

        if (confirmacao == 'n' || confirmacao == 'N') {
            fprintf(stdout, "Saindo do Cadastra-Equipamentos...");
            return -1;
        } else if (confirmacao == 's' || confirmacao == 'S') {
            arq2 = fopen("loginSenha.txt", "w");
            fprintf(stdout, "Arquivo criado com sucesso.\n");
            break;
        } else {
            fprintf(stderr, "Opção inválida.\n");
        }
    }

    fclose(arq2);
    arq2 = fopen("loginSenha.txt", "r"); // O arquivo é reaberto para evitar erros, caso tenha sido criado na parte anterior 

    fprintf(stdout, "Em suspeita de alteração não permitida, peça assistência a um administrador.\n");
    fprintf(stdout, "Insira seu login e senha. Digite 'c' em ambos para sair.\n");

    fprintf(stdout, "\nLogin:\n");
    fgets(login, sizeof(login), stdin);
    
    login[strcspn(login, "\r\n")] = 0;
    if (strcmp(login, LOGINSENHAMESTRES) == 0) {
        fprintf(stdout, "Bem vindo, administrador.\n");
        fclose(arq2);
        return 4;
    }

    fprintf(stdout, "\nSenha:\n");
    fgets(senha, sizeof(senha), stdin);

    senha[strcspn(senha, "\r\n")] = 0;
    if (strcmp(senha, LOGINSENHAMESTRES) == 0) {
        fprintf(stdout, "Bem vindo, administrador. \n");
        fclose(arq2);
        return 4;
    }

    while (fscanf(arq2, "%s %s %d", loginarq, senhaarq, &nivel) == 3) {
       if (strcmp(login, loginarq) == 0 && strcmp(senha, senhaarq) == 0) {
            fprintf(stdout, "\nLogin: %s\tNível de acesso atual: %d\n", nivel);
            fclose(arq2);
            return nivel;
        } else if (login == "c" || login == "C") {
            nivel = 0;
            break;
        } else fprintf(stderr, "Login ou senha incorretos. Tente novamente.\n");
    }

    fclose(arq2);
    return 0;
}

void configurar_login_senha(int nivelAcesso){ // Oferece opções internas de configuração de usuários (a primeira entrada é feita usando LOGINSENHAMESTRES)
    if (nivelAcesso < 3) {
        fprintf(stderr, "Acesso negado. Nível de acesso insuficiente.\n");
        return;
    }

    int nivel = 0, i = 0;
    char login[MAX_STRING], senha[MAX_STRING], loginarq[MAX_STRING], senhaarq[MAX_STRING];
    LoginSenha *usuarios = (LoginSenha *) malloc(sizeof(LoginSenha *));

    fprintf(stdout, "Para ter acesso a essa função, digite seu login e senha novamente.\n");

    FILE *arq2 = fopen("loginSenha.txt", "r");

    fprintf(stdout, "Em suspeita de alteração não permitida, peça assistência a um administrador.\n");
    fprintf(stdout, "Insira seu login e senha. Digite 'c' em ambos para sair.");
    
    fprintf(stdout, "\n\nLogin:\n");
    getchar();
    fgets(login, sizeof(login), stdin);
    login[strcspn(login, "\r\n")] = 0;
    
    fprintf(stdout, "\nSenha:\n");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\r\n")] = 0;
    
    while (fscanf(arq2, "%s %s %d", loginarq, senhaarq, &nivel) == 3) {
       if (strcmp(login, loginarq) == 0 && strcmp(senha, senhaarq) == 0) {
            fprintf(stdout, "\nLogin: %s\tNível de acesso: %d\n", nivel);
            fclose(arq2);
        } else if (login == "c" || login == "C") {
            fclose(arq2);
            return;
        } else if (login == LOGINSENHAMESTRES || senha == LOGINSENHAMESTRES) {
            break;
        } else fprintf(stderr, "Login ou senha incorretos. Tente novamente.\n");
    }

    fprintf(stdout, "Você deseja adicionar, editar ou remover um usuário? Digite 'a' para adicionar, 'e' para editar, 'r' para remover ou 'c' para cancelar.\n");
    char confirmacao;
    scanf(" %c", &confirmacao);
    getchar();

    while (confirmacao != 'c' && confirmacao != 'C') {
        if (confirmacao == 'a' || confirmacao == 'A') {
            fprintf(stdout, "Digite o login do novo usuário:\n");
            fgets(login, sizeof(login), stdin);
            login[strcspn(login, "\r\n")] = 0;

            fprintf(stdout, "Digite a senha do novo usuário:\n");
            fgets(senha, sizeof(senha), stdin);
            senha[strcspn(senha, "\r\n")] = 0;

            fprintf(stdout, "Digite o nível de acesso do novo usuário (entre 1 e 3):\n");
            scanf(" %d", &nivel);

            arq2 = fopen("loginSenha.txt", "a");
            fprintf(arq2, "%s %s %d\n", login, senha, nivel);
            fclose(arq2);
            return;
        } else if (confirmacao == 'e' || confirmacao == 'E') {
            listar_usuarios(nivelAcesso);
            fprintf(stdout, "Digite o login do usuário que deseja editar:\n");
            fgets(login, sizeof(login), stdin);
            login[strcspn(login, "\r\n")] = 0;

            for (i = 0; i < MAX_USUARIOS; i++) {
                if (strcmp(login, usuarios[i].login) == 0) {
                    fprintf(stdout, "Digite o novo login do usuário:\n");
                    fgets(login, sizeof(login), stdin);
                    login[strcspn(login, "\r\n")] = 0;

                    fprintf(stdout, "Digite a nova senha do usuário:\n");
                    fgets(senha, sizeof(senha), stdin);
                    senha[strcspn(senha, "\r\n")] = 0;

                    fprintf(stdout, "Digite o novo nível de acesso do usuário (entre 1 e 3):\n");
                    while (nivel < 1 || nivel > 3) {
                        scanf(" %d", &nivel);
                        if (nivel < 1 || nivel > 3) fprintf(stderr, "Nível de acesso inválido. Tente novamente.\n");
                    }

                    FILE *arq2 = fopen("loginSenha.txt", "w");
                    fprintf(arq2, "%s %s %d\n", usuarios[i].login, usuarios[i].senha, usuarios[i].nivelAcesso);
                    fclose(arq2);
                    return;
                }
            }
        } else if (confirmacao == 'r' || confirmacao == 'R') {
            fprintf(stdout, "Digite o login do usuário que deseja remover:\n");
            fgets(login, sizeof(login), stdin);
            login[strcspn(login, "\r\n")] = 0;
            
            for (i = 0; i < MAX_USUARIOS; i++) {
                if (strcmp(login, usuarios[i].login) == 0) {

                    fprintf(stdout, "Usuário removido com sucesso.\n");
                    return;
                }
            }
        } else fprintf(stderr, "Opção inválida. Tente novamente.\n");
    }

    return;
}

void listar_usuarios(int nivelAcesso){
    int niv = 0, j = 0;
    char login[MAX_STRING], senha[MAX_STRING];

    if (nivelAcesso < 1) {
        fprintf(stderr, "\nAcesso negado. Nível de acesso insuficiente.\n");
        return;
    }

    FILE *arq2 = fopen("loginSenha.txt", "r");

    if (!arq2) {
        fprintf(stderr, "\nErro ao abrir o arquivo. Peça assistência a um administrador.\n");
        return;
    }

    while (fscanf(arq2, "%s %s %d", login, senha, &niv) == 3) {
        if (nivelAcesso < niv) {
            fprintf(stdout, "\nLogin: %s\tSenha: %s\t Nível: *\t", login, senha);
            continue;
        } else {
            fprintf(stdout, "\nLogin: %s\tSenha: %s\t Nível: %d\t", login, senha, niv);
        }
        j++;
    }

    if (j == 0) {
        fprintf(stdout, "\nNenhum usuário cadastrado.\n");
    }

    fclose(arq2);
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
        vetor[index] = *cadastro; // Define a posição n do vetor(ambos definidos no main) como o equipamento salvo.
    } else if (confirmacao == 'n' || confirmacao == 'N') {
        free(cadastro);
        return -1;
    } else fprintf(stderr, "Opção inválida. Tente novamente.");

    free(cadastro);
    return 0;
}

void adicionar_equipamento(Equipamentos vetor[], int *n, int nivelAcesso){
     if (nivelAcesso < 2) {
        fprintf(stderr, "Acesso negado. Nível de acesso insuficiente.\n");
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

    int numero;
    char confirmacao;
    
    listar_equipamentos(vetor, n, nivelAcesso);
    
    fprintf(stdout, "Você escolheu a função para editar equipamentos. Tem certeza que quer fazer isso?");
    scanf(" %c", confirmacao);
    switch (confirmacao){
        case 's':
        case 'S':
            break;

        case 'n':
        case 'N':
            return;

        default: 
            fprintf(stdout, "Opção inválida.");
            break;
    }

    fprintf(stdout, "\nQual o número do equipamento que você quer editar?\n");
    
    scanf(" %i", &numero);
    numero--; // Atualiza o índice do equipamento no vetor

    fprintf(stdout, "\nPara editar o equipamento, informe:\n");

    modificar_equipamento(vetor, numero, nivelAcesso); 
    
    fprintf(stdout,"Você editou o equipamento %d.", numero+1);
    return;
}

void recursao(Equipamentos vetor[], int num, int n){ // Função recursiva usada em remover_equipamento().
    num >= n ? return : vetor[num] = vetor[num+1];
    recursao(vetor, num+1, n); //  Desloca, para a esquerda, os valores a partir do index escolhido, assim, alterando a disposição dos elementos.
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
        scanf(" %i", &numero); //Define o equipamento a ser excluído
        fprintf(stdout, "\nTem certeza de que quer excluir o equipamento %d? Digite 's' caso positivo, 'n' caso negativo. ", numero);
        scanf(" %c", &confirma);

        if (confirma == 'n' || confirma == 'N') return;

        if (confirma == 's' || confirma == 'S') {
        numero--; //Ajusta o valor para o index à esquerda no vetor
        recursao(vetor, numero, *n);
        *n--; //elemento excluído.
        fprintf(stdout, "\nEquipamento %d excluído", numero + 1);
        }
    } else fprintf(stdout, "\nNão há equipamentos cadastrados.\n");

    return;
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

int contar_equipamentos(Equipamentos vetor[]){
    int i = 0;

    FILE *arqconta = fopen("Equipamentos.txt", "r");
    while (fscanf(arqconta, "%d %s %s %d %d %f %s %s %f %f %f", &vetor[i].codigo,vetor[i].nome,vetor[i].fabricante,&vetor[i].ano,&vetor[i].utilizacao,&vetor[i].valor,
                vetor[i].serie,vetor[i].t_specs.tipo,&vetor[i].t_specs.tensaoNominal,&vetor[i].t_specs.correnteMaxima,&vetor[i].t_specs.Potencia) == 11) {
                i++;
    }

    fclose(arqconta);
    return i;
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

    int j = contar_equipamentos(vetor);

    fprintf(stdout, "\n-Lista de equipamentos:-\n");

    for (int i = 0; i < j; i++) lista(vetor, i);

    //caso o valor de n seja 0 significa que não há elementos no vetor, logo não há equipamentos cadastrados
    if (*n == 0) fprintf(stdout, "\nNenhum equipamento cadastrado\n");
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

    int j=0; //j é controle, caso seja 0 não há elementos em uso
    int i, input;

    if (*n == 0) {
        fprintf(stdout, "\nSem equipamentos cadastrados\n");
        return;
    }

    fprintf(stdout, "\nDeseja listar quais equipamentos?\n0) Disponíveis\n1) Ocupados\n");
    scanf(" %d", &input);

    if (input == 0) {
        for (i = 0; i < *n; i++) {
            if (vetor[i].utilizacao = 0) {
            fprintf(stdout, "\n- Produto(s) disponíveis: -\n");
            lista(vetor, i);
            }

        if (i == 0) fprintf(stdout, "\nNenhum produto disponível.\n");
        }
    } else if (input == 1) {
        for (i = 0; i < *n; i++) {
            if (vetor[i].utilizacao = 1) {
            fprintf(stdout, "\n- Produto(s) em uso: -\n");
            lista(vetor, j);
            }

        if (i == 0) fprintf(stdout, "\nNenhum produto em uso.\n");
        }
    } else fprintf(stderr, "Opção inválida");

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
        fprintf(stderr, "Erro ao abrir o arquivo");
    } else {
        // Lê o arquivo, caso exista, e salva todos os equipamentos do vetor no arquivo em questão,
        for (int i = 0; i < *n; i++) { 
            fprintf(arq, "%d %s %s %d %d %.2f %s %s %.2f %.2f %.2f\n", vetor[i].codigo, vetor[i].nome, vetor[i].fabricante, vetor[i].ano, vetor[i].utilizacao, 
                vetor[i].valor, vetor[i].serie, vetor[i].t_specs.tipo, vetor[i].t_specs.tensaoNominal, vetor[i].t_specs.correnteMaxima, vetor[i].t_specs.Potencia);
        }
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
        fprintf(stderr, "Erro ao abrir o arquivo");
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
        case '1':
            fprintf(stdout, "\nDeseja abrir os dados de qual formato?\n1) CSV\n2) JSON\n3) XML\nc) Sair\n");
            char confirmacao;
            scanf(" %c", &confirmacao);
            switch (confirmacao){
                case '1':
                    FILE *csv = fopen("Equipamentos.csv", "r");
                    if (!csv) {
                        fprintf(stderr, "Erro ao abrir o arquivo. Verifique se o arquivo existe e tente novamente.\n");
                        return;
                    }

                    int i = 0;
                    while (fscanf(csv, "%d,%[^,],%[^,],%d,%d,%f,%[^,],%[^,],%f,%f,%f", &vetor[i].codigo, vetor[i].nome, vetor[i].fabricante, &vetor[i].ano, &vetor[i].utilizacao,
                                &vetor[i].valor, vetor[i].serie, vetor[i].t_specs.tipo, &vetor[i].t_specs.tensaoNominal, &vetor[i].t_specs.correnteMaxima, &vetor[i].t_specs.Potencia) == 11) {
                        i++;
                    }

                    fclose(csv);
                    *n = i; // Define o número de equipamentos
                    fprintf(stdout, "\nDados importados de CSV.\n");
                    break; 
                
                case '2':
                    FILE *json = fopen("Equipamentos.json", "r");
                    if (!json) {
                        fprintf(stderr, "Erro ao abrir o arquivo. Verifique se o arquivo existe e tente novamente.\n");
                        return;
                    }

                    i = 0;
                    /* 
                    while (fscanf(json, "%d,%[^,],%[^,],%d,%d,%f,%[^,],%[^,],%f,%f,%f", &vetor[i].codigo, vetor[i].nome, vetor[i].fabricante, &vetor[i].ano, &vetor[i].utilizacao,
                                &vetor[i].valor, vetor[i].serie, vetor[i].t_specs.tipo, &vetor[i].t_specs.tensaoNominal, &vetor[i].t_specs.correnteMaxima, &vetor[i].t_specs.Potencia) == 11) {
                        i++;
                    }
                    */
                    fclose(json);
                    *n = i;
                    fprintf(stdout, "\nDados importados de JSON.\n");
                    break;

                case '3':
                    FILE *xml = fopen("Equipamentos.xml", "r");
                    if (!xml) {
                        fprintf(stderr, "Erro ao abrir o arquivo. Verifique se o arquivo existe e tente novamente.\n");
                        return;
                    }

                    i = 0;
                    /*
                    while (fscanf(xml, "%d,%[^,],%[^,],%d,%d,%f,%[^,],%[^,],%f,%f,%f", &vetor[i].codigo, vetor[i].nome, vetor[i].fabricante, &vetor[i].ano, &vetor[i].utilizacao,
                                &vetor[i].valor, vetor[i].serie, vetor[i].t_specs.tipo, &vetor[i].t_specs.tensaoNominal, &vetor[i].t_specs.correnteMaxima, &vetor[i].t_specs.Potencia) == 11) {
                        i++;
                    }
                    */
                    fclose(xml);
                    *n = i;
                    fprintf(stdout, "\nDados importados de XML.\n");
                    break;
                
                case 'c':
                case 'C':
                    return;

                default:
                    fprintf(stderr, "Opção inválida. Tente novamente.\n");
                    break;
            }

            break;

        case '2':
            FILE *txt = fopen("Equipamentos.txt", "r");
            char confirmacao2;

            fprintf(stdout, "\nDeseja exportar os dados para qual formato?\n1) CSV\n2) JSON\n3) XML\nc) Sair\n");
            scanf(" %c", &confirmacao2);
            switch (confirmacao2){
                case '1': 
                    FILE *txt = fopen("Equipamentos.txt", "r");
                    FILE *csv = fopen("Equipamentos.csv", "w");
                    fprintf(csv, "Codigo,Nome,Fabricante,Ano,Utilizacao,Valor,Serie,Tipo,TensaoNominal,CorrenteMaxima,Potencia\n");
                    for (int i = 0; i < *n; i++) { 
                        fprintf(csv, "%d,%s,%s,%d,%d,%f,%s,%s,%f,%f,%f\n", vetor[i].codigo, vetor[i].nome, vetor[i].fabricante, vetor[i].ano, vetor[i].utilizacao, 
                        vetor[i].valor, vetor[i].serie, vetor[i].t_specs.tipo, vetor[i].t_specs.tensaoNominal, vetor[i].t_specs.correnteMaxima, vetor[i].t_specs.Potencia);
                    }

                    fclose(txt); fclose (csv);
                    fprintf(stdout, "\nDados exportados para CSV.\n");
                    break;

                case '2':
                    FILE *json = fopen("Equipamentos.json", "w");
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
                    fclose(txt); fclose (json);
                    fprintf(stdout, "\nDados exportados para JSON.\n");
                    break;

                case '3':
                    FILE *xml = fopen("Equipamentos.xml", "w");
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
                    fclose(txt); fclose (xml);
                    fprintf(stdout, "\nDados exportados para XML.\n");
                    break;
                
                case 'C':
                case 'c':
                    return;

                default:
                    fprintf(stderr, "Opção inválida. Tente novamente.\n");
                    break;
                }
            break;

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
    EQUIPAMENTOS E USUÁRIOS, TOMANDO CUIDADO PARA NÃO GERAR FALHAS DE SEGURANÇA (BUFFER OVERFLOW/SQL INJECTION) OU DE AUTENTICAÇÃO, POIS O USUÁRIO PODE TER ACESSO A 
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
