#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits> // Include for std::numeric_limits

class interval {
  public:
    // Define the minimum and maximum values of the interval
    float min, max;

    // Define the default constructor
    interval() : min(std::numeric_limits<float>::infinity()), max(-std::numeric_limits<float>::infinity()) {}
    interval(float min, float max) : min(min), max(max) {}

    // Define the size of the interval
    float size() const {
        return max - min;
    }

    // Define the center of the interval
    bool contains(float x) const {
        return min <= x && x <= max;
    }

    // Define the surrounding interval
    bool surrounds(float x) const {
        return min < x && x < max;
    }

    // Define the intersection of the interval
    float clamp(float x) const {
        return x < min ? min : x > max ? max : x;
    }

    // Define the intersection of two intervals
    static const interval empty, universe;
};

// Define the empty and universe intervals
const interval interval::empty = interval(std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());
const interval interval::universe = interval(-std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());

#endif
