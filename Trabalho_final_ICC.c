/*
        PROJETO ICC1

            GRUPO:
    Leonardo Demore
    n°USP: 15674786

    Matheus Fonseca
    n°USP: 15640447

    Arthur Araujo
    n°USP: 14651458

        INFORMAÇÕES:
    Neste projeto, priorizamos ao máximo a legibilidade e
    otimização do código, realizando comentários por todo o
    código e optando pelo uso de funções para economia de variáveis,
    utilizando apenas aquelas que são estritamente necessárias (apenas 5).
    Além disso, a lógica por trás do código funciona da seguinte forma:
    Na primeira vez que o código está sendo executado, cria um arquivo
    para salvar os dados. Quando o usuário digitar 'FD', salva todos os
    dados no arquivo e encerra o terminal. Ao executar novamente, o
    algoritmo "puxa" os dados do arquivo, os coloca em suas respectivas
    variáveis e continua executando normalmente, como se nem houvesse parado.
    Dessa forma, diminuimos o risco de erros na lógica, assim como erros ao
    salvar e carregar arquivos.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*struct com todos os dados da reserva;
(cada struct representa um passageiro)*/
struct ficha_passageiro{
    char *nome;
    char *sobrenome;
    char cpf[15];
    char data[11];
    char classe_passagem[10];
    char origem[4];
    char destino[4];
    float valor_passagem;
    char numero_voo[5];
    char assento[4];
};

/*função para salvar todos os dados;
usamos antes de fechar o código: caso 'FD'*/
void salvar_dados(char *nome_arquivo, struct ficha_passageiro *passageiros, int tamanho, int qtd_assentos, int flag) {
    //abre o arquivo em modo de escrita binária
    FILE *arquivo = fopen(nome_arquivo, "wb");

    if (arquivo == NULL)
        return;

    //escreve a quantidade atual de passageiros (structs)
    fwrite(&tamanho, sizeof(int), 1, arquivo);
    
    //escreve o valor da flag
    fwrite(&flag, sizeof(int), 1, arquivo);

    //escreve a quantidade total de assentos do voo
    fwrite(&qtd_assentos, sizeof(int), 1, arquivo);

    //loop para escrever todas as structs
    for (int i = 0; i < tamanho; i++) {
        int nome_tamanho = strlen(passageiros[i].nome) + 1; //tamanho do nome
        int sobrenome_tamanho = strlen(passageiros[i].sobrenome) + 1; //tamanho do sobrenome

        fwrite(&nome_tamanho, sizeof(int), 1, arquivo); //escreve o tamanho do nome
        fwrite(passageiros[i].nome, sizeof(char), nome_tamanho, arquivo); //escreve o nome

        fwrite(&sobrenome_tamanho, sizeof(int), 1, arquivo); //escreve o tamanho do sobrenome
        fwrite(passageiros[i].sobrenome, sizeof(char), sobrenome_tamanho, arquivo); //escreve o sobrenome

        //escreve os outros campos da estrutura
        fwrite(passageiros[i].cpf, sizeof(char), 15, arquivo);
        fwrite(passageiros[i].data, sizeof(char), 11, arquivo);
        fwrite(passageiros[i].classe_passagem, sizeof(char), 10, arquivo);
        fwrite(passageiros[i].origem, sizeof(char), 4, arquivo);
        fwrite(passageiros[i].destino, sizeof(char), 4, arquivo);
        fwrite(&passageiros[i].valor_passagem, sizeof(float), 1, arquivo);
        fwrite(passageiros[i].numero_voo, sizeof(char), 5, arquivo);
        fwrite(passageiros[i].assento, sizeof(char), 4, arquivo);
    }

    fclose(arquivo); //fecha o arquivo
}

/*função para ler todos os dados salvos;
usamos ao executar o código, para recuperar
o que foi salvo previamente*/
struct ficha_passageiro* carregar_dados(char *nome_arquivo, int *tamanho, int *qtd_assentos, int *flag) {
    //abre o arquivo em modo leitura binária
    FILE *arquivo = fopen(nome_arquivo, "rb");

    if (arquivo == NULL) {
        return NULL;
    }

    //lê a quantidade atual de passageiros
    fread(tamanho, sizeof(int), 1, arquivo);
    
    //lê a flag
    fread(flag, sizeof(int), 1, arquivo);

    //lê a quantidade total de assentos
    fread(qtd_assentos, sizeof(int), 1, arquivo);

    struct ficha_passageiro *passageiros = malloc(*tamanho * sizeof(struct ficha_passageiro));
    if (passageiros == NULL) {
        fclose(arquivo);
        return NULL;
    }

