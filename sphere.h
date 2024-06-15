#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "hittable.h"

class sphere : public hittable {
public:
	sphere(const point3& center, double radius) : center(center), radius(fmax(0, radius)) {}

	bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override {
		vec3 origin_center = center - r.origin();
		//quadratic formula
		auto a = r.direction().length_squeared();
		auto h = dot(r.direction(), origin_center);
		auto c = origin_center.length_squeared() - (radius * radius);
		auto discriminant = (h * h) - (a * c);

		if (discriminant < 0.0)
			return false;

		auto sqrt_discriminant = sqrt(discriminant);
		auto root = (h - sqrt_discriminant) / a;
		//if root is in range
		if (root <= tmin || root >= tmax) {
			root = (h + sqrt_discriminant) / a;
			if (root <= tmin || root >= tmax) {
				return false;
			}
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);

		return true;
	}

private:
	point3 center;
	double radius;
};

#endif // !SPHERE_H
