
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "Vect.h"
#include "Ray.h"
#include "Camera.h"
#include "Light.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"

struct RGBType
{
	double red;
	double green;
	double blue;
};

void savebmp(const char* filename, int width, int height, int dpi, RGBType* data)
{
	FILE* file;
	int window = width * height;
	int size = 4 * window;
	int filesize = 54 + size;

	double factor = 39.375;
	int m = static_cast<int>(factor);

	int pixels_per_meter = dpi * m;

	unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0 ,0,0,0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(width);
	bmpinfoheader[5] = (unsigned char)(width >> 8);
	bmpinfoheader[6] = (unsigned char)(width >> 16);
	bmpinfoheader[7] = (unsigned char)(width >> 24);

	bmpinfoheader[8] = (unsigned char)(height);
	bmpinfoheader[9] = (unsigned char)(height >> 8);
	bmpinfoheader[10] = (unsigned char)(height >> 16);
	bmpinfoheader[11] = (unsigned char)(height >> 24);

	bmpinfoheader[21] = (unsigned char)(size);
	bmpinfoheader[22] = (unsigned char)(size >> 8);
	bmpinfoheader[23] = (unsigned char)(size >> 16);
	bmpinfoheader[24] = (unsigned char)(size >> 24);

	bmpinfoheader[25] = (unsigned char)(pixels_per_meter);
	bmpinfoheader[26] = (unsigned char)(pixels_per_meter >> 8);
	bmpinfoheader[27] = (unsigned char)(pixels_per_meter >> 16);
	bmpinfoheader[28] = (unsigned char)(pixels_per_meter >> 24);

	bmpinfoheader[29] = (unsigned char)(pixels_per_meter);
	bmpinfoheader[30] = (unsigned char)(pixels_per_meter >> 8);
	bmpinfoheader[31] = (unsigned char)(pixels_per_meter >> 16);
	bmpinfoheader[32] = (unsigned char)(pixels_per_meter >> 24);

	file = fopen(filename, "wb");

	fwrite(bmpfileheader, 1, 14, file);
	fwrite(bmpinfoheader, 1, 40, file);

	for (int i = 0; i < window; i++)
	{
		RGBType rgb = data[i];

		double red = (data[i].red) * 255;
		double green = (data[i].green) * 255;
		double blue = (data[i].blue) * 255;

		unsigned char color[3] = { static_cast<unsigned char>((int)floor(blue)), static_cast<unsigned char>((int)floor(green)), static_cast<unsigned char>((int)floor(red)) };

		fwrite(color, 1, 3, file);
	}

	fclose(file);
}

int winningObjectIndex(std::vector<double> object_intersections)
{
	// Return the index of the winning intersections.
	int index_of_minimum_value;

	// Prevent unnecessary calculations.
	if (object_intersections.empty())
	{
		// If there are no intersections
		return -1;
	}
	else if (object_intersections.size() == 1)
	{
		if (object_intersections.at(0) > 0)
		{
			// If that intersection is greater than zero then it's our index of minumum value.
			return 0;
		}
		else
		{
			// Otherwise the only intersection is negative.
			return -1;
		}
	}
	else
	{
		// Othervise there is more than one intersection.
		// First find the maximum value.

		double max = 0;
		for (double object_intersection : object_intersections)
		{
			if (max < object_intersection)
			{
				max = object_intersection;
			}
		}
		// Then starting from the maximum value find the minimum positive value.
		if (max > 0)
		{
			// We only want positive intersections.
			for (size_t index = 0; index < object_intersections.size(); index++)
			{
				if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max)
				{
					max = object_intersections.at(index);
					index_of_minimum_value = index;
				}
			}
			return index_of_minimum_value;
		}
		else
		{
			// All the intersections were negative.
			return -1;
		}
	}
}

Color getColorAt(Vect intersection_position, Vect intersecting_ray_direction, std::vector<Object*> scene_objects, int index_of_winning_objects, std::vector<Source*> light_sources, double accuracy, double ambient_light)

