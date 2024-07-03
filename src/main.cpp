#include "pch.h"

#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "camera.h"


int main() {
	// world
	hittable_list world;
	auto material_ground = make_unique<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_unique<lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = make_unique<metal>(color(0.8, 0.8, 0.8), 0.3);
	auto material_right = make_unique<metal>(color(0.8, 0.6, 0.2), 1.0);

	world.add(make_unique<sphere>(point3(0.0, -100.5, -1.0), 100.0, std::move(material_ground)));
	world.add(make_unique<sphere>(point3(0.0, 0.0, -1.2), 0.5, std::move(material_center)));
	world.add(make_unique<sphere>(point3(-1.0, 0.0, -1.0), 0.5, std::move(material_left)));
	world.add(make_unique<sphere>(point3(1.0, 0.0, -1.0), 0.5, std::move(material_right)));

	// camera
	camera cam;
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 800;
	cam.sample_per_pixel = 100;
	cam.max_depth = 50;
	
	cam.render(world);
	
	return 0;
}