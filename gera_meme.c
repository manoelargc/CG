#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct image
{
	int largura;
	int altura;
	int **red;
	int **green;
	int **blue;

} image;

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

image *carimbo2(image *im1, image *im2, int y, int x, int c)
{

	int digit = 0;
	int i, j, col, flag, currcol, gap;
	col = 0;
	currcol = 0;

	while (digit != c)
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
				{
					if (im2->red[i][j] == 0 && im2->green[i][j] == 0 && im2->blue[i][j] == 0)
					{
						flag = 1;
					}
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
			{
				if (im2->red[i][j] == 0 && im2->green[i][j] == 0 && im2->blue[i][j] == 0)
				{
					flag = 1;
					im1->red[(x + i) % (im1->altura)][(y + j) % (im1->largura)] = 255; //im2->red[i][j];
					im1->green[(x + i) % (im1->altura)][(y + j) % (im1->largura)] = 255; //im2->green[i][j];
					im1->blue[(x + i) % (im1->altura)][(y + j) % (im1->largura)] = 255; //im2->blue[i][j];
				}
			}
		}
	}

	return im1;
}

image *carimbarFrase(image *figura1, image *figura2, const char *frase, int x, int y, int larguraLetra, int alturaLetra)
{
	int i, j;
	char *indexes = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ";
	int len = strlen(frase);
	int imagemLargura = figura2->largura;
	int imagemAltura = figura2->altura;

	for (i = 0; i < len; i++)
	{
		char caractereFrase = frase[i];

		// Verifique se atingiu a borda direita da imagem
		if (x + larguraLetra > 780)
		{
			x = 0;			  // Volte para a coluna 0
			y += alturaLetra; // Mova para a próxima linha
		}

		if (y + alturaLetra > imagemAltura)
		{
			// A imagem está cheia, não podemos escrever mais
			break;
		}

		if (caractereFrase == ' ')
		{
			j = 62; // Índice para espaço em branco
		}
		else
		{
			for (j = 0; indexes[j] != '\0'; j++)
			{
				if (caractereFrase == indexes[j])
				{
					break;
				}
			}
		}

		figura2 = carimbo2(figura2, figura1, x, y, j);

		x += larguraLetra;
	}

	return figura2;
}

int main()
{

	int i, j;
	image *figura1;
	image *figura2;
	figura1 = load("impactfont.ppm");
	//figura1 = negativo(figura1);
	//save(figura1, "impactfont_negativo.ppm");
	figura2 = load("whenIdo.ppm");

	int count = 0;

	char *frase_superior = "oi";
	char *frase_inferior = "ta";

	int x = 20;
	int y_superior = 20;  // Ajuste conforme necessário
	int y_inferior = 500; // Ajuste conforme necessário
	int larguraLetra = 40;
	int alturaLetra = 80;

	// Adiciona o texto superior
	figura2 = carimbarFrase(figura1, figura2, frase_superior, x, y_superior, larguraLetra, alturaLetra);

	// Adiciona o texto inferior
	figura2 = carimbarFrase(figura1, figura2, frase_inferior, x, y_inferior, larguraLetra, alturaLetra);

	// image *watermark = load("watermark.ppm");
	// figura2 = carimbarWatermark(figura2, watermark, 10, 10);

	char nomeOutput[100];
	// snprintf(nomeOutput, sizeof(nomeOutput), "output%d.ppm");
	save(figura2, "meme.ppm");

	apaga(figura2);
	apaga(figura1);
	// apaga(watermark);

	return 0;
}