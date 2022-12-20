//
// Created by Melvic Ybanez on 12/19/22.
//

#include "../include/intersection.h"

namespace rt::intersections {
    Intersection::Intersection(real t, const Shape *shape) : t_{t}, shape_{shape} {}

    real Intersection::t() const {
        return t_;
    }

    const Shape *Intersection::shape() const {
        return shape_;
    }

    Set::Set(SetElems elems_) : elems_{std::move(elems_)} {}

    Set::Set() = default;

    Set::Set(const Set &set) = default;

    Set::Set(Set &&set) noexcept = default;

    Set &Set::operator=(const Set &other) = default;

    Set &Set::operator=(Set &&other)  noexcept = default;

    Set::~Set() {
        for (auto e : elems_) {
            delete e;
        }
        elems_.clear();
    }

    size_t Set::size() const {
        return elems_.size();
    }

    bool Set::empty() const {
        return elems_.empty();
    }

    Intersection *Set::operator[](size_t i) {
        return elems_[i];
    }
}