    for (int i = 0; i < *tamanho; i++) {
        int nome_tamanho;
        int sobrenome_tamanho;

        fread(&nome_tamanho, sizeof(int), 1, arquivo);
        passageiros[i].nome = malloc(nome_tamanho * sizeof(char));
        fread(passageiros[i].nome, sizeof(char), nome_tamanho, arquivo);

        fread(&sobrenome_tamanho, sizeof(int), 1, arquivo);
        passageiros[i].sobrenome = malloc(sobrenome_tamanho * sizeof(char));
        fread(passageiros[i].sobrenome, sizeof(char), sobrenome_tamanho, arquivo);

        fread(passageiros[i].cpf, sizeof(char), 15, arquivo);
        fread(passageiros[i].data, sizeof(char), 11, arquivo);
        fread(passageiros[i].classe_passagem, sizeof(char), 10, arquivo);
        fread(passageiros[i].origem, sizeof(char), 4, arquivo);
        fread(passageiros[i].destino, sizeof(char), 4, arquivo);
        fread(&passageiros[i].valor_passagem, sizeof(float), 1, arquivo);
        fread(passageiros[i].numero_voo, sizeof(char), 5, arquivo);
        fread(passageiros[i].assento, sizeof(char), 4, arquivo);
    }

    fclose(arquivo);
    return passageiros;
}

//função para alocar memória para cada struct
void ajusta_memoria_struct(struct ficha_passageiro **passageiros, int *tamanho){

    *tamanho += 1;
    *passageiros = realloc(*passageiros, (*tamanho) * sizeof(struct ficha_passageiro));
    // Inicializa as strings com NULL
    (*passageiros)[*tamanho - 1].nome = NULL;
    (*passageiros)[*tamanho - 1].sobrenome = NULL;
}

/*função para alocar memória dinamicamente para um campo da struct;
usamos esta função para salvar o nome e o sobrenome, que variam a cada passageiro
(portanto, alocação dinâmica é mais eficiente)*/
void ajusta_memoria_string(char **campo, char string_temp[50]){

    *campo = (char *)malloc((strlen(string_temp) + 1) * sizeof(char));
    strcpy(*campo, string_temp);
}

//função para escanear os dados de um novo passageiro
void ler_dados(struct ficha_passageiro **passageiros, int indice){

    int dia, mes, ano;
    char temp_nome[50];
    char temp_sobrenome[50];

    scanf(" %s %s", temp_nome, temp_sobrenome);
    ajusta_memoria_string(&((*passageiros)[indice].nome), temp_nome);
    ajusta_memoria_string(&((*passageiros)[indice].sobrenome), temp_sobrenome);

    scanf(" %s", (*passageiros)[indice].cpf);

    scanf(" %d %d %d", &dia, &mes, &ano);
    //comando para salvar a data no formato correto
    sprintf((*passageiros)[indice].data, "%02d/%02d/%d", dia, mes, ano);

    scanf(" %s", (*passageiros)[indice].numero_voo);

    scanf(" %s", (*passageiros)[indice].assento);

    scanf(" %s", (*passageiros)[indice].classe_passagem);

    scanf(" %f", &((*passageiros)[indice].valor_passagem));

    scanf(" %s", (*passageiros)[indice].origem);

    scanf(" %s", (*passageiros)[indice].destino);

}

/*função para liberar apenas dois campos de uma struct especifica;
usamos para liberar nome e sobrenome (alocados dinamicamente)*/
void liberar_ficha(struct ficha_passageiro *passageiro){
    if (passageiro->nome != NULL) {
        free(passageiro->nome);
    }
    if (passageiro->sobrenome != NULL) {
        free(passageiro->sobrenome);
    }
}

/*função para liberar totalmente a memoria;
usamos antes de fechar o terminal (casos 'FD' e 'FV')*/
void free_em_tudo(struct ficha_passageiro *passageiros, int tamanho){
    //liberando o que foi alocado dinamicamente
    for (int i = 0; i < tamanho; i++){
        free(passageiros[i].nome);
        free(passageiros[i].sobrenome);
    }
    //liberando a struct em si
    free(passageiros);
}

/*função para imprimir os dados de todas as reservas até o momento;
utilizamos para realizar testes durante a escrita*/
void imprimir_tudo(struct ficha_passageiro *passageiro, int tamanho){
    for(int i=0; i<tamanho; i++){
    printf("Nome: %s\n", passageiro[i].nome);
    printf("Sobrenome: %s\n", passageiro[i].sobrenome);
    printf("CPF: %s\n", passageiro[i].cpf);
    printf("Data: %s\n", passageiro[i].data);
    printf("Número do Voo: %s\n", passageiro[i].numero_voo);
    printf("Assento: %s\n", passageiro[i].assento);
    printf("Classe da Passagem: %s\n", passageiro[i].classe_passagem);
    printf("Valor da Passagem: %.2f\n", passageiro[i].valor_passagem);
    printf("Origem: %s\n", passageiro[i].origem);
    printf("Destino: %s\n", passageiro[i].destino);
    }
}

