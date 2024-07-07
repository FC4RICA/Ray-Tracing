#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
public:
	double aspect_ratio = 1.0;			// Ratio of image width over height
	int    image_width = 100;			// Rendered image width in pixel count
	int    samples_per_pixel = 10;		// Count of random samples for each pixel
	int    max_depth = 10;				// Maximum number of ray bounces into scene

	double vfov = 90;					// Vertical view angle (field of view)
	point3 lookfrom = point3(0, 0, 0);	// Point camera is looking from
	point3 lookat = point3(0, 0, -1);	// Point camera is looking at
	vec3   vup = vec3(0, 1, 0);			// Camera-relative "up" direction

	double defocus_angle = 0;			// Variation angle of rays through each pixel
	double focus_dist = 10;				// Distance from camera lookfrom point to plane of perfect focus

	void render(const hittable& world) {
		init();

		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (int y = 0; y < image_height; y++) {
			std::clog << "\rScanlines remaining: " << (image_height - y) << ' ' << std::flush;
			for (int x = 0; x < image_width; x++) {
				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; sample++) {
					ray r = get_ray(x, y);
					pixel_color += ray_color(r, max_depth, world);
				}

				write_color(std::cout, pixel_samples_scale * pixel_color);
			}
		}

		std::clog << "\rDone\n";
	}

private:
	int    image_height = 0;        // Rendered image height
	double pixel_samples_scale = 0; // Color scale factor for a sum of pixel samples
	point3 camera_center;       // Camera center
	point3 pixel0_loc;          // Location of pixel 0, 0
	vec3   pixel_delta_u;       // Offset to pixel to the right
	vec3   pixel_delta_v;       // Offset to pixel below
	vec3   u, v, w;             // Camera frame basis vectors
	vec3 defocus_disk_u;		// Defocus disk horizontal radius
	vec3 defocus_disk_v;		// Defocus disk vertical radius

	void init() {
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		pixel_samples_scale = 1.0 / samples_per_pixel;

		camera_center = lookfrom;

		// Determine viewport dimensions.
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / 2);
		auto viewport_height = 2 * h * focus_dist;
		auto viewport_width = viewport_height * (double(image_width) / image_height);

		// Calculate the u,v,w unit basis vectors for the camera coordinate frame
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		// Calculate the vectors across the horizontal and down the vertical viewport edges
		vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
		vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

		// Calculate the horizontal and vertical delta vectors from pixel to pixel
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		// Calculate the location of the upper left pixel
		auto viewport_top_left = camera_center - (focus_dist * w) - (viewport_u / 2) - (viewport_v / 2);
		pixel0_loc = viewport_top_left + 0.5 * (pixel_delta_u + pixel_delta_v);

		// Calcuate the camera defocus disk vectors
		auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
		defocus_disk_u = u * defocus_radius;
		defocus_disk_v = v * defocus_radius;
	}

	ray get_ray(int x, int y) const {
		// Construct a camera ray originating from the defocus disk and directed at a randomly
		// sampled point around the pixel location i, j.
		auto offset = sample_square();
		auto pixel_sample = pixel0_loc + ((x + offset.x()) * pixel_delta_u) + ((y + offset.y()) * pixel_delta_v);
		auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
		auto ray_direction = pixel_sample - ray_origin;
		return ray(ray_origin, ray_direction);
	}

	vec3 sample_square() const {
		return vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	point3 defocus_disk_sample() const {
		// Return a random point in the camera defocus disk.
		auto point = random_in_unit_disk();
		return camera_center + (point[0] * defocus_disk_u) + (point[1] * defocus_disk_v);
	}

	color ray_color(const ray& r, int depth, const hittable& world) const {
		if (depth <= 0)
			return color(0, 0, 0);

		hit_record rec;

		if (world.hit(r, interval(0.001, Infinity), rec)) {
			ray scattered;
			color attenuation;
			if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
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