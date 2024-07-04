#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <vector>

using std::make_unique;
using std::unique_ptr;

class hittable_list : public hittable {
public:
	std::vector<unique_ptr<hittable>> objects;

	hittable_list() {}
	hittable_list(unique_ptr<hittable> object) {
		add(std::move(object));
	}

	void clear() {
		objects.clear();
	}

	void add(unique_ptr<hittable> object) {
		objects.push_back(std::move(object));
	}

	bool hit(const ray& r, interval t, hit_record& rec) const override {
		hit_record temp_rec;
		bool hit_anything = false;
		auto current_closest = t.max;

		for (const auto& object : objects) {
			if (object->hit(r, interval(t.min, current_closest), temp_rec)) {
				hit_anything = true;
				current_closest = temp_rec.t;
				rec = std::move(temp_rec);
			}
		}

		return hit_anything;
	}
};

#endif // !HITTABLE_LIST_H

