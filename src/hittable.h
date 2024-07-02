#ifndef HITTABLE_H
#define HITTABLE_H

class material;

class hit_record {
public:
	point3 p;
	vec3 normal;
	material* mat_ptr;
	double t;
	bool front_face;

	hit_record() : mat_ptr(nullptr) {}

    // Copy constructor and assignment operator
    hit_record(const hit_record& other)
        : p(other.p), normal(other.normal), t(other.t), mat_ptr(other.mat_ptr) {}

    hit_record& operator=(const hit_record& other) {
        if (this != &other) {
            p = other.p;
            normal = other.normal;
            t = other.t;
            mat_ptr = other.mat_ptr;
        }
        return *this;
    }

    // Move constructor and assignment operator
    hit_record(hit_record&& other) noexcept
        : p(std::move(other.p)), normal(std::move(other.normal)), t(other.t), mat_ptr(other.mat_ptr) {
        other.mat_ptr = nullptr;  // Ensure ownership transfer
    }

    hit_record& operator=(hit_record&& other) noexcept {
        if (this != &other) {
            p = std::move(other.p);
            normal = std::move(other.normal);
            t = other.t;
            mat_ptr = other.mat_ptr;
            other.mat_ptr = nullptr;  // Ensure ownership transfer
        }
        return *this;
    }

	void set_face_normal(const ray& r, const vec3& outward_normal) {
		// outward_normal is assumed to have unit length
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual ~hittable() = default;

	virtual bool hit(const ray& r, interval t, hit_record& rec) const = 0;
};

#endif // !HITTABLE_H
