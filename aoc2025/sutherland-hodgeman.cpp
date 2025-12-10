
#include "sutherland-hodgeman.h"

// Sutherland-Hodgman clipping
std::vector<point_t> sutherland_hodgman(
    std::span<point_t const> subject_polygon, std::span<point_t const> clip_polygon) {
    if (clip_polygon.empty() || subject_polygon.empty()) {
        return {};
    }

    std::vector<point_t> ring{subject_polygon.begin(), subject_polygon.end()};

    point_t p1 = clip_polygon[clip_polygon.size() - 1];

    std::vector<point_t> input;

    for (point_t p2 : clip_polygon) {
        input.clear();
        input.insert(input.end(), ring.begin(), ring.end());
        point_t s = input[input.size() - 1];

        ring.clear();

        for (point_t e : input) {
            if (is_inside(e, p1, p2)) {
                if (!is_inside(s, p1, p2)) {
                    ring.push_back(intersection(p1, p2, s, e));
                }

                ring.push_back(e);
            } else if (is_inside(s, p1, p2)) {
                ring.push_back(intersection(p1, p2, s, e));
            }

            s = e;
        }

        p1 = p2;
    }

    return ring;
}
