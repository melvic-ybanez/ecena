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

    bool Intersection::operator<(const Intersection &that) const {
        return this->t() < that.t();
    }

    Aggregate::Aggregate(AggregateData elems_) : elems_{std::move(elems_)} {
        is_sorted = false;
    }

    Aggregate::~Aggregate() {
        for (auto e: elems_) {
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

    Intersection *Aggregate::hit() {
        if (!is_sorted) {
            std::sort(elems_.begin(), elems_.end(), [](Intersection *t1, Intersection *t2) { return *t1 < *t2; });
            is_sorted = true;
        }

        for (auto e : elems_) {
            if (e->t() >= 0) return e;
        }
        return nullptr;
    }
}