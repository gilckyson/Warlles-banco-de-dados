#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(nome) _mkdir(nome)
#else
#include <sys/stat.h>
#define MKDIR(nome) mkdir(nome, 0777)
#endif

typedef struct {
    int id;
    char nome[50];
    char matricula[20];
    char turma[10];
    int ativo;
} Aluno;

void criarBanco(char nomeBanco[]) {

    char caminho[100];
    sprintf(caminho, "%s", nomeBanco);

    if (MKDIR(caminho) == 0) {
        printf("Banco de dados criado com sucesso!\n");
    } else {
        printf("Banco ja existe ou erro ao criar.\n");
    }

    char tabela[150];

    sprintf(tabela, "%s/alunos.txt", nomeBanco);

    FILE *f = fopen(tabela, "a");

    if (f != NULL) {
        fclose(f);
    }
}

void cadastrarAluno(char arquivo[]) {

    FILE *f;
    Aluno a;

    int ultimo = 0;

    f = fopen(arquivo, "r");

    if (f != NULL) {

        while (fscanf(f, "%d;%[^;];%[^;];%[^;];%d\n",
                      &a.id,
                      a.nome,
                      a.matricula,
                      a.turma,
                      &a.ativo) != EOF) {

            ultimo = a.id;
        }

        fclose(f);
    }

    f = fopen(arquivo, "a");

    a.id = ultimo + 1;

    printf("Nome do aluno: ");
    scanf(" %[^\n]", a.nome);

    printf("Turma: ");
    scanf("%s", a.turma);

    sprintf(a.matricula, "2026%d", a.id);

    a.ativo = 1;

    fprintf(f, "%d;%s;%s;%s;%d\n",
            a.id,
            a.nome,
            a.matricula,
            a.turma,
            a.ativo);

    fclose(f);

    printf("Aluno cadastrado!\n");
    printf("ID do aluno: %d\n", a.id);
}

void listarAlunos(char arquivo[]) {

    FILE *f;
    Aluno a;

    f = fopen(arquivo, "r");

    if (f == NULL) {
        printf("Nenhum aluno cadastrado.\n");
        return;
    }

    printf("\n===== LISTA DE ALUNOS =====\n");

    while (fscanf(f, "%d;%[^;];%[^;];%[^;];%d\n",
                  &a.id,
                  a.nome,
                  a.matricula,
                  a.turma,
                  &a.ativo) != EOF) {

        if (a.ativo) {

            printf("ID: %d\n", a.id);
            printf("Nome: %s\n", a.nome);
            printf("Matricula: %s\n", a.matricula);
            printf("Turma: %s\n", a.turma);
            printf("----------------------\n");
        }
    }

    fclose(f);
}

void buscarPorID(char arquivo[]) {

    FILE *f;
    Aluno a;

    int id;
    int encontrado = 0;

    printf("Digite o ID: ");
    scanf("%d", &id);

    f = fopen(arquivo, "r");

    if (f == NULL) {
        printf("Arquivo nao encontrado.\n");
        return;
    }

    while (fscanf(f, "%d;%[^;];%[^;];%[^;];%d\n",
                  &a.id,
                  a.nome,
                  a.matricula,
                  a.turma,
                  &a.ativo) != EOF) {

        if (a.id == id && a.ativo) {

            printf("\n===== ALUNO =====\n");

            printf("ID: %d\n", a.id);
            printf("Nome: %s\n", a.nome);
            printf("Matricula: %s\n", a.matricula);
            printf("Turma: %s\n", a.turma);

            encontrado = 1;
        }
    }

    if (!encontrado) {
        printf("Aluno nao encontrado.\n");
    }

    fclose(f);
}

void excluirAluno(char arquivo[]) {

    FILE *f, *temp;

    Aluno a;

    int id;
    int encontrado = 0;

    printf("Digite o ID para excluir: ");
    scanf("%d", &id);

    f = fopen(arquivo, "r");

    if (f == NULL) {
        printf("Arquivo nao encontrado.\n");
        return;
    }

    temp = fopen("temp.txt", "w");

    while (fscanf(f, "%d;%[^;];%[^;];%[^;];%d\n",
                  &a.id,
                  a.nome,
                  a.matricula,
                  a.turma,
                  &a.ativo) != EOF) {

        if (a.id == id) {
            a.ativo = 0;
            encontrado = 1;
        }

        fprintf(temp, "%d;%s;%s;%s;%d\n",
                a.id,
                a.nome,
                a.matricula,
                a.turma,
                a.ativo);
    }

    fclose(f);
    fclose(temp);

    remove(arquivo);
    rename("temp.txt", arquivo);

    if (encontrado) {
        printf("Aluno excluido com sucesso!\n");
    } else {
        printf("Aluno nao encontrado.\n");
    }
}

int main() {

    int opBanco;
    int op;

    char nomeBanco[50];
    char arquivo[150];

    printf("===== SISTEMA DE BANCO DE DADOS =====\n");

    printf("1 Criar novo banco\n");
    printf("2 Usar banco existente\n");

    printf("Escolha: ");
    scanf("%d", &opBanco);

    if (opBanco == 1) {

        printf("Nome do banco da escola: ");
        scanf("%s", nomeBanco);

        criarBanco(nomeBanco);
    }

    printf("Digite o nome do banco que deseja usar: ");
    scanf("%s", nomeBanco);

    sprintf(arquivo, "%s/alunos.txt", nomeBanco);

    do {

        printf("\n===== MENU =====\n");

        printf("1 Cadastrar aluno\n");
        printf("2 Listar alunos\n");
        printf("3 Buscar aluno por ID\n");
        printf("4 Excluir aluno\n");
        printf("0 Sair\n");

        printf("Escolha: ");
        scanf("%d", &op);

        switch(op) {

            case 1:
                cadastrarAluno(arquivo);
                break;

            case 2:
                listarAlunos(arquivo);
                break;

            case 3:
                buscarPorID(arquivo);
                break;

            case 4:
                excluirAluno(arquivo);
                break;

            case 0:
                printf("Encerrando sistema...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while(op != 0);

    return 0;
}
