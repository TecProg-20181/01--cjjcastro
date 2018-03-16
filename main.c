#include <stdio.h>

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> r
    // 1 -> g
    // 2 -> b
    unsigned short int pixel[512][512][3];
    unsigned int w;
    unsigned int h;
} Image;


int max(int a, int b);
int pixel_igual(Pixel p1, Pixel p2);

Image escala_de_cinza(Image img);
Image sepia(Image img);
Image rotacionar(Image img);
Image rotacionar90direita(Image img);
Image espelhamento(Image img);
Image inverter_cores(Image img);
Image cortar_imagem(Image img);
Image read_image();

void blur(unsigned int h, unsigned short int pixel[512][512][3], unsigned int w);
void print_image(Image img);


int main() {
    Image img;
    img = read_image();

    
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
                blur(img.h, img.pixel, img.w);
                break;
            }
            case 4: { // Rotacao
                img = rotacionar(img);
                break;
            }
            case 5: { // Espelhamento
                img = espelhamento(img);
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

int pixel_igual(Pixel p1, Pixel p2) {
    if (p1.r == p2.r &&
        p1.g == p2.g &&
        p1.b == p2.b)
        return 1;
    return 0;
}


Image escala_de_cinza(Image img) {
    /*for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            print("%u", img.pixel[i][j][0] + img.pixel[i][j][1] + img.pixel[i][j][2]);
        }
    }*/

    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            int media = img.pixel[i][j][0] +
                        img.pixel[i][j][1] +
                        img.pixel[i][j][2];
            media /= 3;
            img.pixel[i][j][0] = media;
            img.pixel[i][j][1] = media;
            img.pixel[i][j][2] = media;
        }
    }

    return img;
}

void blur(unsigned int h, unsigned short int pixel[512][512][3], unsigned int w) {
    int tamanho = 0;
    scanf("%d", &tamanho);

    for (unsigned int i = 0; i < h; ++i) {
        for (unsigned int j = 0; j < w; ++j) {
            Pixel media = {0, 0, 0};

            int menor_h = (h - 1 > i + tamanho/2) ? i + tamanho/2 : h - 1;
            int min_w = (w - 1 > j + tamanho/2) ? j + tamanho/2 : w - 1;
            for(int x = (0 > i - tamanho/2 ? 0 : i - tamanho/2); x <= menor_h; ++x) {
                for(int y = (0 > j - tamanho/2 ? 0 : j - tamanho/2); y <= min_w; ++y) {
                    media.r += pixel[x][y][0];
                    media.g += pixel[x][y][1];
                    media.b += pixel[x][y][2];
                }
            }

            // printf("%u", media.r)
            media.r /= tamanho * tamanho;
            media.g /= tamanho * tamanho;
            media.b /= tamanho * tamanho;

            pixel[i][j][0] = media.r;
            pixel[i][j][1] = media.g;
            pixel[i][j][2] = media.b;
        }
    }
}

Image rotacionar(Image img){
    int quantas_vezes = 0;
    int aux_i;

    scanf("%d", &quantas_vezes);
    quantas_vezes %= 4;
    
    for (aux_i = 0; aux_i < quantas_vezes; ++aux_i) {
        img = rotacionar90direita(img);
    }
    return img;
}

Image rotacionar90direita(Image img) {
    Image rotacionada;

    rotacionada.w = img.h;
    rotacionada.h = img.w;

    for (unsigned int i = 0, y = 0; i < rotacionada.h; ++i, ++y) {
        for (int j = rotacionada.w - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j][0] = img.pixel[x][y][0];
            rotacionada.pixel[i][j][1] = img.pixel[x][y][1];
            rotacionada.pixel[i][j][2] = img.pixel[x][y][2];
        }
    }

    return rotacionada;
}

Image inverter_cores(Image img) {
    unsigned int aux_i, aux_j;

    for (aux_i = 0; aux_i < img.h; ++aux_i) {
        for (aux_j = 0; aux_j < img.w; ++aux_j) {
            img.pixel[aux_i][aux_j][0] = 255 - img.pixel[aux_i][aux_j][0];
            img.pixel[aux_i][aux_j][1] = 255 - img.pixel[aux_i][aux_j][1];
            img.pixel[aux_i][aux_j][2] = 255 - img.pixel[aux_i][aux_j][2];
        }
    }
    return img;
}

Image cortar_imagem(Image img) {
    Image img_cortada;
    int x, y;

    scanf("%d %d", &x, &y);
    scanf("%d %d", &img_cortada.w, &img_cortada.h);


    for(int i = 0; i < img_cortada.h; ++i) {
        for(int j = 0; j < img_cortada.w; ++j) {
            img_cortada.pixel[i][j][0] = img.pixel[i + y][j + x][0];
            img_cortada.pixel[i][j][1] = img.pixel[i + y][j + x][1];
            img_cortada.pixel[i][j][2] = img.pixel[i + y][j + x][2];
        }
    }

    return img_cortada;
}

Image sepia(Image img){
    for (unsigned int x = 0; x < img.h; ++x) {
        for (unsigned int j = 0; j < img.w; ++j) {
            unsigned short int pixel[3];
            pixel[0] = img.pixel[x][j][0];
            pixel[1] = img.pixel[x][j][1];
            pixel[2] = img.pixel[x][j][2];

            int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
            int menor_r = (255 >  p) ? p : 255;
            img.pixel[x][j][0] = menor_r;

            p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
            menor_r = (255 >  p) ? p : 255;
            img.pixel[x][j][1] = menor_r;

            p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
            menor_r = (255 >  p) ? p : 255;
            img.pixel[x][j][2] = menor_r;
        }
    }
    return img;
}

Image read_image(){
    Image img;
    char p3[4];
    int max_color;
    unsigned int aux_i;
    unsigned int aux_j;

    // read type of image
    scanf("%s", p3);

    // read width height and color of image
    scanf("%u %u %d", &img.w, &img.h, &max_color);

    // read all pixels of image
    for (aux_i = 0; aux_i < img.h; ++aux_i) {
        for (aux_j = 0; aux_j < img.w; ++aux_j) {
            scanf("%hu %hu %hu", &img.pixel[aux_i][aux_j][0],
                                 &img.pixel[aux_i][aux_j][1],
                                 &img.pixel[aux_i][aux_j][2]);

        }
    }
    return img;
}

Image espelhamento(Image img){
    int horizontal = 0;
    int w = img.w, h = img.h;

    scanf("%d", &horizontal);

    if (horizontal == 1) w /= 2;
    else h /= 2;

    for (int i2 = 0; i2 < h; ++i2) {
        for (int j = 0; j < w; ++j) {
            int x = i2, y = j;

            if (horizontal == 1) y = img.w - 1 - j;
            else x = img.h - 1 - i2;

            Pixel aux1;
            aux1.r = img.pixel[i2][j][0];
            aux1.g = img.pixel[i2][j][1];
            aux1.b = img.pixel[i2][j][2];

            img.pixel[i2][j][0] = img.pixel[x][y][0];
            img.pixel[i2][j][1] = img.pixel[x][y][1];
            img.pixel[i2][j][2] = img.pixel[x][y][2];

            img.pixel[x][y][0] = aux1.r;
            img.pixel[x][y][1] = aux1.g;
            img.pixel[x][y][2] = aux1.b;
        }
    }
    return img;
}

void print_image(Image img){
    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.w, img.h);

    // print pixels of image
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j][0],
                                   img.pixel[i][j][1],
                                   img.pixel[i][j][2]);

        }
        printf("\n");
    }
}