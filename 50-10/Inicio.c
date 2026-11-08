#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototipos (as funcoes sao definidas depois do main)
int correr(void);
int lagrange(void);



// Função principal
int main() {
    int opcao;
    do {
        // Exibe o menu
        printf("\n\033[1;36m                                                          BEM VINDO AO GlicControl\033[0m\n\n");
        printf("1. Visualizar estatisticas da glicemia");
        printf("                    2. Prever dados futuros");
        printf("                                3. Sair\n");
        printf("    \nEscolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
        case 1: // Visualizar estatísticas da glicemia
            correr();
            break;
        case 2: // Prever dados futuros
            lagrange();
            break;
        case 3: // Sair
            printf("Saindo do programa. Obrigado!\n");
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 3);

    return 0;

}

// Função para encontrar o valor máximo (pico) em uma matriz
int encontrarPico(int matriz[][3], int linhas, int colunas) {
    int maximo = matriz[0][0]; // Inicializa com o primeiro elemento

    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            if (matriz[i][j] > maximo) {
                maximo = matriz[i][j];
            }
        }
    }

    return maximo;
}

// Função para calcular a média da glicemia em um determinado período do dia
double calcularMedia(int matriz[][3], int linhas, int periodo) {
    double soma = 0.0;

    for (int i = 0; i < linhas; ++i) {
        soma += matriz[i][periodo];
    }

    return soma / linhas;
}

// Função para preencher a matriz com os dados do usuário
void preencherMatriz(int matriz[][3], int linhas) {
    for (int i = 0; i < linhas; ++i) {
        printf("Insira a glicemia as 8h do dia %d: ", i + 1);
        scanf("%d", &matriz[i][0]);

        printf("Insira a glicemia as 12h do dia %d: ", i + 1);
        scanf("%d", &matriz[i][1]);

        printf("Insira a glicemia as 21:30h do dia %d: ", i + 1);
        scanf("%d", &matriz[i][2]);
    }
}

void preencherMatrizArquivo(int matriz[][3], int linhas) {
    FILE* arquivo = fopen("dados_glicemia.txt", "r");

    if (arquivo == NULL) {
        perror("\n\033[1;31mErro ao abrir o arquivo\033[0m");
        exit(EXIT_FAILURE);
    }

    // Verifica se o arquivo está vazio
    fseek(arquivo, 0, SEEK_END);
    long tamanhoArquivo = ftell(arquivo);

    if (tamanhoArquivo == 0) {
        printf("O arquivo está vazio. Por favor, adicione os dados:\n");
        fclose(arquivo);

        // Chama a função para preencher a matriz com os valores do usuário
        preencherMatriz(matriz, linhas);

        // Abre o arquivo para escrita
        arquivo = fopen("dados_glicemia.txt", "w");
        if (arquivo == NULL) {
            perror("Erro ao abrir o arquivo");
            exit(EXIT_FAILURE);
        }

        // Escreve os valores no arquivo
        for (int i = 0; i < linhas; ++i) {
            fprintf(arquivo, "%d %d %d\n", matriz[i][0], matriz[i][1], matriz[i][2]);
        }

        fclose(arquivo);
    }
    else {
        // O arquivo não está vazio, lê os dados do arquivo
        fseek(arquivo, 0, SEEK_SET);

        // Alteração: Lê os dados até o final do arquivo
        for (int i = 0; i < linhas && !feof(arquivo); ++i) {
            if (fscanf(arquivo, "%d %d %d", &matriz[i][0], &matriz[i][1], &matriz[i][2]) != 3) {
                fprintf(stderr, "\nErro ao ler os dados do arquivo\n");
                exit(EXIT_FAILURE);
            }
        }

        fclose(arquivo);
    }
}

void imprimirTabela(int matriz[][3], int linhas) {
    // Define escape codes para cores
    const char* cianoClaro = "\x1b[1;36m";
    const char* resetCor = "\x1b[0m";

    // Imprime a tabela estilizada
    printf("\n+------+------------------+-------------------+---------------------+\n");
    printf("%s| %-4s | %-16s | %-17s | %-20s|%s\n", cianoClaro, "Dia", "Glicemia as 8h", "Glicemia as 12h", "Glicemia as 21:30h", resetCor);
    printf("+------+------------------+-------------------+---------------------+\n");

    for (int i = 0; i < linhas; ++i) {
        // Imprime a coluna "Dia" em ciano e o restante em branco
        printf("%s| %-4d%s | %-16d | %-17d | %-20d|%s\n", cianoClaro, i + 1, resetCor, matriz[i][0], matriz[i][1], matriz[i][2], resetCor);

        // Adiciona linhas horizontais após cada linha de dados, exceto na última linha
        if (i < linhas - 1) {
            printf("+------+------------------+-------------------+---------------------+\n");
        }
    }

    printf("+------+------------------+-------------------+---------------------+%s\n", resetCor);
}

