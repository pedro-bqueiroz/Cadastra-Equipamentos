#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <sys/stat.h>

#include "cadastra_equipamentos.h"

int main(int argc, char *argv[]){
    setlocale(LC_ALL, "Portuguese");
    setlocale(LC_NUMERIC, "C"); // Evita a conversão de pontos decimais para vírgulas, facilitando a impressão de dados em arquivos CSV.
    system("cls || clear"); // Útil para limpar a tela em sistemas Windows e Unix, respectivamente.

    int n=0; // Variável que vai fazer a contagem de quantos equipamentos estão cadastrados, importante para se usar nos loops.
    int confirmacao;
    Equipamentos vetor[MAX_EQUIPAMENTOS];

    int nivelAcesso = login_senha();
    if (nivelAcesso == -1) {
        return 0;
    }

    fprintf(stdout, "\nBem vindo ao Cadastra-Equipamentos.\nCertifique-se de que fechou o programa quando for embora!!!\n");

    FILE *arq = fopen("Equipamentos.txt", "r");
    while (!arq) {
        fprintf(stderr, "ATENÇÃO! Não há arquivo .txt de equipamentos a ser aberto. Deseja criar um? S/N ");
        scanf("\n%c", &confirmacao);
        if (confirmacao == 'n' || confirmacao == 'N') {
        fprintf(stdout, "Saindo do Cadastra-Equipamentos...");
        return 0;
        } else if (confirmacao == 's' || confirmacao == 'S') {
        arq = fopen("Equipamentos.txt", "w");
        fclose(arq);
        } else {
        fprintf(stderr, "Opção inválida.\n");
        }
    }
    
    fprintf(stdout, "\nNível de acesso: %d\n", nivelAcesso);

    fprintf(stdout, "Selecione uma das alternativas abaixo:");
    char input;

    do {
        fprintf(stdout, "\n\n1) Adicionar equipamento\n");
        fprintf(stdout, "2) Editar equipamento\n");
        fprintf(stdout, "3) Remover equipamento\n");
        fprintf(stdout, "4) Listar equipamentos\n");
        fprintf(stdout, "5) Buscar por fabricante\n");
        fprintf(stdout, "6) Listar equipamentos por uso\n");
        fprintf(stdout, "7) Salvar dados em arquivo\n");
        fprintf(stdout, "8) Carregar dados do arquivo\n");
        fprintf(stdout, "9) Configurações de login e senha\n");
        fprintf(stdout, "0) Listar usuários\n");
        fprintf(stdout, "Q) Verificar alterações\n");
        fprintf(stdout, "W) Importar ou exportar arquivos em outros formatos\n");
        fprintf(stdout, "C) Sair do programa\n\n");

        scanf(" %c", &input);
        switch (input){ 
            case '1':
                adicionar_equipamento(vetor, &n, nivelAcesso);
                break;

            case '2':
                editar_equipamento(vetor, &n, nivelAcesso);
                break;

            case '3':
                remover_equipamento(vetor, &n, nivelAcesso);
                break;

            case '4':
                listar_equipamentos(vetor, &n, nivelAcesso);
                break;
                
            case '5':
                buscar_por_fabricante(vetor, &n, nivelAcesso);
                break;

            case '6':
                listar_equipamentos_por_uso(vetor, &n, nivelAcesso);
                break;

            case '7':
                salvar_dados_em_arquivo(vetor, &n, nivelAcesso);
                break;

            case '8':
                carregar_dados_do_arquivo(vetor, &n, nivelAcesso);
                break;

            case '9':
                configurar_login_senha(nivelAcesso);
                break;

            case '0':
                listar_usuarios(nivelAcesso);
                break;

            case 'q':
            case 'Q':
                verificar_alteracoes(vetor, &n, nivelAcesso);
                break;

            case 'w':
            case 'W':
                abrir_outros_formatos(vetor, &n, nivelAcesso);
                break;

            case 'c':
            case 'C':
                fprintf(stdout, "Saindo do programa...");
                break;

            default:
                fprintf(stderr, "Alternativa inválida.\n");
                break;
        }
    } while (input != 'c' && input != 'C');

    return 0;
}
