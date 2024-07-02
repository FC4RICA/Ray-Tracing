#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
public:
	double aspect_ratio = 1.0;
	int image_width = 100;
	int sample_per_pixel = 10;
	int max_depth = 10;

	void render(const hittable& world) {
		init();

		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (int i = 0; i < image_height; i++) {
			std::clog << "\rScanlines remaining: " << (image_height - i) << ' ' << std::flush;
			for (int j = 0; j < image_width; j++) {
				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < sample_per_pixel; sample++) {
					ray r = get_ray(j, i);
					pixel_color += ray_color(r, max_depth, world);
				}

				write_color(std::cout, pixel_sample_scale * pixel_color);
			}
		}

		std::clog << "\rDone\n";
	}

private:
	int image_height;
	double pixel_sample_scale;
	point3 camera_center;
	point3 pixel0_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;


	void init() {
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		pixel_sample_scale = 1.0 / sample_per_pixel;

		auto focal_length = 1.0;
		auto viewport_height = 2.0;
		auto viewport_width = viewport_height * (double(image_width) / image_height);
		camera_center = point3(0, 0, 0);

		auto viewport_u = vec3(viewport_width, 0, 0);
		auto viewport_v = vec3(0, -viewport_height, 0);
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		auto viewport_top_left = camera_center - vec3(0, 0, focal_length) - (viewport_u / 2) - (viewport_v / 2);
		pixel0_loc = viewport_top_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	}

	ray get_ray(int x, int y) const {
		auto offset = sample_square();
		auto pixel_sample = pixel0_loc + ((x + offset.x()) * pixel_delta_u) + ((y + offset.y()) * pixel_delta_v);
		auto ray_origin = camera_center;
		auto ray_direction = pixel_sample - ray_origin;
		return ray(ray_origin, ray_direction);
	}

	vec3 sample_square() const {
		return vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	color ray_color(const ray& r, int depth, const hittable& world) const {
		if (depth <= 0)
			return color(0, 0, 0);

		hit_record rec;

		if (world.hit(r, interval(0.001, Infinity), rec)) {
			ray scattered;
			color attenuation;
			if (rec.mat->scatter(r, rec, attenuation, scattered)) {
				return attenuation * ray_color(scattered, depth - 1, world);
			}
			
			return color(0, 0, 0);
		}

		return sky_color(r);
	}

	color sky_color(const ray& r) const {
		vec3 unit_direction = unit_vector(r.direction());
		//linear blend
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return ((1.0 - a) * color(1.0, 1.0, 1.0)) + (a * color(0.5, 0.7, 1.0));
	}


};

#endif // !CAMERA_H