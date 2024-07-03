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
		auto reflexed_dir = reflex(ray_in.direction(), rec.normal);
		reflexed_dir = unit_vector(reflexed_dir) + (fuzz * random_unit_vec());
		scattered = ray(rec.p, reflexed_dir);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

private:
	color albedo;
	double fuzz;
};

#endif // !MATERIAL_H