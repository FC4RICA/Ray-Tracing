#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
public:
	double min, max;

	interval() : min(+Infinity), max(-Infinity) {}

	interval(double min, double max) : min(min), max(max) {}

	double size() const {
		return max - min;
	}

	bool contains(double num) const {
		return min <= num && num <= max;
	}

	bool surrounds(double num) const {
		return min < num && num < max;
	}

	double clamp(double num) const {
		if (num < min) return min;
		if (num > max) return max;
		return num;
	}

	static const interval empty, universe;
};

const interval interval::empty = interval(+Infinity, -Infinity);
const interval interval::universe = interval(-Infinity, +Infinity);

#endif // !INTERVAL_H
