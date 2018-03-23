#include <stdio.h>


typedef struct _pixel
{
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image
{
    Pixel pixel[512][512]; // [width][height]
    unsigned int width;
    unsigned int height;
} Image;


int max(int a, int b);
int pixel_igual(Pixel pixel_a, Pixel pixel_b);
unsigned short int average(Pixel pixel);

Image grey_scale(Image img);
Image sepia(Image img);
Image choose_rotate(Image img);
Image rotate_90_right(Image img);
Image mirroring(Image img);
Image invert_colors(Image img);
Image crop_image(Image img);
Image read_image();

void blur(Image img, Pixel pixel[512][512]);
void print_image(Image img);

Pixel invert_pixel(Pixel pixel);
Pixel pixel_average(Pixel pixel);
Pixel increment_pixel(Pixel media, Pixel pixel);
Pixel div_pixel(Pixel media, int size);


int main()
{
    Image img = read_image();

    
    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i)
    {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao)
        {
            case 1:
            {   // Grey scale
                img = grey_scale(img);
                break;
            }
            case 2:
            {   // Sepia filter
                img = sepia(img);
                break;
            }
            case 3:
            {   // Blur
                blur(img, img.pixel);
                break;
            }
            case 4: 
            {   // Rotate
                img = choose_rotate(img);
                break;
            }
            case 5:
            {   // Mirroring
                img = mirroring(img);
                break;
            }
            case 6:
            {   // Color inversion
                img = invert_colors(img);
                break;
            }
            case 7: 
            {   // Crop image
                img = crop_image(img);
                break;
            }
        }
    }

    print_image(img);

    return 0;
}

// auxiliary functions -------------------------

Image choose_rotate(Image img)
{
    int repetitions = 0;
    int aux_i;

    scanf("%d", &repetitions);
    repetitions %= 4;
    
    for (aux_i = 0; aux_i < repetitions; ++aux_i) 
    {
        img = rotate_90_right(img);
    }
    return img;
}

Pixel invert_pixel(Pixel pixel)
{
    
    pixel.red = 255 - pixel.red;
    pixel.green = 255 - pixel.green;
    pixel.blue = 255 - pixel.blue;

    return pixel;
}

unsigned short int average(Pixel pixel)
{
    return (pixel.red + pixel.green + pixel.blue)/3;
}

Pixel pixel_average(Pixel pixel)
{
    Pixel pixel_aux;

    pixel_aux.red = average(pixel);
    pixel_aux.green = average(pixel);
    pixel_aux.blue = average(pixel);

    return pixel_aux;
}

Pixel increment_pixel(Pixel media, Pixel pixel)
{
    media.red += pixel.red;
    media.green += pixel.green;
    media.blue += pixel.blue;

    return media;
}

Pixel div_pixel(Pixel media, int size)
{
    media.red /= size;
    media.green /= size;
    media.blue /= size;

    return media;
}
// edit the image ----------------------------

Image grey_scale(Image img)
{
    unsigned int aux_i, aux_j;

    for (aux_i = 0; aux_i < img.height; ++aux_i)
    {
        for (aux_j = 0; aux_j < img.width; ++aux_j)
        {
            img.pixel[aux_i][aux_j] = pixel_average(img.pixel[aux_i][aux_j]);
        }
    }

    return img;
}

void blur(Image img, Pixel pixel[512][512])
{
    int size = 0;
    scanf("%d", &size);

    for (unsigned int i = 0; i < img.height; ++i)
    {
        for (unsigned int j = 0; j < img.width; ++j)
        {
            Pixel media = {0, 0, 0};

            int menor_h = (img.height - 1 > i + size/2) ? i + size/2 : img.height - 1;
            int min_w = (img.width - 1 > j + size/2) ? j + size/2 : img.width - 1;
            for(int x = (0 > i - size/2 ? 0 : i - size/2); x <= menor_h; x++)
            {
                for(int y = (0 > j - size/2 ? 0 : j - size/2); y <= min_w; y++)
                {
                    media = increment_pixel(media, pixel[x][y]);
                }
            }

            media = div_pixel(media, size * size);
            
            pixel[i][j] = media;
        }
    }
}