{
	Color winning_objects_color = scene_objects.at(index_of_winning_objects)->getColor();
	Vect winning_objects_normal = scene_objects.at(index_of_winning_objects)->getNormalAt(intersection_position);

	if (winning_objects_color.getColorSpecial() == 2)
	{
		// Checkered tile floor pattern.
		int square = (int)floor(intersection_position.getVectX()) + (int)floor(intersection_position.getVectZ());

		if ((square % 2) == 0)
		{
			// Black Tile.
			winning_objects_color.setColorRed(0);
			winning_objects_color.setColorGreen(0);
			winning_objects_color.setColorBlue(0);
		}
		else
		{
			// White Tile.
			winning_objects_color.getColorRed();
			winning_objects_color.getColorGreen();
			winning_objects_color.getColorBlue();
		}
	}

	Color final_color = winning_objects_color.colorScalar(ambient_light);

	if (winning_objects_color.getColorSpecial() > 0 && winning_objects_color.getColorSpecial() <= 1)
	{
		// Reflection from objects with specular intensity.
		double dot1 = winning_objects_normal.dotProduct(intersecting_ray_direction.negative());
		Vect scalar1 = winning_objects_normal.vectMult(dot1);
		Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
		Vect scalar2 = add1.vectMult(2);
		Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
		Vect reflection_direction = add2.normalize();

		Ray reflection_ray(intersection_position, reflection_direction);

		// Determine what the ray intersects with first.

		std::vector<double> reflection_intersections;

		reflection_intersections.reserve(scene_objects.size());
for (auto & scene_object : scene_objects)
		{
			reflection_intersections.push_back(scene_object->findIntersection(reflection_ray));
		}

		int index_of_winning_objects_with_reflection = winningObjectIndex(reflection_intersections);

		if (index_of_winning_objects_with_reflection != -1)
		{
			// Reflection ray missed everything else.
			if (reflection_intersections.at(index_of_winning_objects_with_reflection) > accuracy)
			{
				// Determine the position and direction at the point of intersection with the reflection ray.
				// The ray only affects the color if it's reflected off something.
				Vect reflection_intersection_position = intersection_position.vectAdd(reflection_direction.vectMult(reflection_intersections.at(index_of_winning_objects_with_reflection)));
				Vect reflection_intersection_ray_direction = reflection_direction;

				Color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersection_ray_direction, scene_objects, index_of_winning_objects_with_reflection, light_sources, accuracy, ambient_light);

				final_color = final_color.colorAdd(reflection_intersection_color.colorScalar(winning_objects_color.getColorSpecial()));
			}
		}
	}

	for (auto & light_source : light_sources)
	{
		Vect light_direction = light_source->getLightPosition().vectAdd(intersection_position.negative()).normalize();

		double cosine_angle = winning_objects_normal.dotProduct(light_direction);

		if (cosine_angle > 0)
		{
			// Test for shadows.
			bool shadowed = false;
			Vect distance_to_light = light_source->getLightPosition().vectAdd(intersection_position.negative()).normalize();
			double distance_to_light_magnitude = distance_to_light.magnitude();
			Ray shadow_ray(intersection_position, light_source->getLightPosition().vectAdd(intersection_position.negative()).normalize());

			std::vector<double> secondary_intersections;

			secondary_intersections.reserve(scene_objects.size());
for (auto & scene_object : scene_objects)
			{
				secondary_intersections.push_back(scene_object->findIntersection(shadow_ray));
			}

			for (double secondary_intersection : secondary_intersections)
			{
				if (secondary_intersection > accuracy)
				{
					if (secondary_intersection <= distance_to_light_magnitude)
					{
						shadowed = true;
					}
					break;
				}
			}

			if (!shadowed)
			{
				final_color = final_color.colorAdd(winning_objects_color.colorMultiply(light_source->getLightColor()).colorScalar(cosine_angle));

				if (winning_objects_color.getColorSpecial() > 0 && winning_objects_color.getColorSpecial() <= 1)
				{
					// Special [0 - 1].

					double pixel1 = winning_objects_normal.dotProduct(intersecting_ray_direction.negative());

					Vect scalar_pixel = winning_objects_normal.vectMult(pixel1);
					Vect add_pixel1 = scalar_pixel.vectAdd(intersecting_ray_direction);
					Vect scalar_pixel2 = add_pixel1.vectMult(2);
					Vect add_pixel2 = intersecting_ray_direction.negative().vectAdd(scalar_pixel2);
					Vect reflection_direction = add_pixel2.normalize();

					double specular = reflection_direction.dotProduct(light_direction);
					if (specular > 0)
					{
						specular = pow(specular, 10);
						final_color = final_color.colorAdd(light_source->getLightColor().colorScalar(specular * winning_objects_color.getColorSpecial()));
					}
				}
			}
		}
	}

	return final_color.clip();
}

std::vector<Object*> scene_objects;

