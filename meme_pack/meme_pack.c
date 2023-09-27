#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct image{
    int largura;
    int altura;
    int** red;
    int** green;
    int** blue;

}image;

image* inicializa(image* imagem,int largura, int altura){
    int i,j;

    imagem->altura=altura;
    imagem->largura=largura;
	
    imagem->red = (int **)malloc( altura * sizeof(int *));
    for (i=0; i<altura; i++)
        imagem->red[i] = (int *)malloc(largura * sizeof(int));

    imagem->green = (int **)malloc(altura  * sizeof(int *));
    for (i=0; i<altura; i++)
        imagem->green[i] = (int *)malloc(largura * sizeof(int));

    imagem->blue = (int **)malloc(altura  * sizeof(int *));
    for (i=0; i<altura; i++)
        imagem->blue[i] = (int *)malloc(largura * sizeof(int));
	
    for (i = 0; i <  imagem->altura; i++)
        for (j = 0; j < imagem->largura; j++)
        {
            imagem->red[i][j] = 0;
            imagem->green[i][j] = 0;
            imagem->blue[i][j] = 0;
        }
    return imagem;
}

image* apaga(image* imagem){
    int i;

    for (i=0; i<imagem->altura; i++){
        free(imagem->red[i]);
        free(imagem->green[i]);
        free(imagem->blue[i]);
        }
    free(imagem->red);
    free(imagem->green);
    free(imagem->blue);
    free (imagem);
    return NULL;
}

image* mostra(image* imagem){
    int i,j;

    printf("P3\n%d %d\n255\n",imagem->largura, imagem->altura);

    for (i = 0; i <  imagem->altura; i++){
        for (j = 0; j < imagem->largura; j++){
            printf("%d %d %d ",imagem->red[i][j],imagem->green[i][j],imagem->blue[i][j]);
        }
        printf ("\n");
    }
}

image* save(image* imagem, char nome[80]){
	FILE* arq;
	arq=fopen(nome,"wt");
    int i,j;

    fprintf(arq,"P3\n%d %d\n255\n",imagem->largura, imagem->altura);

    for (i = 0; i <  imagem->altura; i++){
        for (j = 0; j < imagem->largura; j++){
            fprintf(arq,"%d %d %d ",imagem->red[i][j],imagem->green[i][j],imagem->blue[i][j]);
        }
        fprintf (arq,"\n");
    }
	fclose(arq);
}

image* load(char nome[80]){     
	FILE* arq;
	
	char linha1[100];
    int i,j,width,height,maxcolor;
	image* buffer;
	
	arq=fopen(nome,"rt");
	fgets(linha1,sizeof linha1, arq);
	fscanf(arq,"%i %i",&width,&height);
	fscanf(arq,"%i",&maxcolor);
	
	buffer = malloc(sizeof(image));
	
	buffer = inicializa(buffer,width, height);

    for (i = 0; i <  buffer->altura; i++){
        for (j = 0; j < buffer->largura; j++){
            fscanf(arq,"%i",&(buffer->red[i][j]));
			fscanf(arq,"%i",&(buffer->green[i][j]));
			fscanf(arq,"%i",&(buffer->blue[i][j]));
        }
    }
	fclose(arq);
	return buffer;
}

image* negativo(image* buffer){
	int i, j;
	 for (i = 0; i <  buffer->altura; i++){
        for (j = 0; j < buffer->largura; j++){
            buffer->red[i][j] = 255 - buffer->red[i][j];
			buffer->green[i][j] = 255 - buffer->green[i][j];
			buffer->blue[i][j] =  255 - buffer->blue[i][j];
        }
   }
   return buffer;
}

image* carimbo(image* im1, image* im2,int y, int x){
	int i,j;
	for (i=0;i<im2->altura;i++)
		for (j=0;j<im2->largura;j++)
		{
			if(im2->red[i][j]!=255 && im2->green[i][j] !=255 && im2->blue[i][j]!=255){
			im1->red[(x+i)%(im1->altura)][(y+j)%(im1->largura)]=im2->red[i][j];
			im1->green[(x+i)%(im1->altura)][(y+j)%(im1->largura)]=im2->green[i][j];
			im1->blue[(x+i)%(im1->altura)][(y+j)%(im1->largura)]=im2->blue[i][j];}
		}	

	/*im1->red[(x+i)%(im1->altura)][(y+j)%(im1->largura)]=im2->red[i][j];
			im1->green[(x+i)%(im1->altura)][(y+j)%(im1->largura)]=im2->green[i][j];
			im1->blue[(x+i)%(im1->altura)][(y+j)%(im1->largura)]=im2->blue[i][j];
	*/
	return im1;	
}

