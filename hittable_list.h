#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <vector>

using std::make_shared;
using std::shared_ptr;

class hittable_list : public hittable {
public:
	std::vector<shared_ptr<hittable>> objects;

	hittable_list() {}
	hittable_list(shared_ptr<hittable> object) {
		add(object);
	}

	void clear() {
		objects.clear();
	}

	void add(shared_ptr<hittable> object) {
		objects.push_back(object);
	}

	bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override {
		hit_record temp_rec;
		bool is_hit = false;
		auto current_closest = tmax;

		for (const auto& object : objects) {
			if (object->hit(r, tmin, current_closest, temp_rec)) {
				is_hit = true;
				current_closest = temp_rec.t;
				rec = temp_rec;
			}
		}

		return is_hit;
	}
};

#endif // !HITTABLE_LIST_H