/*função para remover a reserva com base no CPF;
usamos no caso 'CA' (cancelar reserva)*/
void cancelar_reserva(struct ficha_passageiro **passageiros, int *tamanho) {
    char cpf[15];
    scanf(" %s", cpf);

    int i, j;
    // Procurando a reserva com o CPF fornecido
    for (i = 0; i < *tamanho; i++) {
        if (strcmp((*passageiros)[i].cpf, cpf) == 0) {
            // Liberando a memória da reserva que será removida
            liberar_ficha(&(*passageiros)[i]);

            // Movendo as reservas subsequentes uma posição para trás
            for (j = i; j < *tamanho - 1; j++) {
                (*passageiros)[j] = (*passageiros)[j + 1];
            }
            (*tamanho)--; // Atualizando o número de passageiros

            if (*tamanho > 0) {
                struct ficha_passageiro *temp = realloc(*passageiros, (*tamanho) * sizeof(struct ficha_passageiro));
                if (temp == NULL) {
                    fprintf(stderr, "Erro ao realocar memória.\n");
                    exit(1);
                }
                *passageiros = temp;
            } else {
                // Se o tamanho for 0, libera a memória e define passageiros como NULL
                free(*passageiros);
                *passageiros = NULL;
            }

            return;
        }
    }
}

/*função que mostra todos os dados de uma reserva
com base no CPF fornecido; utilizamos no caso 'CR'*/
void consulta_reserva(struct ficha_passageiro *passageiros, int tamanho){
    char cpf[15];
    scanf(" %s", cpf);

    for (int i = 0; i < tamanho; i++) {
        if (strcmp(passageiros[i].cpf, cpf) == 0){
            printf("%s\n", passageiros[i].cpf);
            printf("%s ", passageiros[i].nome);
            printf("%s\n", passageiros[i].sobrenome);
            printf("%s\n", passageiros[i].data);
            printf("Voo: %s\n", passageiros[i].numero_voo);
            printf("Assento: %s\n", passageiros[i].assento);
            printf("Classe: %s\n", passageiros[i].classe_passagem);
            printf("Trecho: %s ", passageiros[i].origem);
            printf("%s\n", passageiros[i].destino);
            printf("Valor: %.2f\n", passageiros[i].valor_passagem);
            printf("--------------------------------------------------\n");
        }
    }
}

/*função que altera a reserva do passageiro com
base no CPF fornecido; utilizamos no caso 'MR'*/
void modificar_reserva(struct ficha_passageiro *passageiros, int tamanho) {
    char cpf[15];

    //escaneando o CPF da reserva que será modificada
    scanf(" %14s", cpf);

    //procurando a reserva com o CPF fornecido e modificando-a
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(passageiros[i].cpf, cpf) == 0){
            // Modificando a reserva:
            scanf(" %s", passageiros[i].nome);
            scanf(" %s", passageiros[i].sobrenome);
            scanf(" %s", passageiros[i].cpf);
            scanf(" %s", passageiros[i].assento);

            printf("Reserva Modificada:\n");
            printf("%s\n", passageiros[i].cpf);
            printf("%s %s\n", passageiros[i].nome, passageiros[i].sobrenome);
            printf("%s\n", passageiros[i].data);
            printf("Voo: %s\n", passageiros[i].numero_voo);
            printf("Assento: %s\n", passageiros[i].assento);
            printf("Classe: %s\n", passageiros[i].classe_passagem);
            printf("Trecho: %s %s\n", passageiros[i].origem, passageiros[i].destino);
            printf("Valor: %.2f\n", passageiros[i].valor_passagem);
            printf("--------------------------------------------------\n");
            return;
        }
    }
}

//função para somar o total arrecadado com as passagens
double total_arrecadado(struct ficha_passageiro *passageiros, int tamanho){
    double total = 0;
    for(int i=0; i<tamanho; i++){
        total += passageiros[i].valor_passagem;
    }
    return total;
}

