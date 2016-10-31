#include <stdio.h>
#include <math.h>
#include "cg_raytracer.h"
#include "parser.h"

#define SWAP(x) ( ((x) << 24) | \
	(((x) << 8) & 0x00ff0000) | \
	(((x) >> 8) & 0x0000ff00) | \
	((x) >> 24) )

void save_image();

const char* FILE_NAME = "scene.avs";

int main(int argc, char const *argv[]) {

	if (argc == 1){
		printf("No file available\n");
		return -1;
	}
	else{
		FILE *fp = fopen(argv[1], "r");
		parser_init(fp);
		fclose(fp);
	}

	cgColor color = {1, 1, 0, 1};

	cgPoint3f points[4] = {
		{.x = 100, .y = 750, .z = 2000},
		{.x = 1000, .y = 750, .z = 2000},
		{.x = 1000, .y = 800, .z = 200},
		{.x = 0, .y = 800, .z = 200}
	};

	cgAddPolygonToScene(points, 4, color);

	cgPoint3f center = {.x = 700, .y = 150, .z = 450};

	color.r = 0;
	color.g = 1;
	color.b = 1;
	cgVector3f direction = {100, 0, 100};
	cgAddCylinderToScene(center, direction, 150, -200, 100, color);

	center.x = 100;
	center.y = 150;
	center.z = 300;

	direction.x = 0;
	direction.y = 100;
	direction.z = 100;
	cgAddConeToScene(center, direction, 50, 100, 0, 200, color);

	cgPoint3f light_position = {200, 270, 100};
	cgAddLightSourceToScene(light_position, 0.8, 0, 0.0025, 0);

	light_position.x = 750;
	light_position.y = 700;
	cgAddLightSourceToScene(light_position, 0.6, 0, 0.01, 0);

	light_position.x = 550;
	light_position.y = 100;
	light_position.z = 300;
	cgAddLightSourceToScene(light_position, 1, 0, 0.01, 0);
	
	cgGenerateImage();
	save_image();

	return 0;
}

void save_image(){
	FILE * fp = fopen(FILE_NAME, "w");
	/* Attempt to open the file */
	if (fp == NULL) {
		printf("failed to create file\n");
	}
	else{
		// Store header
		unsigned int width = SWAP(framebuffer_h);
		unsigned int height = SWAP(framebuffer_v);
		fwrite(&width, sizeof(unsigned int), 1, fp);
		fwrite(&height, sizeof(unsigned int), 1, fp);

		for(int i = 0; i < framebuffer_v; i++){
			for(int j = 0; j < framebuffer_h; j++){
				/* Write the current pixel */
				unsigned char alpha = round(framebuffer[j][i].a * 255);
				unsigned char red = round(framebuffer[j][i].r * 255);
				unsigned char green = round(framebuffer[j][i].g * 255);
				unsigned char blue = round(framebuffer[j][i].b * 255);

				fwrite(&alpha, sizeof(unsigned char), 1, fp);
				fwrite(&red, sizeof(unsigned char), 1, fp);
				fwrite(&green, sizeof(unsigned char), 1, fp);
				fwrite(&blue, sizeof(unsigned char), 1, fp);
			}
		}

		fclose(fp);
	}
}