image* carimbo2(image* im1, image* im2, int y, int x, int c) {
    int digit = 0;
    int i, j, col, flag, currcol, gap;
    col = 0;
    currcol = 0;

    while (digit != c) {
        gap = 0;
        for (j = col; j < im2->largura, gap == 0; j++)
            for (i = 0; i < im2->altura; i++) {
                if (im2->red[i][j] == 0 && im2->green[i][j] == 0 && im2->blue[i][j] == 0) {
                    col = j;
                    gap = 1;
                    break;
                }
            }

        flag = 1;
        for (j = col; j < im2->largura, flag == 1; j++) {
            flag = 0;
            currcol = j;
            for (i = 0; i < im2->altura; i++) {
                if (im2->red[i][j] == 0 && im2->green[i][j] == 0 && im2->blue[i][j] == 0) {
                    flag = 1;
                }
            }
        }
        digit++;
        col = currcol;
    }
    gap = 0;
    for (j = col; j < im2->largura, gap == 0; j++)
        for (i = 0; i < im2->altura; i++) {
            if (im2->red[i][j] == 0 && im2->green[i][j] == 0 && im2->blue[i][j] == 0) {
                col = j;
                gap = 1;
                break;
            }
        }

    flag = 1;
    for (j = col; j < im2->largura, flag == 1; j++) {
        flag = 0;
        for (i = 0; i < im2->altura; i++) {
            if (im2->red[i][j] == 0 && im2->green[i][j] == 0 && im2->blue[i][j] == 0) {
                flag = 1;
                // Defina a cor branca (255) ao carimbar a letra e leve em consideração o valor de x
                im1->red[(x + i) % (im1->altura)][(y + j - col) % (im1->largura)] = 255;
                im1->green[(x + i) % (im1->altura)][(y + j - col) % (im1->largura)] = 255;
                im1->blue[(x + i) % (im1->altura)][(y + j - col) % (im1->largura)] = 255;
            }
        }
    }

    return im1;
}


//-------------------------------------TAREFAS--------------------------------------------
///1) Implemente a funcao setpixel(coluna, linha, r,g,b); - define a cor do pixel nas coordenadas (coluna,linha) com as componentes r,g,b
image* setPixel(image* imagem, int coluna, int linha, int r, int g, int b) {
    imagem->red[linha][coluna] = r;
    imagem->green[linha][coluna] = g;
    imagem->blue[linha][coluna] = b;
}

//2) Implemente as funções getred(coluna, linha), que retorna o valor da componente vermelha nas coordenadas dadas, bem como getgreen e getblue (analogamente)
int getRed(image* imagem, int coluna, int linha) {
    return imagem->red[linha][coluna];
}

int getGreen(image* imagem, int coluna, int linha) {
    return imagem->green[linha][coluna];
}

int getBlue(image* imagem, int coluna, int linha) {
    return imagem->blue[linha][coluna];
}

//3) Crie a função salvaImagem(image*, const char* nome_arquivo) para salvar uma imagem ppm no disco
image* salvaImagem(image* imagem, char nome[80]) {
    FILE* arq;
    arq = fopen(nome, "wt");

    int i, j;
    fprintf(arq, "P3\n%d %d\n255\n", imagem->largura, imagem->altura);

    for (i = 0; i < imagem->altura; i++) {
        for (j = 0; j < imagem->largura; j++) {
            fprintf(arq, "%d %d %d ", imagem->red[i][j], imagem->green[i][j], imagem->blue[i][j]);
        }
        fprintf(arq, "\n");
    }

    fclose(arq);
}

// 4) Preencher uma imagem com cores aleatórias, salvando-a no disco.
image* preencheImagem(image* imagem) {
    int i, j;

    srand(time(NULL));

    for (i = 0; i < imagem->altura; i++) {
        for (j = 0; j < imagem->largura; j++) {
            imagem->red[i][j] = rand() % 256;  // Valores aleatórios entre 0 e 255
            imagem->green[i][j] = rand() % 256;
            imagem->blue[i][j] = rand() % 256;
        }
    }
}

// 5) Crie função leImagem(image*, const char* nome_arquivo) para ler uma imagem ppm do disco. Implemente um filtro de negativação. Teste as funções carregando uma fotografia qualquer do disco (em PPM), negativando-a e salvando-a novamente no disco.
/*
image* leImagem(const char* nome_arquivo) {
    image* imagemOriginal = load(nome_arquivo);

    if (!imagemOriginal) {
        printf("Erro ao carregar a imagem.\n");
        return NULL;
    }

    imagemOriginal = negativo(imagemOriginal);

    char nome_negativo[80];
    snprintf(nome_negativo, sizeof(nome_negativo), "negativo_%s", nome_arquivo);

    // Salva a imagem negativada no disco
    save(imagemOriginal, nome_negativo);

    printf("Imagem negativada salva como: %s\n", nome_negativo);

    return imagemOriginal; // Retorna a imagem negativada
}
*/
//------------------------------------------------------------------------------------