int main(){

    //guarda a quantidade atual de passageiros
    int tamanho = 0;
    //guarda o total de assentos do voo
    int qtd_assentos = 0;
    //guarda se o voo já foi fechado ou não
    int flag = 0;
    //guardam os valores das passagens
    float valor_eco = 0.0, valor_exec = 0.0;
    //array para guardar todas as reservas
    struct ficha_passageiro *passageiros = carregar_dados("fichas.bin", &tamanho, &qtd_assentos, &flag);

    if (passageiros == NULL && tamanho > 0) {
        fprintf(stderr, "Erro ao carregar os dados\n");
        return 1;
    }

    //verifica se o caso "FV" já ocorreu;
    //0 = nunca ocorreu, 1 = já ocorreu
    if(flag == 0){
        //inicializa o loop onde todo o código ocorre
        char caso[3];
        while (1) {
            scanf(" %2s", caso);

            if (strcmp(caso, "FV") == 0) { //fechamento do voo
                printf("Voo Fechado!\n\n");
                for (int i = 0; i < tamanho; i++) {
                    printf("%s\n", passageiros[i].cpf);
                    printf("%s ", passageiros[i].nome);
                    printf("%s\n", passageiros[i].sobrenome);
                    printf("%s\n\n", passageiros[i].assento);
                }
            printf("Valor Total: %.2lf\n", total_arrecadado(passageiros, tamanho));
            printf("--------------------------------------------------\n");
            flag = 1;
            salvar_dados("fichas.bin", passageiros, tamanho, qtd_assentos, flag);
            free_em_tudo(passageiros, tamanho);
            return 0;

            } else if (strcmp(caso, "FD") == 0) { //fechamento do dia
                printf("Fechamento do dia:\n");
                printf("Quantidade de reservas: %d\n", tamanho);
                printf("Posição: %.2lf\n", total_arrecadado(passageiros, tamanho));
                printf("--------------------------------------------------\n");
                salvar_dados("fichas.bin", passageiros, tamanho, qtd_assentos, flag);
                free_em_tudo(passageiros, tamanho);
                return 0;

            } else if (strcmp(caso, "AV") == 0) { //abrir voo
                scanf(" %d %f %f", &qtd_assentos, &valor_eco, &valor_exec);

            } else if (strcmp(caso, "RR") == 0) { //realizar reserva
                ajusta_memoria_struct(&passageiros, &tamanho);
                ler_dados(&passageiros, tamanho - 1);

            } else if (strcmp(caso, "CR") == 0) { //consultar reserva
                consulta_reserva(passageiros, tamanho);

            } else if (strcmp(caso, "CA") == 0) { //cancelar reserva
                cancelar_reserva(&passageiros, &tamanho);

            } else if (strcmp(caso, "MR") == 0) { //modificar reserva
                modificar_reserva(passageiros, tamanho);
            }

            //verificar se o voo deve ser fechado automaticamente
            if (tamanho == qtd_assentos) {
                printf("Voo Fechado!\n\n");
                for (int i = 0; i < tamanho; i++) {
                    printf("%s\n", passageiros[i].cpf);
                    printf("%s ", passageiros[i].nome);
                    printf("%s\n", passageiros[i].sobrenome);
                    printf("%s\n\n", passageiros[i].assento);
                }   
                printf("Valor Total: %.2lf\n", total_arrecadado(passageiros, tamanho));
                printf("--------------------------------------------------\n");
                flag = 1;
                salvar_dados("fichas.bin", passageiros, tamanho, qtd_assentos, flag);
                free_em_tudo(passageiros, tamanho);
                return 0;
            }
        }
    }
    
    if(flag == 1){
        printf("Voo Fechado!\n\n");
            for (int i = 0; i < tamanho; i++) {
                printf("%s\n", passageiros[i].cpf);
                printf("%s ", passageiros[i].nome);
                printf("%s\n", passageiros[i].sobrenome);
                printf("%s\n\n", passageiros[i].assento);
            }
        printf("Valor Total: %.2lf\n", total_arrecadado(passageiros, tamanho));
        printf("--------------------------------------------------\n");
        
        char caso[3];
        while (1) {
        scanf(" %2s", caso);
            if (strcmp(caso, "CR") == 0) { //consultar reserva
            consulta_reserva(passageiros, tamanho);
           
            } else if (strcmp(caso, "FD") == 0) { //fechamento do dia
            printf("Fechamento do dia:\n");
            printf("Quantidade de reservas: %d\n", tamanho);
            printf("Posição: %.2lf\n", total_arrecadado(passageiros, tamanho));
            printf("--------------------------------------------------\n");
            salvar_dados("fichas.bin", passageiros, tamanho, qtd_assentos, flag);
            free_em_tudo(passageiros, tamanho);
            return 0;
            
            } else if (strcmp(caso, "FV") == 0) { //fechamento do voo
                printf("Voo Fechado!\n\n");
                for (int i = 0; i < tamanho; i++) {
                    printf("%s\n", passageiros[i].cpf);
                    printf("%s ", passageiros[i].nome);
                    printf("%s\n", passageiros[i].sobrenome);
                    printf("%s\n\n", passageiros[i].assento);
                }
                printf("Valor Total: %.2lf\n", total_arrecadado(passageiros, tamanho));
                printf("--------------------------------------------------\n");
                salvar_dados("fichas.bin", passageiros, tamanho, qtd_assentos, flag);
                free_em_tudo(passageiros, tamanho);
                return 0;
            }
        }
    }
}


