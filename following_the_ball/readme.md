# Following The Ball - an interactive app with OpenGL

- Manoela Resende

Este exercício é propriedade de Manoela Resende, perfil [manoelargc](https://github.com/manoelargc). Ele foi desenvolvido como entrega da avaliação final da disciplina de Computação Gráfica do 4º Semestre de Ciência da Computação - 23.2.
<p align="center">
  <img src="https://github.com/manoelargc/CG/assets/105336239/08c15df5-a24e-4012-953c-626e456884e4" width="500">
</p>

## Descrição
Este é um programa OpenGL que carrega e exibe objetos 3D a partir de arquivos `.obj`. A aplicação permite a interação com a cena, movimentação da câmera, zoom e rotação dos objetos.

## Uso
1. Compile o código usando um compilador C++ com suporte a OpenGL.
2. Certifique-se de ter as bibliotecas GLUT instaladas.
3. Coloque os arquivos `.obj` desejados na mesma pasta do executável.
4. Execute o programa resultante.

## Recursos

- **Carregamento de Objetos:** A aplicação pode carregar arquivos Wavefront (.obj) para exibir objetos 3D.
  
- **Controle de Câmera:** Use callbacks de teclado e mouse para controlar a visualização da câmera.
  
- **Manipulação de Objetos:** Gire, translate e escale os objetos carregados interativamente.

## Primeiros Passos

### Pré-requisitos

Certifique-se de ter as bibliotecas necessárias instaladas:

```bash
sudo apt-get install freeglut3-dev
```

### Compilação

Compile o código usando g++:

```bash
g++ followingTheBall.cpp -o app -lGL -lGLU -lglut -lm
```

### Execução

Execute o executável compilado:

```bash
./app
```

## Controles

- **W:** Mover câmera para cima
- **S:** Mover câmera para baixo
- **A:** Mover câmera para a esquerda
- **D:** Mover câmera para a direita
- **Setas:** Ajustar ângulos da câmera
- **Teclas 1 e 2:** Realizam o zoom out e zoom in, respectivamente.

## Carregamento de Arquivos

A aplicação carrega dois objetos: "ball.obj" e "elephant.obj". Você pode substituí-los por seus próprios objetos Wavefront.

## Notas Adicionais

- O programa inclui configurações de iluminação e sombreamento para melhorar a aparência visual dos objetos.
  
- A aplicação utiliza timers para atualizar continuamente e renderizar a cena.

-----------
## Organização do Código

### 1. Variáveis Globais
  - Contém variáveis globais que controlam o estado do programa, como posição da câmera, velocidade de movimento e configurações de luz.

### 2. Estruturas de Dados
- **`cameraView`:**
  - Estrutura para armazenar os parâmetros da câmera.
- **`coordenada`:**
  - Estrutura para armazenar as coordenadas X,Y,Z

### 3. Classe `myobj`
- Encapsula funcionalidades relacionadas ao carregamento e manipulação de objetos 3D a partir de arquivos `.obj`.

### 4. Funções Globais
- **`facedraw(myobj &obj)`:**
  - Desenha as faces do objeto.

- **`timer(int value)`:**
  - Atualizações a serem feitas no timer.

- **`display()`:**
  - Função principal de exibição.

- **`myReshape(int w, h)`:**
  - Função chamada quando a janela é redimensionada.

- **`myInit()`:**
  - Inicializa configurações iniciais.

- **`handleKeypress(unsigned char key, int x, int y)`:**
  - Manipula pressionamentos de teclas.

- **`processSpecialKeys(int key, int x, int y)`:**
  - Manipula teclas especiais.

- **`setInitialObjectPosition()`:**
  - Define as posições iniciais dos objetos.

### 5. Função `main`
- Função principal que inicializa o programa, configura as janelas GLUT, carrega os objetos, inicializa o ambiente e estabelece as funções de callback.

----