void Cube(Vect corner_1, Vect corner_2, Color color)
{
	// Corner 1.
	double corner_one_x = corner_1.getVectX();
	double corner_one_y = corner_1.getVectY();
	double corner_one_z = corner_1.getVectZ();
	// Corner 2.
	double corner_two_x = corner_2.getVectX();
	double corner_two_y = corner_2.getVectY();
	double corner_two_z = corner_2.getVectZ();

	Vect A(corner_two_x, corner_one_y, corner_one_z);
	Vect B(corner_two_x, corner_one_y, corner_two_z);
	Vect C(corner_one_x, corner_one_y, corner_two_z);

	Vect D(corner_two_x, corner_two_y, corner_one_z);
	Vect E(corner_one_x, corner_two_y, corner_one_z);
	Vect F(corner_one_x, corner_two_y, corner_two_z);
	
	// Left Side.
	scene_objects.push_back(new Triangle (D, A, corner_1, color));
	scene_objects.push_back(new Triangle (corner_1, E, D, color));
	// Far Side.
	scene_objects.push_back(new Triangle (corner_2, B, A, color));
	scene_objects.push_back(new Triangle (A, D, corner_2, color));
	// Right side.
	scene_objects.push_back(new Triangle(F, C, B, color));
	scene_objects.push_back(new Triangle(B, corner_2, F, color));
	// Front side.
	scene_objects.push_back(new Triangle(E, corner_1, C, color));
	scene_objects.push_back(new Triangle(C, A, E, color));
	// Top side.
	scene_objects.push_back(new Triangle(D, E, F, color));
	scene_objects.push_back(new Triangle(F, corner_2, D, color));
	// Bottom side.
	scene_objects.push_back(new Triangle(corner_1, A, B, color));
	scene_objects.push_back(new Triangle(B, C, corner_1, color));
}

