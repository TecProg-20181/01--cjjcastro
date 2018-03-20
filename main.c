#include <stdio.h>

#define RED 0
#define GREEN 1
#define BLUE 2

typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    unsigned short int pixel[512][512][3]; // [width][height][rgb]
    unsigned int width;
    unsigned int height;
} Image;


int max(int a, int b);
int pixel_igual(Pixel pixel_a, Pixel pixel_b);

Image escala_de_cinza(Image img);
Image sepia(Image img);
Image choose_rotate(Image img);
Image rotacionar_90_direita(Image img);
Image mirroring(Image img);
Image inverter_cores(Image img);
Image cortar_imagem(Image img);
Image read_image();

void blur(unsigned int height, unsigned short int pixel[512][512][3], unsigned int width);
void print_image(Image img);


int main() {
    Image img = read_image();

    
    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = escala_de_cinza(img);
                break;
            }
            case 2: { // Filtro Sepia
                img = sepia(img);
                break;
            }
            case 3: { // Blur
                blur(img.height, img.pixel, img.width);
                break;
            }
            case 4: { // Rotacao
                img = choose_rotate(img);
                break;
            }
            case 5: { // Espelhamento
                img = mirroring(img);
                break;
            }
            case 6: { // Inversao de Cores
                img = inverter_cores(img);
                break;
            }
            case 7: { // Cortar Imagem
                img = cortar_imagem(img);
                break;
            }
        }

    }

    print_image(img);

    return 0;
}


int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

int pixel_igual(Pixel pixel_a, Pixel pixel_b) {
    if (pixel_a.red == pixel_b.red &&
        pixel_a.green == pixel_b.green &&
        pixel_a.blue == pixel_b.blue)
        return 1;
    return 0;
}

Image choose_rotate(Image img){
    int quantas_vezes = 0;
    int aux_i;

    scanf("%d", &quantas_vezes);
    quantas_vezes %= 4;
    
    for (aux_i = 0; aux_i < quantas_vezes; ++aux_i) {
        img = rotacionar_90_direita(img);
    }
    return img;
}

// edit the image ----------------------------

Image escala_de_cinza(Image img) {
    /*for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            print("%u", img.pixel[i][j][0] + img.pixel[i][j][1] + img.pixel[i][j][2]);
        }
    }*/

    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int media = img.pixel[i][j][RED] +
                        img.pixel[i][j][GREEN] +
                        img.pixel[i][j][BLUE];
            media /= 3;
            img.pixel[i][j][RED] = media;
            img.pixel[i][j][GREEN] = media;
            img.pixel[i][j][BLUE] = media;
        }
    }

    return img;
}

void blur(unsigned int height, unsigned short int pixel[512][512][3], unsigned int width) {
    int tamanho = 0;
    scanf("%d", &tamanho);

    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            Pixel media = {0, 0, 0};

            int menor_h = (height - 1 > i + tamanho/2) ? i + tamanho/2 : height - 1;
            int min_w = (width - 1 > j + tamanho/2) ? j + tamanho/2 : width - 1;
            for(int x = (0 > i - tamanho/2 ? 0 : i - tamanho/2); x <= menor_h; ++x) {
                for(int y = (0 > j - tamanho/2 ? 0 : j - tamanho/2); y <= min_w; ++y) {
                    media.red += pixel[x][y][RED];
                    media.green += pixel[x][y][GREEN];
                    media.blue += pixel[x][y][BLUE];
                }
            }

            // printf("%u", media.red)
            media.red /= tamanho * tamanho;
            media.green /= tamanho * tamanho;
            media.blue /= tamanho * tamanho;

            pixel[i][j][RED] = media.red;
            pixel[i][j][GREEN] = media.green;
            pixel[i][j][BLUE] = media.blue;
        }
    }
}

Image rotacionar_90_direita(Image img) {
    Image rotacionada;

    rotacionada.width = img.height;
    rotacionada.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotacionada.height; ++i, ++y) {
        for (int j = rotacionada.width - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j][RED] = img.pixel[x][y][RED];
            rotacionada.pixel[i][j][GREEN] = img.pixel[x][y][GREEN];
            rotacionada.pixel[i][j][BLUE] = img.pixel[x][y][BLUE];
        }
    }

    return rotacionada;
}

