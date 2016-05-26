#include "duality/CoordinateSystem.h"

#include <string>

using namespace IVDA;

Axis::Axis()
    : coordinateAxis(X_Axis)
    , positive(true) {}

Axis::Axis(CoordinateAxis ca, bool p)
    : coordinateAxis(ca)
    , positive(p) {}

Axis::Axis(int a, bool p)
    : axis(a)
    , positive(p) {}

std::ostream& operator<<(std::ostream& os, Axis const& a) {
    std::string axis;
    switch (a.axis) {
    case X_Axis:
        axis = "x";
        break;
    case Y_Axis:
        axis = "y";
        break;
    case Z_Axis:
        axis = "z";
        break;
    }
    return os << (a.positive ? "+" : "-") << axis;
}

bool Axis::operator==(Axis const& other) const {
    return axis == other.axis && positive == other.positive;
}

bool Axis::operator!=(Axis const& other) const {
    return axis != other.axis || positive != other.positive;
}


Direction::Direction(TechnicalDirection td)
    : technicalDirection(td) {}

Direction::Direction(AnatomicalDirection ad)
    : anatomicalDirection(ad) {}

Direction::Direction(int d)
    : direction(d) {}

std::ostream& operator<<(std::ostream& os, Direction const& d) {
    std::string direction = "???";
    switch (d.technicalDirection) {
    case TD_Unkown:
        direction = "UNKNOWN";
        break;
    case TD_TechnicalLeft:
        direction = "technicalLeft/anatomicalRight/-x";
        break;
    case TD_TechnicalRight:
        direction = "technicalRight/anatomicalLeft/+x";
        break;
    case TD_Bottom:
        direction = "bottom/inferior/-y";
        break;
    case TD_Top:
        direction = "top/superior/+y";
        break;
    case TD_Back:
        direction = "back/posterior/-z";
        break;
    case TD_Front:
        direction = "front/anterior/+z";
        break;
    }
    return os << direction;
}

Direction::operator int() const {
    return direction;
}


CoordinateSystem::CoordinateSystem()
    : type(CST_Technical)
    , mapping({TD_TechnicalRight, TD_Top, TD_Front})
    , orientation({TD_Top, TD_Front}) {}

bool CoordinateSystem::isValid() const {
    return type != CST_Invalid;
}

Axis CoordinateSystem::mappedDirectionToAxis(Direction const& direction) const {
    if (direction == 0 || direction < -3 || direction > 3)
        throw std::runtime_error("invalid direction");

    // create inverse mapping
    std::array<Axis, 3> inverseMapping;
    for (int i = 0; i < mapping.axes.size(); i++) {
        int d = mapping.axes[i];
        size_t a = std::abs(d) - 1;
        inverseMapping[a] = {i, 0 < d};
    }

    // do lookup
    size_t a = std::abs(direction) - 1;
    Axis axis = inverseMapping[a];
    if (direction < 0)
        axis.positive = !axis.positive;

    return axis;
}

Direction CoordinateSystem::mappedAxisToDirection(Axis const& a) const {
    if (a.axis > 2)
        throw std::runtime_error("invalid axis");

    int mappedAxis = mapping.axes[a.axis];

    if (!a.positive) {
        mappedAxis *= -1;
    }

    return mappedAxis;
}

Axis duality::directionToAxis(Direction const& direction) {
    switch (direction) {
    case +1:
        return {0, true};
    case -1:
        return {0, false};
    case +2:
        return {1, true};
    case -2:
        return {1, false};
    case +3:
        return {2, true};
    case -3:
        return {2, false};
    }
    throw std::runtime_error("invalid direction");
}

Direction duality::axisToDirection(Axis const& a) {
    if (a.axis > 2)
        throw std::runtime_error("invalid axis");

    int direction = (int)a.axis + 1;

    if (!a.positive) {
        direction *= -1;
    }

    return direction;
}

Axis duality::vectorToAxis(Vec3i const& v) {
    if (v.x != 0 && v.y == 0 && v.z == 0) {
        if (v.x > 0)
            return {0, true};
        else
            return {0, false};
    } else if (v.x == 0 && v.y != 0 && v.z == 0) {
        if (v.y > 0)
            return {1, true};
        else
            return {1, false};
    } else if (v.x == 0 && v.y == 0 && v.z != 0) {
        if (v.z > 0)
            return {2, true};
        else
            return {2, false};
    }
    throw std::runtime_error("invalid conversion");
}

Vec3i duality::axisToVector(Axis const& a) {
    switch (a.axis) {
    case X_Axis:
        if (a.positive)
            return {+1, 0, 0};
        else
            return {-1, 0, 0};
    case Y_Axis:
        if (a.positive)
            return {0, +1, 0};
        else
            return {0, -1, 0};
    case Z_Axis:
        if (a.positive)
            return {0, 0, +1};
        else
            return {0, 0, -1};
    default:
        throw std::runtime_error("invalid conversion");
    }
}
