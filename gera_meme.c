#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===============================================================
CG-23.2
GERADOR DE MEMES (24/09)

>Manoela Resende 2210100235

Utilizando o código em anexo (ou o seu próprio), crie uma função 
que, recebendo ponteiros para uma imagem, um par de coordenada 
(x,y) e uma string de texto, "carimbe-a" com os caracters do texto 
sobre a segunda imagem partindo  das coordenadas especificadas, 
fazendo assim um "criador de memes" simples. Crie uma borda 
escura e sombra nas letras para que fiquem mais fáceis de 
visualizar. Salve o resultado como uma imagem PPM.

================================================================
*/

typedef struct image
{
	int largura;
	int altura;
	int **red;
	int **green;
	int **blue;

} image;

//------------------------
// PROTOTIPOS DAS FUNCOES
//------------------------

image *inicializa(image *imagem, int largura, int altura);
image *apaga(image *imagem);
image *save(image *imagem, char nome[80]);
image *load(char nome[80]);
image *negativo(image *buffer);
image *carimbo(image *im1, image *im2, int y, int x);
image *carimbo2(image *im1, image *im2, int y, int x, int c);
image *carimbarFrase(image *figura1, image *figura2, const char *frase, int x, int y, int espacamento_horizontal, int espacamento_vertical);


int main()
{
	int i, j;
	image *font;
	image *meme;
	font = load("impactfont.ppm");
	//negativo(font);
	meme = load("whenIdo.ppm");

	//FRASE CARIMBADA NA PARTE DE CIMA DO MEME
	char *frase_sup_L1 = "Idontalways"; //linha 1 superior
	char *frase_sup_L2 = "makememes"; //linha 2

	//FRASE CARIMBADA NA PARTE DE BAIXO DO MEME
	char *frase_inf_L1 = "Butwhen I"; //linha 1 inferior
	char *frase_inf_L2 = "do,is in C"; //linha 2

	int x = 7; //quanto menor, mais perto da margem

	//LOCALIZAÇÃO DA FRASE SUPERIOR
	int y_sup_L1 = 10; //altura no eixo y da linha 1, quando menor mais pra cima
	int y_sup_L2 = 100;

	//LOCALIZACÃO DA FRASE INFERIOR
	int y_inf_L1 = 400; //quanto maior, mais pra baixo
	int y_inf_L2 = 500;

	int espacamento_horizontal = 45; //o quanto as letras ficam juntas ou separadas
	int espacamento_vertical = 20; //o quanto se deslocam pra cima

	int espacamento_frase_sup_L2 = 50; //corrigindo bug se usasse com o espacamento das outras


	//PARTE DE CIMA
	meme = carimbarFrase(font, meme, frase_sup_L1, x, y_sup_L1, espacamento_horizontal, espacamento_vertical);
	meme = carimbarFrase(font, meme, frase_sup_L2, x, y_sup_L2, espacamento_frase_sup_L2, espacamento_vertical);

	//PARTE DE BAIXO
	meme = carimbarFrase(font, meme, frase_inf_L1, x, y_inf_L1, espacamento_horizontal, espacamento_vertical);
	meme = carimbarFrase(font, meme, frase_inf_L2, x, y_inf_L2, espacamento_horizontal, espacamento_vertical);

	save(meme, "meme.ppm");

	apaga(meme);
	apaga(font);

	return 0;
}


//============
// FUNCOES
//============


//inicializa uma imagem com a largura e altura especificadas. Todos os pixels são inicialmente definidos como preto 
image *inicializa(image *imagem, int largura, int altura)
{
	int i, j;
	imagem->altura = altura;
	imagem->largura = largura;
	imagem->red = (int **)malloc(altura * sizeof(int *));
	for (i = 0; i < altura; i++)
		imagem->red[i] = (int *)malloc(largura * sizeof(int));

	imagem->green = (int **)malloc(altura * sizeof(int *));
	for (i = 0; i < altura; i++)
		imagem->green[i] = (int *)malloc(largura * sizeof(int));

	imagem->blue = (int **)malloc(altura * sizeof(int *));
	for (i = 0; i < altura; i++)
		imagem->blue[i] = (int *)malloc(largura * sizeof(int));

	for (i = 0; i < imagem->altura; i++)
		for (j = 0; j < imagem->largura; j++)
		{
			imagem->red[i][j] = 0;
			imagem->green[i][j] = 0;
			imagem->blue[i][j] = 0;
		}
	return imagem;
}

//libera a memória alocada para uma imagem
image *apaga(image *imagem)
{
	int i;
	for (i = 0; i < imagem->altura; i++)
	{
		free(imagem->red[i]);
		free(imagem->green[i]);
		free(imagem->blue[i]);
	}
	free(imagem->red);
	free(imagem->green);
	free(imagem->blue);
	free(imagem);
	return NULL;
}


//salva uma imagem em um arquivo, no formato P3 PPM
image *save(image *imagem, char nome[80])
{

	FILE *arq;
	arq = fopen(nome, "wt");
	int i, j;
	fprintf(arq, "P3\n%d %d\n255\n", imagem->largura, imagem->altura);
	for (i = 0; i < imagem->altura; i++)
	{
		for (j = 0; j < imagem->largura; j++)
		{
			fprintf(arq, "%d %d %d ", imagem->red[i][j], imagem->green[i][j], imagem->blue[i][j]);
		}
		fprintf(arq, "\n");
	}

	fclose(arq);

	return imagem;
}

