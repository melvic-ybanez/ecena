//
// Created by Melvic Ybanez on 1/23/23.
//

#include <utility>

#include "../include/bounds.h"

namespace rt {
    /**
     * Note: Since the min point is at positive infinity, and the max point is at
     * negative infinity, this box is considered invalid.
     */
    Bounds::Bounds() : Bounds({math::infinity, math::infinity, math::infinity}) {}

    Bounds::Bounds(Point min, Point max) : min{std::move(min)}, max{std::move(max)} {}

    Bounds::Bounds(const Point &min) : Bounds(min, -min) {}

    Bounds Bounds::cube() {
        return {{-1, -1, -1}};
    }

    Bounds Bounds::operator+(const Bounds &other) const {
        return *this + other.min + other.max;
    }

    Bounds Bounds::transform(const Matrix<4, 4> &transformation) const {
        std::vector<Point> corners{
                {min.x(), min.y(), min.z()}, // lower left back
                {max.x(), min.y(), min.z()}, // lower right back
                {min.x(), min.y(), max.z()}, // lower left front
                {max.x(), min.y(), max.z()}, // lower right front
                {min.x(), max.y(), min.z()}, // upper left back
                {max.x(), max.y(), min.z()}, // upper right back
                {min.x(), max.y(), max.z()}, // upper left front
                {max.x(), max.y(), max.z()}, // upper right front
        };

        Bounds bounds;
        for (const auto &corner: corners) {
            bounds = bounds + (transformation * corner);
        }
        return bounds;
    }

    Bounds Bounds::operator+(const Point &point) const {
        Point new_min{
                std::min(min.x(), point.x()),
                std::min(min.y(), point.y()),
                std::min(min.z(), point.z())
        };
        Point new_max{
                std::max(max.x(), point.x()),
                std::max(max.y(), point.y()),
                std::max(max.z(), point.z())
        };
        return {new_min, new_max};
    }

    bool Bounds::operator==(const Bounds &that) const {
        return this->min == that.min && this->max == that.max;
    }

    std::ostream &operator<<(std::ostream &out, const Bounds &bounds) {
        return out << "{ min: " << bounds.min << ", max" << bounds.max << " }";
    }

    bool Bounds::contains(const Point &point) const {
        return point.x() >= min.x() && point.x() <= max.x()
               && point.y() >= min.y() && point.y() <= max.y()
               && point.z() >= min.z() && point.z() <= max.z();
    }

    bool Bounds::contains(const Bounds &bounds) const {
        return contains(bounds.min) && contains(bounds.max);
    }
}