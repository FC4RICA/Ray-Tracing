#include "pch.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

color sky_color(const ray& r) {
	vec3 unit_direction = unit_vector(r.direction());
	//linear blend
	auto a = 0.5 * (unit_direction.y() + 1.0);
	return ((1.0 - a) * color(1.0, 1.0, 1.0)) + (a * color(0.5, 0.7, 1.0));
}

color ray_color(const ray& r, const hittable& world) {
	hit_record rec;
	if (world.hit(r, interval(0, Infinity), rec)) {
		return 0.5 * color(rec.normal + vec3(1, 1, 1));
	}

	return sky_color(r);
}

int main() {
	//image
	auto aspect_ratio = 16.0 / 9.0;
	int image_width = 800;
	int image_height = int(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;

	//world
	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	//camera
	auto focal_length = 1.0;
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (double(image_width) / image_height);
	auto camera_center = point3(0, 0, 0);

	auto viewport_u = vec3(viewport_width, 0, 0);
	auto viewport_v = vec3(0, -viewport_height, 0);
	auto pixel_delta_u = viewport_u / image_width;
	auto pixel_delta_v = viewport_v / image_height;

	auto viewport_top_left = camera_center - vec3(0, 0, focal_length) - (viewport_u / 2) - (viewport_v / 2);
	auto pixel0_loc = viewport_top_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	 
	for (int i = 0; i < image_height; i++) {
		std::clog << "\rScanlines remaining: " << (image_height - i) << ' ' << std::flush;
		for (int j = 0; j < image_width; j++) {
			auto pixel_center = pixel0_loc + (j * pixel_delta_u) + (i * pixel_delta_v);
			auto direction = pixel_center - camera_center;
			ray r(camera_center, direction);

			auto pixel_color = ray_color(r, world);
			write_color(std::cout, pixel_color);
		}
	}

	std::clog << "\rDone\n";
	
	return 0;
}