//carrega uma imagem de um arquivo necessariamente P3 PPM.
image *load(char nome[80])
{

	FILE *arq;

	char linha1[100];
	int i, j, width, height, maxcolor;
	image *buffer;

	arq = fopen(nome, "rt");
	// fgets(linha1,sizeof linha1, arq);
	fgets(linha1, sizeof linha1, arq);
	fscanf(arq, "%i %i", &width, &height);
	fscanf(arq, "%i", &maxcolor);

	buffer = malloc(sizeof(image));

	buffer = inicializa(buffer, width, height);

	for (i = 0; i < buffer->altura; i++)
	{
		for (j = 0; j < buffer->largura; j++)
		{
			fscanf(arq, "%i", &(buffer->red[i][j]));
			fscanf(arq, "%i", &(buffer->green[i][j]));
			fscanf(arq, "%i", &(buffer->blue[i][j]));
		}
	}

	fclose(arq);
	return buffer;
}

//inverte as cores de uma imagem
image *negativo(image *buffer)
{
	int i, j;
	for (i = 0; i < buffer->altura; i++)
	{
		for (j = 0; j < buffer->largura; j++)
		{
			buffer->red[i][j] = 255 - buffer->red[i][j];
			buffer->green[i][j] = 255 - buffer->green[i][j];
			buffer->blue[i][j] = 255 - buffer->blue[i][j];
		}
	}
	return buffer;
}

//carimba uma imagem em outra
image *carimbo(image *im1, image *im2, int y, int x)
{
	int i, j;
	for (i = 0; i < im2->altura; i++)
		for (j = 0; j < im2->largura; j++)
		{
			if (im2->red[i][j] != 255 && im2->green[i][j] != 255 && im2->blue[i][j] != 255)
			{
				im1->red[(x + i) % (im1->altura)][(y + j) % (im1->largura)] = im2->red[i][j];
				im1->green[(x + i) % (im1->altura)][(y + j) % (im1->largura)] = im2->green[i][j];
				im1->blue[(x + i) % (im1->altura)][(y + j) % (im1->largura)] = im2->blue[i][j];
			}
		}

	return im1;
}

//carimba um caractere específico da imagem de carimbo na imagem de destino
image *carimbo2(image *im1, image *im2, int y, int x, int c)
{
	int digit = 0;
	int i, j, col, flag, currcol, gap;
	col = 0;
	currcol = 0;

	while (digit != c) //c=caractere
	{
		gap = 0;
		for (j = col; j < im2->largura, gap == 0; j++)
			for (i = 0; i < im2->altura; i++)
			{
				if (im2->red[i][j] == 0 && im2->green[i][j] == 0 && im2->blue[i][j] == 0)
				{
					col = j;
					gap = 1;
					break;
				}
			}

		flag = 1;
		for (j = col; j < im2->largura, flag == 1; j++)
		{
			flag = 0;
			currcol = j;
			for (i = 0; i < im2->altura; i++)
			{
				if (im2->red[i][j] == 0 && im2->green[i][j] == 0 && im2->blue[i][j] == 0)
				{
					flag = 1;
				}
			}
		}
		digit++;
		col = currcol;
	}
	gap = 0;
	for (j = col; j < im2->largura, gap == 0; j++)
		for (i = 0; i < im2->altura; i++)
		{
			if (im2->red[i][j] == 0 && im2->green[i][j] == 0 && im2->blue[i][j] == 0)
			{
				col = j;
				gap = 1;
				break;
			}
		}

	flag = 1;
	for (j = col; j < im2->largura, flag == 1; j++)
	{
		flag = 0;
		for (i = 0; i < im2->altura; i++)
		{
			if (im2->red[i][j] == 0 && im2->green[i][j] == 0 && im2->blue[i][j] == 0)
			{
				flag = 1;
				im1->red[(x + i) % (im1->altura)][(y + j - col) % (im1->largura)] = 255;
				im1->green[(x + i) % (im1->altura)][(y + j - col) % (im1->largura)] = 255;
				im1->blue[(x + i) % (im1->altura)][(y + j - col) % (im1->largura)] = 255;
			}
		}
	}

	return im1;
}

//carimba a frase juntando os caracteres
image *carimbarFrase(image *figura1, image *figura2, const char *frase, int x, int y, int espacamento_horizontal, int espacamento_vertical)
{
	int i, j;
	char *indexes = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ";
	int len = strlen(frase);
	int img_largura = figura2->largura;
	int img_altura = figura2->altura;

	for (i = 0; i < len; i++)
	{
		char caractere_frase = frase[i];

		// verificando se atingiu a borda direita
		if (x + espacamento_horizontal > 780)
		{
			x = 0;			  
			y += espacamento_vertical; // proxima linha
		}

		if (y + espacamento_vertical > img_altura)
		{
			// imagem cheia, não podemos escrever mais
			break;
		}

		if (caractere_frase == ' ')
		{
			j = 71; // index para espaço em branco
		}
		else
		{
			for (j = 0; indexes[j] != '\0'; j++)
			{
				if (caractere_frase == indexes[j])
				{
					break;
				}
			}
		}

		figura2 = carimbo2(figura2, figura1, x, y, j);

		x += espacamento_horizontal;
	}

	return figura2;
}