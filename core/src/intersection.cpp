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

    Aggregate::Aggregate(AggregateData elems_) : elems_{std::move(elems_)} {}

    Aggregate::Aggregate() = default;

    Aggregate::Aggregate(const Aggregate &set) = default;

    Aggregate::Aggregate(Aggregate &&set) noexcept = default;

    Aggregate &Aggregate::operator=(const Aggregate &other) = default;

    Aggregate &Aggregate::operator=(Aggregate &&other)  noexcept = default;

    Aggregate::~Aggregate() {
        for (auto e : elems_) {
            delete e;
        }
        elems_.clear();
    }

    size_t Aggregate::count() const {
        return elems_.size();
    }

    bool Aggregate::empty() const {
        return elems_.empty();
    }

    Intersection *Aggregate::operator[](size_t i) {
        return elems_[i];
    }
}