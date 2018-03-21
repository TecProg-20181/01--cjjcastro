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

Image grey_scale(Image img);
Image sepia(Image img);
Image choose_rotate(Image img);
Image rotate_90_right(Image img);
Image mirroring(Image img);
Image invert_colors(Image img);
Image crop_image(Image img);
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
            case 1: { // Grey scale
                img = grey_scale(img);
                break;
            }
            case 2: { // Sepia filter
                img = sepia(img);
                break;
            }
            case 3: { // Blur
                blur(img.height, img.pixel, img.width);
                break;
            }
            case 4: { // Rotate
                img = choose_rotate(img);
                break;
            }
            case 5: { // Mirroring
                img = mirroring(img);
                break;
            }
            case 6: { // Color inversion
                img = invert_colors(img);
                break;
            }
            case 7: { // Crop image
                img = crop_image(img);
                break;
            }
        }
    }

    print_image(img);

    return 0;
}

// auxiliary functions -------------------------

/*int max(int a, int b) {
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
}*/

Image choose_rotate(Image img){
    int repetitions = 0;
    int aux_i;

    scanf("%d", &repetitions);
    repetitions %= 4;
    
    for (aux_i = 0; aux_i < repetitions; ++aux_i) {
        img = rotate_90_right(img);
    }
    return img;
}

// edit the image ----------------------------

Image grey_scale(Image img) {
    int average;
    unsigned int aux_i, aux_j;

    for (aux_i = 0; aux_i < img.height; ++aux_i) {
        for (aux_j = 0; aux_j < img.width; ++aux_j) {
            average = img.pixel[aux_i][aux_j][RED] +
                        img.pixel[aux_i][aux_j][GREEN] +
                        img.pixel[aux_i][aux_j][BLUE];
            average /= 3;
            img.pixel[aux_i][aux_j][RED] = average;
            img.pixel[aux_i][aux_j][GREEN] = average;
            img.pixel[aux_i][aux_j][BLUE] = average;
        }
    }

    return img;
}

void blur(unsigned int height, unsigned short int pixel[512][512][3], unsigned int width) {
    int size = 0;
    scanf("%d", &size);

    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            Pixel media = {0, 0, 0};

            int menor_h = (height - 1 > i + size/2) ? i + size/2 : height - 1;
            int min_w = (width - 1 > j + size/2) ? j + size/2 : width - 1;
            for(int x = (0 > i - size/2 ? 0 : i - size/2); x <= menor_h; ++x) {
                for(int y = (0 > j - size/2 ? 0 : j - size/2); y <= min_w; ++y) {
                    media.red += pixel[x][y][RED];
                    media.green += pixel[x][y][GREEN];
                    media.blue += pixel[x][y][BLUE];
                }
            }

            // printf("%u", media.red)
            media.red /= size * size;
            media.green /= size * size;
            media.blue /= size * size;

            pixel[i][j][RED] = media.red;
            pixel[i][j][GREEN] = media.green;
            pixel[i][j][BLUE] = media.blue;
        }
    }
}

Image rotate_90_right(Image img) {
    Image rotated;

    rotated.width = img.height;
    rotated.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotated.height; ++i, ++y) {
        for (int j = rotated.width - 1, x = 0; j >= 0; --j, ++x) {
            rotated.pixel[i][j][RED] = img.pixel[x][y][RED];
            rotated.pixel[i][j][GREEN] = img.pixel[x][y][GREEN];
            rotated.pixel[i][j][BLUE] = img.pixel[x][y][BLUE];
        }
    }

    return rotated;
}

Image invert_colors(Image img) {
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

Image crop_image(Image img) {
    Image img_cropped;
    int x, y;
    int aux_i, aux_j;

    scanf("%d %d", &x, &y);
    scanf("%d %d", &img_cropped.width, &img_cropped.height);


    for(aux_i = 0; aux_i < img_cropped.height; ++aux_i) {
        for(aux_j = 0; aux_j < img_cropped.width; ++aux_j) {
            img_cropped.pixel[aux_i][aux_j][RED] = img.pixel[aux_i + y][aux_j + x][RED];
            img_cropped.pixel[aux_i][aux_j][GREEN] = img.pixel[aux_i + y][aux_j + x][GREEN];
            img_cropped.pixel[aux_i][aux_j][BLUE] = img.pixel[aux_i + y][aux_j + x][BLUE];
        }
    }

    return img_cropped;
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
    int width = img.width, height = img.height;
    int aux_i, aux_j;
    Pixel pixel_aux;

    scanf("%d", &horizontal);

    if (horizontal == 1) width /= 2;
    else height /= 2;

    for (aux_i = 0; aux_i < height; ++aux_i) {
        for (aux_j = 0; aux_j < width; ++aux_j) {
            int x = aux_i, y = aux_j;

            if (horizontal == 1) y = img.width - 1 - aux_j;
            else x = img.height - 1 - aux_i;

            pixel_aux.red = img.pixel[aux_i][aux_j][RED];
            pixel_aux.green = img.pixel[aux_i][aux_j][GREEN];
            pixel_aux.blue = img.pixel[aux_i][aux_j][BLUE];

            img.pixel[aux_i][aux_j][RED] = img.pixel[x][y][RED];
            img.pixel[aux_i][aux_j][GREEN] = img.pixel[x][y][GREEN];
            img.pixel[aux_i][aux_j][BLUE] = img.pixel[x][y][BLUE];

            img.pixel[x][y][RED] = pixel_aux.red;
            img.pixel[x][y][GREEN] = pixel_aux.green;
            img.pixel[x][y][BLUE] = pixel_aux.blue;
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