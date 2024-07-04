#ifndef MATERIAL_H
#define MATERIAL_H

class hit_record;

class material {
public:
	virtual ~material() = default;

	virtual bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const {
		return false;
	}
};

class lambertian : public material {
public:
	lambertian(const color& albedo) : albedo(albedo) {}

	bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		auto scatter_dir = rec.normal + random_unit_vec();
		if (scatter_dir.near_zero())
			scatter_dir = rec.normal;

		scattered = ray(rec.p, scatter_dir);
		attenuation = albedo;
		return true;
	}

private:
	color albedo;
};

class metal : public material {
public:
	metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		auto reflected_dir = reflect(ray_in.direction(), rec.normal);
		reflected_dir = unit_vector(reflected_dir) + (fuzz * random_unit_vec());
		scattered = ray(rec.p, reflected_dir);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

private:
	color albedo;
	double fuzz;
};

class dielectric : public material {
public:
	dielectric(double refraction_index) : refraction_index(refraction_index) {}

	bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		attenuation = color(1.0, 1.0, 1.0);
		double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

		vec3 unit_direction = unit_vector(ray_in.direction());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		vec3 direction;

		if (ri * sin_theta > 1.0 || reflectance(cos_theta, ri) > random_double()) {
			// cannot refract: reflecting instead
			direction = reflect(unit_direction, rec.normal);
		}
		else {
			direction = refract(unit_direction, rec.normal, ri);
		}

		scattered = ray(rec.p, direction);
		return true;
	}

private: 
	double refraction_index;

	static double reflectance(double cosin, double reflectance_index) {
		auto r0 = (1 - reflectance_index) / (1 + reflectance_index);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosin), 5);
	}
};

#endif // !MATERIAL_H