//----------------------------------------PROVA---------------------------------------
//6) CARIMBA MARCA D'AGUA
image* carimbarWatermark(image* im1, image* im2, int y, int x) {
    int i, j;
    for (i = 0; i < im2->altura; i++) {
        for (j = 0; j < im2->largura; j++) {
            if (im2->red[i][j] != 0 && im2->green[i][j] != 0 && im2->blue[i][j] != 0) {
                im1->red[(x + i) % (im1->altura)][(y + j) % (im1->largura)] =
                    (im1->red[(x + i) % (im1->altura)][(y + j) % (im1->largura)] + im2->red[i][j]) / 2;
                im1->green[(x + i) % (im1->altura)][(y + j) % (im1->largura)] =
                    (im1->green[(x + i) % (im1->altura)][(y + j) % (im1->largura)] + im2->green[i][j]) / 2;
                im1->blue[(x + i) % (im1->altura)][(y + j) % (im1->largura)] =
                    (im1->blue[(x + i) % (im1->altura)][(y + j) % (im1->largura)] + im2->blue[i][j]) / 2;
            }
        }
    }
    return im1;
}

//7) CARIMBA FRASE
image* carimbarFrase(image* figura1, image* figura2, const char* frase, int x, int y, int larguraLetra, int alturaLetra) {
    int i, j;
    char* indexes = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,.!?-+()";
    int len = strlen(frase);
    int imagemLargura = figura2->largura;
    int imagemAltura = figura2->altura;

    for (i = 0; i < len; i++) {
        char caractereFrase = frase[i];
        
        // Verifique se atingiu a borda direita da imagem
        if (x + larguraLetra > 790) {
            x = 100; // Volte para a coluna 0
            y += alturaLetra; // Mova para a próxima linha
        }

        if (y + alturaLetra > imagemAltura) {
            // A imagem está cheia, não podemos escrever mais
            break;
        }

        if (caractereFrase == ' ') {
            j = 70; // Índice para espaço em branco
        } else {
            for (j = 0; indexes[j] != '\0'; j++) {
                if (caractereFrase == indexes[j]) {
                    break;
                }
            }
        }
        
        figura2 = carimbo2(figura2, figura1, x, y, j);
        
        x += larguraLetra; 
    }

    return figura2;
}



// Função para ler frases de um arquivo de texto
void lerFrases(char* nome_arquivo, char** frases, int* numeroFrases) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de frases");
        exit(1);
    }

    char linha[256];
    int contador = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = '\0'; // Remove o caractere de nova linha
        frases[contador] = strdup(linha);   // Aloca memória para a frase
        contador++;
    }

    fclose(arquivo);
    *numeroFrases = contador;
}


//------------------------------------------------------------------------------------

int main() {
    image* figura1;
    image* watermark;

    // O nome da pasta que você deseja criar
    char* folderName = "images";

    // Estrutura para armazenar as informações do arquivo/pasta
    struct stat st = {0};

    // Verifica se a pasta já existe
    if (stat(folderName, &st) == -1) {
        // Se a pasta não existir, cria a pasta
        if (mkdir(folderName, 0755) == -1) {
            perror("Erro ao criar a pasta");
            return 1;
        }
    }

    int num_imagens = 10;  // Número de imagens disponíveis
    int num_frases = 0;
    
    char* frases[100];  
    char* imagens[10] = {"crf.ppm", "cristo.ppm", "gabi.ppm", "manto.ppm", "maraca.ppm", "mosaico.ppm", "rei.ppm", "remo.ppm","torcida.ppm", "urubu.ppm"};

    


    figura1 = load("impactfont.ppm");
    watermark = load("watermark.ppm");
    negativo(watermark);

    srand(time(NULL));

    lerFrases("frases.txt", frases, &num_frases);
    for (int img_index = 0; img_index < num_imagens; img_index++) {
        for (int frase_index = 0; frase_index < num_frases; frase_index++) {
            image* figura2 = load(imagens[img_index]);  // Carregue uma nova cópia da imagem

            int x = 20;
            int y = 500;
            int larguraLetra = 45;  // Largura da letra
            int alturaLetra = 80;   // Altura da letra, ajuste conforme necessário

            char* frase_escolhida = frases[frase_index];

            figura2 = carimbarFrase(figura1, figura2, frase_escolhida, x, y, larguraLetra, alturaLetra);
            figura2 = carimbarWatermark(figura2, watermark, 350, 50);  // Carimbar a marca d'água

            char nomeOutput[100];
            snprintf(nomeOutput, sizeof(nomeOutput), "images/img_%s_frase%d.ppm", imagens[img_index], frase_index+1);
            save(figura2, nomeOutput);

            apaga(figura2);
        }
    }

    for (int i = 0; i < num_frases; i++) {
        if (frases[i] != NULL) {
            free(frases[i]);
        }
    }

    apaga(figura1);
    apaga(watermark);

    return 0;
}
