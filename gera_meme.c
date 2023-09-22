#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct image
{
    int largura;
    int altura;
    int** red;
    int** green;
    int** blue;

}image;

image* inicializa(image* imagem,int largura, int altura)
{
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

image* apaga(image* imagem)
{
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


image* save(image* imagem, char nome[80] )
{
	
	FILE* arq;
	arq=fopen(nome,"wt");
    int i,j;
    fprintf(arq,"P3\n%d %d\n255\n",imagem->largura, imagem->altura);
    for (i = 0; i <  imagem->altura; i++)
    {
        for (j = 0; j < imagem->largura; j++)
        {
            fprintf(arq,"%d %d %d ",imagem->red[i][j],imagem->green[i][j],imagem->blue[i][j]);

        }
        fprintf (arq,"\n");
    }
	
	fclose(arq);
	
}


image* load(char nome[80])
{
	
	FILE* arq;
	
	char linha1[100];
    int i,j,width,height,maxcolor;
	image* buffer;
	
	arq=fopen(nome,"rt");
	//fgets(linha1,sizeof linha1, arq);
	fgets(linha1,sizeof linha1, arq);
	fscanf(arq,"%i %i",&width,&height);
	fscanf(arq,"%i",&maxcolor);
	
	buffer = malloc(sizeof(image));
	
	buffer = inicializa(buffer,width, height);

    for (i = 0; i <  buffer->altura; i++)
    {
        for (j = 0; j < buffer->largura; j++)
        {
            fscanf(arq,"%i",&(buffer->red[i][j]));
			fscanf(arq,"%i",&(buffer->green[i][j]));
			fscanf(arq,"%i",&(buffer->blue[i][j]));
        }
            }
	
	fclose(arq);
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


	return im1;
		
}

image* carimbo2(image* im1, image* im2,int y, int x,int c){
	
    int digit=0;
	int i,j,col,flag,currcol,gap;
	col=0;
	currcol=0;
	
	while(digit!=c){
	gap=0;
	for (j=col;j<im2->largura,gap==0;j++)
	    for (i=0;i<im2->altura;i++)
		{
			if(im2->red[i][j]==0 && im2->green[i][j] ==0 && im2->blue[i][j]==0){
			col=j;
			gap=1;
			break;
		}
		}		
		
	flag=1;
	for (j=col;j<im2->largura,flag==1;j++){
	flag=0;	
	   currcol=j;
	   for (i=0;i<im2->altura;i++){	
		{
			if(im2->red[i][j]==0 && im2->green[i][j] ==0 && im2->blue[i][j]==0){
			flag=1;
		
			}
		}	
	}
	}
	digit++;
	col=currcol;
	}
	gap=0;
	for (j=col;j<im2->largura,gap==0;j++)
	    for (i=0;i<im2->altura;i++)
		{
			if(im2->red[i][j]==0 && im2->green[i][j] ==0 && im2->blue[i][j]==0){
			col=j;
			gap=1;
			break;
		}
		}

	flag=1;
	for (j=col;j<im2->largura,flag==1;j++){
	flag=0;	
	   for (i=0;i<im2->altura;i++){	
		{
			if(im2->red[i][j]==0 && im2->green[i][j] ==0 && im2->blue[i][j]==0){
			flag=1;
			im1->red[(x+i)%(im1->altura)][(y+j)%(im1->largura)]=im2->red[i][j];
			im1->green[(x+i)%(im1->altura)][(y+j)%(im1->largura)]=im2->green[i][j];
			im1->blue[(x+i)%(im1->altura)][(y+j)%(im1->largura)]=im2->blue[i][j];
			}
		}	
	}
	}

	return im1;
		
}




image* criarMeme(image* imagem, int x_topo, int y_topo, char* texto_topo, int x_bottom, int y_bottom, char* texto_bottom) {
    // Verifica se as coordenadas estão dentro dos limites da imagem
    if (x_topo >= imagem->altura || x_bottom >= imagem->altura || y_topo >= imagem->largura || y_bottom >= imagem->largura) {
        printf("Coordenadas fora dos limites da imagem.\n");
        return imagem;
    }

    // Adicionar texto superior
    int len_topo = strlen(texto_topo);
    for (int i = 0; i < len_topo; i++) {
        char character = texto_topo[i];
        if (character == ' ') {
            y_topo += 200; // Espaço entre as letras
        } else {
            // Caracteres diferentes de espaço
            image* temp = load("impactfont.ppm"); // Carregue um arquivo de fonte de letras predefinido
            int char_index = character - 'A'; // Supondo que os caracteres estejam em ordem alfabética A-Z no arquivo de fonte

            // Carimbe o caractere na imagem principal
            imagem = carimbo(imagem, temp, x_topo, y_topo + i * 1); // Espaço entre as letras
            imagem = carimbo2(imagem, temp, x_topo, y_topo + i * 200, char_index);
            temp = apaga(temp);
        }
    }

    // Adicionar texto inferior
    int len_bottom = strlen(texto_bottom);
    for (int i = 0; i < len_bottom; i++) {
        char character = texto_bottom[i];
        if (character == ' ') {
            y_bottom += 15; // Espaço entre as letras
        } else {
            // Caracteres diferentes de espaço
            image* temp = load("impactfont.ppm"); // Carregue um arquivo de fonte de letras predefinido
            int char_index = character - 'A'; // Supondo que os caracteres estejam em ordem alfabética A-Z no arquivo de fonte

            // Carimbe o caractere na imagem principal
            imagem = carimbo(imagem, temp, x_bottom, y_bottom + i * 15); // Espaço entre as letras
            imagem = carimbo2(imagem, temp, x_bottom, y_bottom + i * 15, char_index);
            temp = apaga(temp);
        }
    }

    return imagem;
}

int main() {

    int i,j,col;
	image* figura1;
	image* figura2;
	figura1 = load("impactfont.ppm");
	figura2 = load("whenIdo.ppm");

/* 	char* up;
	char* bottom;
	char* indexes;
	up=malloc(17);
	bottom=malloc(27);
	indexes=malloc(100);
	strcpy(indexes,"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ");
	strcpy(up,"I do not always code memes");
	strcpy(bottom,"But when I do it, I do in C");
	
	
	figura2=carimbo2(figura2,figura1,10, 10,23);
	figura2=carimbo2(figura2,figura1,50, 10,20);
	figura2=carimbo2(figura2,figura1,100, 10,13);
	 */
	
/* 
    char* texto_superior[100];
    char* texto_inferior[100];

    printf("Digite o texto superior: ");
    fgets(texto_superior, sizeof(texto_superior), stdin);
    printf("Digite o texto inferior: ");
    fgets(texto_inferior, sizeof(texto_inferior), stdin);

    // Remova a quebra de linha do final das strings
    texto_superior[strcspn(texto_superior, "\n")] = '\0';
    texto_inferior[strcspn(texto_inferior, "\n")] = '\0'; */

	
	int count = 0;
	char* up;
	char* bottom;
	//char* indexes;
	up=malloc(17);
	bottom=malloc(27);
	//indexes=malloc(100);
	//strcpy(indexes,"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ");
	//strcpy(up,"I do not always code memes");
	//strcpy(bottom,"But when I do it, I do in C");
	strcpy(up, "A ");
	

    // Crie a imagem do meme com os textos carimbados
    figura2 = criarMeme(figura2, 1, 1, up, 1, 1, up); 

    // Salve o resultado como uma imagem PPM
    save(figura2, "meme.ppm");
	free(up);
	free(bottom);


    return 0;
}