Image rotate_90_right(Image img)
{
    Image rotated;

    rotated.width = img.height;
    rotated.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotated.height; ++i, ++y)
    {
        for (int j = rotated.width - 1, x = 0; j >= 0; --j, ++x)
        {
            rotated.pixel[i][j] = img.pixel[x][y];
        }
    }

    return rotated;
}

Image invert_colors(Image img) 
{
    unsigned int aux_i, aux_j;

    for (aux_i = 0; aux_i < img.height; ++aux_i) 
    {
        for (aux_j = 0; aux_j < img.width; ++aux_j) 
        {
            img.pixel[aux_i][aux_j] = invert_pixel(img.pixel[aux_i][aux_j]);
        }
    }
    return img;
}

Image crop_image(Image img)
{
    Image img_cropped;
    int x, y;
    int aux_i, aux_j;

    scanf("%d %d", &x, &y);
    scanf("%d %d", &img_cropped.width, &img_cropped.height);


    for(aux_i = 0; aux_i < img_cropped.height; ++aux_i)
    {
        for(aux_j = 0; aux_j < img_cropped.width; ++aux_j)
        {
            img_cropped.pixel[aux_i][aux_j] = img.pixel[aux_i + y][aux_j + x];
        }
    }

    return img_cropped;
}

Image sepia(Image img)
{
    Pixel pixel;
    unsigned int aux_i, aux_j;
    int p;
    int menor_r;

    for (aux_i = 0; aux_i < img.height; ++aux_i)
    {
        for (aux_j = 0; aux_j < img.width; ++aux_j)
        {    
            pixel = img.pixel[aux_i][aux_j];
            
            p =  pixel.red * .393 + pixel.green * .769 + pixel.blue * .189;
            menor_r = (255 >  p) ? p : 255;
            img.pixel[aux_i][aux_j].red = menor_r;

            p =  pixel.red * .349 + pixel.green * .686 + pixel.blue * .168;
            menor_r = (255 >  p) ? p : 255;
            img.pixel[aux_i][aux_j].green = menor_r;

            p =  pixel.red * .272 + pixel.green * .534 + pixel.blue * .131;
            menor_r = (255 >  p) ? p : 255;
            img.pixel[aux_i][aux_j].blue = menor_r;
        }
    }
    return img;
}

Image mirroring(Image img)
{
    int horizontal = 0;
    int width = img.width, height = img.height;
    int aux_i, aux_j;
    Pixel pixel_aux;

    scanf("%d", &horizontal);

    if (horizontal == 1) 
        width /= 2;
    else 
        height /= 2;

    for (aux_i = 0; aux_i < height; ++aux_i)
    {
        for (aux_j = 0; aux_j < width; ++aux_j)
        {
            int x = aux_i, y = aux_j;

            if (horizontal == 1)
                y = img.width - 1 - aux_j;
            else
                x = img.height - 1 - aux_i;

            pixel_aux = img.pixel[aux_i][aux_j];

            img.pixel[aux_i][aux_j] = img.pixel[x][y];

            img.pixel[x][y] = pixel_aux;
        }
    }
    return img;
}

// read and print image ----------------------------

Image read_image()
{
    Image img;
    char p3[4];
    int max_color;
    unsigned int aux_i, aux_j;

    // read type of image
    scanf("%s", p3);

    // read width height and color of image
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    // read all pixels of image
    for (aux_i = 0; aux_i < img.height; ++aux_i)
    {
        for (aux_j = 0; aux_j < img.width; ++aux_j)
        {
            scanf("%hu %hu %hu", &img.pixel[aux_i][aux_j].red,
                                 &img.pixel[aux_i][aux_j].green,
                                 &img.pixel[aux_i][aux_j].blue);

        }
    }
    return img;
}

void print_image(Image img)
{
    unsigned int aux_i, aux_j;

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // print pixels of image
    for (aux_i = 0; aux_i < img.height; ++aux_i)
    {
        for (aux_j = 0; aux_j < img.width; ++aux_j)
        {
            printf("%hu %hu %hu ", img.pixel[aux_i][aux_j].red,
                                   img.pixel[aux_i][aux_j].green,
                                   img.pixel[aux_i][aux_j].blue);
        }
        printf("\n");
    }
}