Image inverter_cores(Image img) {
    unsigned int aux_i, aux_j;

    for (aux_i = 0; aux_i < img.height; ++aux_i) {
        for (aux_j = 0; aux_j < img.width; ++aux_j) {
            img.pixel[aux_i][aux_j][RED] = 255 - img.pixel[aux_i][aux_j][RED];
            img.pixel[aux_i][aux_j][GREEN] = 255 - img.pixel[aux_i][aux_j][GREEN];
            img.pixel[aux_i][aux_j][BLUE] = 255 - img.pixel[aux_i][aux_j][BLUE];
        }
    }
    return img;
}

Image cortar_imagem(Image img) {
    Image img_cortada;
    int x, y;

    scanf("%d %d", &x, &y);
    scanf("%d %d", &img_cortada.width, &img_cortada.height);


    for(int i = 0; i < img_cortada.height; ++i) {
        for(int j = 0; j < img_cortada.width; ++j) {
            img_cortada.pixel[i][j][RED] = img.pixel[i + y][j + x][RED];
            img_cortada.pixel[i][j][GREEN] = img.pixel[i + y][j + x][GREEN];
            img_cortada.pixel[i][j][BLUE] = img.pixel[i + y][j + x][BLUE];
        }
    }

    return img_cortada;
}

Image sepia(Image img){
    unsigned short int pixel[3];
    unsigned int aux_i, aux_j;
    int p;
    int menor_r;

    for (aux_i = 0; aux_i < img.height; ++aux_i) {
        for (aux_j = 0; aux_j < img.width; ++aux_j) {
            
            pixel[RED] = img.pixel[aux_i][aux_j][RED];
            pixel[GREEN] = img.pixel[aux_i][aux_j][GREEN];
            pixel[BLUE] = img.pixel[aux_i][aux_j][BLUE];

            p =  pixel[RED] * .393 + pixel[GREEN] * .769 + pixel[BLUE] * .189;
            menor_r = (255 >  p) ? p : 255;
            img.pixel[aux_i][aux_j][RED] = menor_r;

            p =  pixel[RED] * .349 + pixel[GREEN] * .686 + pixel[BLUE] * .168;
            menor_r = (255 >  p) ? p : 255;
            img.pixel[aux_i][aux_j][GREEN] = menor_r;

            p =  pixel[RED] * .272 + pixel[GREEN] * .534 + pixel[BLUE] * .131;
            menor_r = (255 >  p) ? p : 255;
            img.pixel[aux_i][aux_j][BLUE] = menor_r;
        }
    }
    return img;
}

Image mirroring(Image img){
    int horizontal = 0;
    int w = img.width, h = img.height;

    scanf("%d", &horizontal);

    if (horizontal == 1) w /= 2;
    else h /= 2;

    for (int i2 = 0; i2 < h; ++i2) {
        for (int j = 0; j < w; ++j) {
            int x = i2, y = j;

            if (horizontal == 1) y = img.width - 1 - j;
            else x = img.height - 1 - i2;

            Pixel pixel_aux;
            pixel_aux.red = img.pixel[i2][j][0];
            pixel_aux.green = img.pixel[i2][j][1];
            pixel_aux.blue = img.pixel[i2][j][2];

            img.pixel[i2][j][0] = img.pixel[x][y][0];
            img.pixel[i2][j][1] = img.pixel[x][y][1];
            img.pixel[i2][j][2] = img.pixel[x][y][2];

            img.pixel[x][y][0] = pixel_aux.red;
            img.pixel[x][y][1] = pixel_aux.green;
            img.pixel[x][y][2] = pixel_aux.blue;
        }
    }
    return img;
}

// read and print image ----------------------------

Image read_image(){
    Image img;
    char p3[4];
    int max_color;
    unsigned int aux_i, aux_j;

    // read type of image
    scanf("%s", p3);

    // read width height and color of image
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    // read all pixels of image
    for (aux_i = 0; aux_i < img.height; ++aux_i) {
        for (aux_j = 0; aux_j < img.width; ++aux_j) {
            scanf("%hu %hu %hu", &img.pixel[aux_i][aux_j][0],
                                 &img.pixel[aux_i][aux_j][1],
                                 &img.pixel[aux_i][aux_j][2]);

        }
    }
    return img;
}

void print_image(Image img){
    unsigned int aux_i, aux_j;

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // print pixels of image
    for (aux_i = 0; aux_i < img.height; ++aux_i) {
        for (aux_j = 0; aux_j < img.width; ++aux_j) {
            printf("%hu %hu %hu ", img.pixel[aux_i][aux_j][0],
                                   img.pixel[aux_i][aux_j][1],
                                   img.pixel[aux_i][aux_j][2]);

        }
        printf("\n");
    }
}