// Função para exibir mensagens de saúde com base no valor máximo
void exibirMensagemSaude(int pico) {
    if (pico < 70) {
        printf("\x1b[1;31m\n!ALERTA!: Valor muito baixo. Consulte um medico.\x1b[0m\n");
    }
    
    else if (pico > 180) {
        printf("\x1b[1;31m\n!ALERTA!: Valor muito alto. Consulte um medico.\x1b[0m\n");
    }
    else {
        printf("\x1b[1;32m\nSeus niveis de glicemia estao dentro da faixa normal.\x1b[0m\n");
    }
    printf("\n----------------------------------------------------\n");
}

// Função principal
int correr() {
    // Pergunta ao usuário quantos dias deseja registrar
    int numDias;
    printf("Quantos dias deseja ler ? -> ");
    scanf("\n%d", &numDias);

    if (numDias <= 0) {
        printf("\nErro na analise de dados.\n");
        return 1; // Indica um erro
    }

    // Cria a matriz dinamicamente
    int(*dados)[3] = malloc(numDias * sizeof(*dados));

    // Preenche a matriz com dados do arquivo
    preencherMatrizArquivo(dados, numDias);

    // Verifica se os dados foram lidos corretamente
    printf("\nVerificar dados lidos:\n");
    imprimirTabela(dados, numDias);

    // Encontra e imprime o pico
    int pico = encontrarPico(dados, numDias, 3);

    printf("\n----------------------------------------------------\n");
    printf("\x1b[1;33mValor maximo encontrado nos dados diarios: %d mg/dl\x1b[0m\n", pico);
  

    // Exibe mensagens de saúde com base no valor máximo
    exibirMensagemSaude(pico);

    // Calcula e exibe médias de glicemia
    double mediaManha = calcularMedia(dados, numDias, 0);
    double mediaTarde = calcularMedia(dados, numDias, 1);
    double mediaNoite = calcularMedia(dados, numDias, 2);

    printf("\nMedias obtidas:\n");
    printf("\n----------------------------------------\n");
    printf("Media da glicemia de manha: %.2lf mg/dl\n", mediaManha);
    printf("Media da glicemia a tarde: %.2lf mg/dl\n", mediaTarde);
    printf("Media da glicemia a noite: %.2lf mg/dl\n", mediaNoite);
    printf("----------------------------------------\n");

    // Libera a memória alocada para a matriz
    free(dados);

    return 0;
}

void lagrange_interpolation(double x_values[], double y_values[], int n) {
    printf("\nPolinomio interpolador de Lagrange:\n");

    printf("P(x) = ");
    for (int i = 0; i < n; i++) {
        printf("(%.4f)", y_values[i]);

        for (int j = 0; j < n; j++) {
            if (j != i) {
                printf(" * (x - %.2f) / (%.2f - %.2f)", x_values[j], x_values[i], x_values[j]);
            }
        }

        if (i < n - 1) {
            printf(" + ");
        }
    }

    printf("\n");
}

double lagrange_interpolation_value(double x_values[], double y_values[], int n, double x) {
    double result = 0.0;

    for (int i = 0; i < n; i++) {
        double term = y_values[i];

        for (int j = 0; j < n; j++) {
            if (j != i) {
                term *= (x - x_values[j]) / (x_values[i] - x_values[j]);
            }
        }

        result += term;
    }

    return result;
}

int lagrange() {
    int n;

    // Solicita ao usuário o número de pontos de dados
    printf("\nDigite o numero de dias de dados: ");
    scanf("%d", &n);

    // Aloca espaço para a matriz de pontos de dados
    double(*points)[2] = malloc(n * sizeof(*points));

    // Verifica se a alocação foi bem-sucedida
    if (points == NULL) {
        fprintf(stderr, "\033[1;31mErro na alocação de memória.\033[0m\n");
        return 1;  // Código de erro
    }

    // Solicita ao usuário os pontos de dados
    printf("Digite os pontos de dados (formato: x y):\n");
    for (int i = 0; i < n; i++) {
        printf("Ponto %d: ", i + 1);
        scanf("%lf %lf", &points[i][0], &points[i][1]);
    }

    // Aloca espaço para os arrays de x e y
    double* x_values = (double*)malloc(n * sizeof(double));
    double* y_values = (double*)malloc(n * sizeof(double));

    // Verifica se a alocação foi bem-sucedida
    if (x_values == NULL || y_values == NULL) {
        fprintf(stderr, "Erro na alocação de memória.\n");
        return 1;  // Código de erro
    }

    // Transfere os pontos da matriz para os arrays
    for (int i = 0; i < n; i++) {
        x_values[i] = points[i][0];
        y_values[i] = points[i][1];
    }

    // Ponto para interpolação
    double x_to_interpolate;
    printf("Digite o valor de x para interpolacao: ");
    scanf("%lf", &x_to_interpolate);

    // Exibe o polinômio interpolador
    lagrange_interpolation(x_values, y_values, n);

    // Calcula o valor interpolado
    double result = lagrange_interpolation_value(x_values, y_values, n, x_to_interpolate);

    // Exibe o resultado
    printf("O valor interpolado para x=%.2f e %.2f\n", x_to_interpolate, result);

    // Libera a memória alocada
    free(points);
    free(x_values);
    free(y_values);

    return 0;
}