//
// Created by Melvic Ybanez on 12/19/22.
//

#include "../include/intersection.h"
#include "../../engine/include/shapes.h"

namespace rt::intersections {
    Intersection::Intersection(real t, Shape* object) : t{t}, object{object} {}

    bool Intersection::operator<(const Intersection& that) const {
        return this->t < that.t;
    }

    Aggregate::Aggregate(AggregateData elems) : elems{std::move(elems)} {}

    Aggregate::~Aggregate() {
        for (auto& elem: elems) {
            delete elem;
            elem = nullptr;
        }
        elems.clear();
    }

    size_t Aggregate::count() const {
        return elems.size();
    }

    bool Aggregate::empty() const {
        return elems.empty();
    }

    Intersection* Aggregate::operator[](size_t i) {
        return elems[i];
    }

    Intersection* Aggregate::hit() {
        sort();
        for (const auto e: elems) {
            if (e->t >= 0) return e;
        }
        return nullptr;
    }

    std::ostream& operator<<(std::ostream& out, const Intersection& intersection) {
        return out << "{ t: " << intersection.t << ", object: " << *intersection.object << " }";
    }

    void Aggregate::combine_with(Aggregate& that) {
        std::copy(that.elems.begin(), that.elems.end(), std::back_inserter(this->elems));
        for (auto& elem: that.elems) {
            elem = nullptr;
        }
        is_sorted = is_sorted && that.count() == 0;
        sort();
    }

    void Aggregate::sort() {
        if (is_sorted) return;

        std::sort(this->elems.begin(), this->elems.end(),
                  [](Intersection* t1, Intersection* t2) { return *t1 < *t2; });
        is_sorted = true;
    }

    void Aggregate::add(Intersection* intersection) {
        elems.push_back(intersection);
    }
}