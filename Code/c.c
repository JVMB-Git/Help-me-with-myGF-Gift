#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "stb_image.h"

#include "stb_image_write.h"


// Define RGB struct for pixel representation
typedef struct {
    double r;
    double g;
    double b;
} RGB;

// Define Image struct to hold image data
typedef struct {
    int width;
    int height;
    RGB** data;
} Image;

// STB functions for loading and saving images
extern unsigned char* stbi_load(char const* filename, int* x, int* y, int* comp, int req_comp);
extern int stbi_write_png(char const* filename, int w, int h, int comp, const void* data, int stride_in_bytes);

// Function to create an empty image
Image* create_image(int width, int height) {
    Image* image = (Image*)malloc(sizeof(Image));
    image->width = width;
    image->height = height;
    image->data = (RGB**)malloc(height * sizeof(RGB*));
    
    for (int i = 0; i < height; i++) {
        image->data[i] = (RGB*)malloc(width * sizeof(RGB));
    }
    
    return image;
}

// Function to destroy an image and free allocated memory
void destroy_image(Image* image) {
    for (int i = 0; i < image->height; i++) {
        free(image->data[i]);
    }
    
    free(image->data);
    free(image);
}

// Function to calculate the "coeur" value
double coeur(double x, double y, double s) {
    return pow(x / s, 2) + 2 * (pow(3.0 / 5 * pow(x / s, 2), 1.0 / 3) - (y + 1.0 / 8) / s) * (y + 1.0 / 8) / s - 1;
}

// Function to apply the "stencil" effect to an image
Image* stencil(Image* image, Image* back, double scale) {
    int w = image->width;
    int h = image->height;
    Image* pixels = create_image(w, h);

    for (int j = 0; j < w; j++) {
        for (int i = 0; i < h; i++) {
            double u = 2.0 * ((double)i / (h - 1)) - 1.0;
            double v = -2.0 * ((double)j / (w - 1)) + 1;
            double value = coeur(u, v, scale);
            
            if (value <= 0) {
                pixels->data[j][i] = image->data[j][i];
            } else {
                pixels->data[j][i] = back->data[j][i];
            }
        }
    }

    return pixels;
}

// Function to load an image from a file
Image* load_image(char* path) {
    int width, height, channels;
    unsigned char* img_data = stbi_load(path, &width, &height, &channels, 3); // 3 channels for RGB

    if (!img_data) {
        printf("Error: Could not load image '%s'\n", path);
        return NULL;
    }

    Image* image = create_image(width, height);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            image->data[j][i].r = (double)img_data[(j * width + i) * 3] / 255.0;
            image->data[j][i].g = (double)img_data[(j * width + i) * 3 + 1] / 255.0;
            image->data[j][i].b = (double)img_data[(j * width + i) * 3 + 2] / 255.0;
        }
    }

    free(img_data); // Free the loaded image data

    return image;
}

// Function to save an image to a file
void save_image(char* path, Image* image) {
    int width = image->width;
    int height = image->height;
    unsigned char* img_data = (unsigned char*)malloc(width * height * 3); // 3 channels for RGB

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            img_data[(j * width + i) * 3] = (unsigned char)(image->data[j][i].r * 255);
            img_data[(j * width + i) * 3 + 1] = (unsigned char)(image->data[j][i].g * 255);
            img_data[(j * width + i) * 3 + 2] = (unsigned char)(image->data[j][i].b * 255);
        }
    }

    stbi_write_png(path, width, height, 3, img_data, 0); // Save as a PNG file

    free(img_data); // Free the allocated image data
}

// Function to perform love animation
void love_animation(double smax, double smin, double sbeat, Image* loveimages[2], Image* background) {
    // Calculate the number of frames based on the desired animation duration and frame rate
    int num_frames = 60;  // Adjust this for the desired number of frames
    double s = smax;

    for (int frame = 0; frame < num_frames; frame++) {
        // Calculate the current scale value for this frame
        double t = (double)frame / (num_frames - 1);  // Interpolation parameter [0, 1]
        s = smax - t * (smax - smin);

        // Create a copy of the background image
        Image* current_frame = create_image(background->width, background->height);

        // Blend loveimages onto current_frame using stencil function
        current_frame = stencil(loveimages[frame % 2], current_frame, s);

        // Save the current_frame as an image (name them sequentially)
        char filename[256];
        sprintf(filename, "frame%d.png", frame);
        save_image(filename, current_frame);

        // Free the current_frame
        destroy_image(current_frame);
    }
}

int main() {
    // Load and filter the background image (use the heart-shaped image)
    Image* background = load_image("heart_background.png");

    // Load love images (two images for animation)
    Image* loveimage1 = load_image("love1.jpg");
    Image* loveimage2 = load_image("love2.jpg");
    Image* loveimages[2] = { loveimage1, loveimage2 };

    // Call the love_animation function with the loaded images
    love_animation(1.0, 1.0 / 30, 1.25, loveimages, background);

    // Free allocated memory
    destroy_image(background);
    destroy_image(loveimage1);
    destroy_image(loveimage2);

    return 0;
}