int main(int argc, char* argv[])
{
	clock_t t1, t2;
	t1 = clock();

	std::cout << "rendering ...\n";

	int dpi = 72;
	int width = 640;
	int height = 480;
	int number_of_pixels = width * height;
	double aspect_ratio = (double)width / (double)height;
	double ambient_light = 0.2;
	double accuracy = 0.0000001;

	// Anti-Aliasing parameters.
	const int anti_aliasing_depth = 10;
	double anti_aliasing_treshhold = 0.1;
	auto* pixels = new RGBType[number_of_pixels];

	
	Vect X(1, 0, 0);
	Vect Y(0, 1, 0);
	Vect Z(0, 0, 1);
	Vect campos(3, 1.5, -4);

	Vect look_at(0, 0, 0);
	Vect diff_between(campos.getVectX() - look_at.getVectX(), campos.getVectY() - look_at.getVectY(), campos.getVectZ() - look_at.getVectZ());
	Vect camdir = diff_between.negative().normalize();
	Vect camright = Y.crossProduct(camdir).normalize();
	Vect camdown = camright.crossProduct(camdir);

	// Camera object for scene.
	Camera scene_cam(campos, camdir, camright, camdown);

	// Color for objects.
	Color white_light(1.0, 1.0, 1.0, 0);
	Color pretty_green(0.5, 1.0, 0.5, 0.3);
	Color maroon(0.5, 0.25, 0.25, 0);
	Color pretty_maroon(0.5, 0.25, 0.25, 0);
	Color tile_floor(1, 1, 1, 2);
	Color gray(0.5, 0.5, 0.5, 0);
	Color black(0.0, 0.0, 0.0, 0);
	Color orange(0.94, 0.75, 0.31, 0);

	Vect light_position(-7, 10, -10);
	Light scene_light(light_position, white_light);
	std::vector<Source*> light_sources;
	light_sources.push_back(dynamic_cast<Source*>(&scene_light));

	// Scene objects.
	//////////////////////////////////////////////////////////////

	// Sphere Location
	Vect scene_sphere_location(0, 0, 0);
	Vect scene_sphere2_location(1.45, -0.25, 0.8);

	// Spheres
	//Sphere scene_sphere(scene_sphere_location, 1, pretty_green);
	Sphere scene_sphere2(scene_sphere2_location, 0.5, pretty_maroon);

	// Triangles
	//Triangle scene_triangle(Vect(3, 0, 0), Vect(0, 3, 0), Vect(0, 0, 3), orange);

	// Planes
	Plane scene_plane(Y, -1, tile_floor);
	/////////////////////////////////////////////////////////////
	
	//scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere2));
	scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));
	//scene_objects.push_back(dynamic_cast<Object*>(&scene_triangle));

	Cube(Vect(1, 1, 1), Vect(-1, -1, -1), orange);

	int thisone2, anti_aliasing_index;
	double xamnt, yamnt;

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
            thisone2 = y * width + x;

			// Start with a blank pixel.
			double tempRed[anti_aliasing_depth * anti_aliasing_depth]{ 0 };
			double tempGreen[anti_aliasing_depth * anti_aliasing_depth]{ 0 };
			double tempBlue[anti_aliasing_depth * anti_aliasing_depth]{ 0 };

			for (size_t anti_aliasing_x = 0; anti_aliasing_x < anti_aliasing_depth; anti_aliasing_x++)
			{
				for (size_t anti_aliasing_y = 0; anti_aliasing_y < anti_aliasing_depth; anti_aliasing_y++)
				{
					anti_aliasing_index = anti_aliasing_y * anti_aliasing_depth + anti_aliasing_x;

					srand(time(nullptr));

					// Create the ray from the camera with this pixel.

					if (anti_aliasing_depth == 1)
					{
						// Start with no anti-aliasing.
						if (width > height)
						{
							// the image is wider than it is tall
							xamnt = ((x + 0.5) / width) * aspect_ratio - (((width - height) / (double)height) / 2);
							yamnt = ((height - y) + 0.5) / height;
						}
						else if (height > width)
						{
							// This image is taller than it is wide.
							xamnt = (x + 0.5) / width;
							yamnt = (((height - y) + 0.5) / height) / aspect_ratio - (((height - width) / (double)width) / 2);
						}
						else
						{
							// This image is square.
							xamnt = (x + 0.5) / width;
							yamnt = ((height - y) + 0.5) / height;
						}
					}
					else
					{
						// Use anti-aliasing.
						if (width > height)
						{
							// the image is wider than it is tall
							xamnt = ((x + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / width) * aspect_ratio - (((width - height) / (double)height) / 2);
							yamnt = ((height - y) + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / height;
						}
						else if (height > width)
						{
							// This image is taller than it is wide.
							xamnt = (x + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / width;
							yamnt = (((height - y) + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / height) / aspect_ratio - (((height - width) / (double)width) / 2);
						}
						else
						{
							// This image is square.
							xamnt = (x + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / width;
							yamnt = ((height - y) + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / height;
						}
					}

					Vect cam_ray_origin = scene_cam.getCameraPosition();
					Vect cam_ray_direction = camdir.vectAdd(camright.vectMult(xamnt - 0.5).vectAdd(camdown.vectMult(yamnt - 0.5))).normalize();

					Ray cam_ray(cam_ray_origin, cam_ray_direction);

					std::vector<double> intersections;

					intersections.reserve(scene_objects.size());
					for (auto & scene_object : scene_objects)
					{
						intersections.push_back(scene_object->findIntersection(cam_ray));
					}

					int index_of_winning_objects = winningObjectIndex(intersections);

					//std::cout << index_of_winning_objects;

					if (index_of_winning_objects == -1)
					{
						// Set the background black.
						tempRed[anti_aliasing_index] = 0;
						tempGreen[anti_aliasing_index] = 0;
						tempBlue[anti_aliasing_index] = 0;
					}
					else
					{
						// Index coresponds to an object in our scene.

						if (intersections.at(index_of_winning_objects) > accuracy)
						{
							// Determine the position and direction vectors at of point of intersection.

							Vect intersection_position = cam_ray_origin.vectAdd(cam_ray_direction.vectMult(intersections.at(index_of_winning_objects)));
							Vect intersecting_ray_direction = cam_ray_direction;
							Color this_color = scene_objects.at(index_of_winning_objects)->getColor();

							Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction, scene_objects, index_of_winning_objects, light_sources, accuracy, ambient_light);

							tempRed[anti_aliasing_index] = intersection_color.getColorRed();
							tempGreen[anti_aliasing_index] = intersection_color.getColorGreen();
							tempBlue[anti_aliasing_index] = intersection_color.getColorBlue();
						}
					}
				}
			}

			// Average the pixel color.
			double totalRed = 0;
			double totalGreen = 0;
			double totalBlue = 0;

			for (double Red : tempRed)

			{
				totalRed += Red;
			}

			for (double Green : tempGreen)

			{
				totalGreen += Green;
			}

			for (double Blue : tempBlue)

			{
				totalBlue += Blue;
			}

			double averageRed = totalRed / (anti_aliasing_depth * anti_aliasing_depth);
			double averageGreen = totalGreen / (anti_aliasing_depth * anti_aliasing_depth);
			double averageBlue = totalBlue / (anti_aliasing_depth * anti_aliasing_depth);

			pixels[thisone2].red = averageRed;
			pixels[thisone2].green = averageGreen;
			pixels[thisone2].blue = averageBlue;
		}
	}

	savebmp("scene_with_anti-aliasing.bmp", width, height, dpi, pixels);

	delete[] pixels;

	t2 = clock();
	float diff = ((float)t1 - (float)t2) / 1000;
	if (diff > -60) { std::cout << diff << " seconds\n"; }
	else { std::cout << diff / 60 << " minutes\n"; }
}
