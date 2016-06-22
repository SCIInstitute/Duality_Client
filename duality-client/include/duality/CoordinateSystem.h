#pragma once

#include "IVDA/Vectors.h"

#include "mocca/base/BidirectionalMap.h"

#include <array>
#include <ostream>

enum CoordinateAxis { X_Axis = 0, Y_Axis = 1, Z_Axis = 2 };
const mocca::BidirectionalMap<CoordinateAxis, std::string>& coordinateAxisMapper();

enum CoordinateSystemType { CST_Invalid = 0, CST_Technical = 1, CST_Anatomical = 2 };

enum TechnicalDirection : int {
    TD_Unkown = 0,
    TD_TechnicalLeft = -1,
    TD_TechnicalRight = +1,
    TD_Bottom = -2,
    TD_Top = +2,
    TD_Back = -3,
    TD_Front = +3
};

enum AnatomicalDirection : int {
    AD_Unkown = 0,
    AD_AnatomicalRight = -1,
    AD_AnatomicalLeft = +1,
    AD_Inferior = -2,
    AD_Superior = +2,
    AD_Posterior = -3,
    AD_Anterior = +3
};


struct Axis {
    union {
        CoordinateAxis coordinateAxis;
        int axis;
    };
    bool positive;
    
    Axis();
    Axis(CoordinateAxis ca, bool p);
    Axis(int a, bool p);
    
    friend std::ostream& operator<<(std::ostream& os, Axis const& a);
    bool operator==(Axis const& other) const;
    bool operator!=(Axis const& other) const;
};

struct Direction {
    union {
        TechnicalDirection technicalDirection;
        AnatomicalDirection anatomicalDirection;
        int direction;
    };
    
    Direction(TechnicalDirection td);
    Direction(AnatomicalDirection ad);
    Direction(int d);
    friend std::ostream& operator<<(std::ostream& os, Direction const& d);
    operator int() const;
};

struct CoordinateSystem {
    CoordinateSystemType type;
    union {
        std::array<int, 3> axes;
        struct {
            Direction x;
            Direction y;
            Direction z;
        };
    } mapping;
    union {
        std::array<int, 2> setup;
        struct {
            Direction userUp;
            Direction userFace;
        };
    } orientation;
    
    CoordinateSystem();
    bool isValid() const;
    Axis mappedDirectionToAxis(Direction const& d) const;
    Direction mappedAxisToDirection(Axis const& a) const;
};

namespace duality {
    Axis directionToAxis(Direction const& d);
    Direction axisToDirection(Axis const& a);
    Axis vectorToAxis(IVDA::Vec3i const& v);
    IVDA::Vec3i axisToVector(Axis const& a);
}
