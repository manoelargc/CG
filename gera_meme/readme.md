# Gerador de Memes

- Manoela Resende

Este exercício é propriedade de Manoela Resende, perfil [manoelargc](https://github.com/manoelargc). Ele foi desenvolvido como entrega pré-avaliação da disciplina de Computação Gráfica do 4º Semestre de Ciência da Computação - 23.2.

## Descrição

Este é um programa em C que permite criar memes simples adicionando texto a uma imagem. O programa recebe uma imagem base, coordenadas (x, y) e uma string de texto. Ele carimba o texto na imagem, adicionando uma borda escura e sombra para tornar o meme mais legível e, em seguida, salva o resultado como uma imagem PPM.

## Uso

Para usar o programa, siga as etapas abaixo:

1. Certifique-se de que você tenha as imagens de fonte e de meme no mesmo diretório do programa.

2. Modifique as variáveis `frase_sup_L1`, `frase_sup_L2`, `frase_inf_L1`, e `frase_inf_L2` com as linhas de texto que você deseja adicionar ao meme.

3. Ajuste as variáveis `x`, `y_sup_L1`, `y_sup_L2`, `y_inf_L1`, `y_inf_L2`, `espacamento_horizontal` e `espacamento_vertical` conforme necessário para posicionar o texto no meme.

4. Compile e execute o programa.

5. O meme gerado será salvo como "meme.ppm" no mesmo diretório do programa.

## Observações

- A função `carimbarFrase` é usada para adicionar texto à imagem. Ela quebra a string de texto em caracteres e carimba cada caractere individualmente na imagem de meme.

- O programa suporta letras maiúsculas, letras minúsculas, números e espaço em branco.

- A imagem de fonte utilizada é ["impactfont.ppm"](https://github.com/manoelargc/CG/blob/main/output/impactfont.ppm) e a imagem de meme base é ["whenIdo.ppm"](https://github.com/manoelargc/CG/blob/main/output/whenIdo.ppm).

- O resultado final terá uma borda escura e sombra nas letras para tornar o texto mais visível.

- Certifique-se de que as imagens de fonte e meme estejam no formato P3 PPM.

Lembre-se de ajustar as configurações conforme necessário para criar memes personalizados. Divirta-se criando memes!

Exemplo utilizado:

![Exemplo](https://github.com/manoelargc/CG/blob/main/output/meme.png)

