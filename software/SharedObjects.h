
#ifndef SharedObjects_h
#define SharedObjects_h

#include "mbed.h"

extern mbed::Serial pc;


namespace Cupteano {
    
    typedef int16_t Angle;
#   define CUPTEANO_ANGLE_NONE INT16_MAX
#   define CUPTEANO_ANGLE_ERROR INT16_MIN
    
    inline static constexpr Angle safeAngles(Angle angle) {
        if (angle == CUPTEANO_ANGLE_NONE || angle == CUPTEANO_ANGLE_ERROR)
            return angle;
        if (angle > 180) angle -= 360;
        if (angle < -179) angle += 360;
        return angle;
    }
    
    inline static constexpr Angle reverseAngles(Angle angle) {
        if (angle == CUPTEANO_ANGLE_NONE || angle == CUPTEANO_ANGLE_ERROR)
            return angle;
        return safeAngles(angle > 0 ? angle-180 : angle+180);
    }
    
    enum class Direction {
        None,
        Error,
        Front,
        Back,
        Left,
        Right,
        LeftFront,
        RightFront,
        LeftBack,
        RightBack
    };
    
    static Angle directionToAngle(Direction dir) {
        Angle angle;
        switch (dir) {
            case Direction::None:
                angle = CUPTEANO_ANGLE_NONE; break;
            case Direction::Error:
                angle = CUPTEANO_ANGLE_ERROR; break;
            case Direction::Front:
                angle = 0;  break;
            case Direction::Back:
                angle = -180; break;
            case Direction::Left:
                angle = -90; break;
            case Direction::Right:
                angle = 90; break;
            case Direction::LeftFront:
                angle = -45; break;
            case Direction::RightFront:
                angle = 45; break;
            case Direction::LeftBack:
                angle = -135; break;
            case Direction::RightBack:
                angle = 135; break;
        }
        return angle;
    }
    
    enum class Vector {
        None = 0,
        Plus = 1,
        Minus = -1
    };
    
    typedef struct {
        Vector x;
        Vector y;
    } Vectors;
    
    static Vectors directionToVector(Direction dir) {
        Vectors vec;
        switch (dir) {
            case Direction::None:
            case Direction::Error:
                vec.x = vec.y = Vector::None; break;
            case Direction::Front:
                vec.x = Vector::Plus; vec.y = Vector::None; break;
            case Direction::Back:
                vec.x = Vector::Minus; vec.y = Vector::None; break;
            case Direction::Left:
                vec.x = Vector::None; vec.y = Vector::Minus; break;
            case Direction::Right:
                vec.x = Vector::None; vec.y = Vector::Plus; break;
            case Direction::LeftFront:
                vec.x = Vector::Plus; vec.y = Vector::Minus; break;
            case Direction::RightFront:
                vec.x = Vector::Plus; vec.y = Vector::Plus; break;
            case Direction::LeftBack:
                vec.x = Vector::Minus; vec.y = Vector::Minus; break;
            case Direction::RightBack:
                vec.x = Vector::Minus; vec.y = Vector::Plus; break;
        }
        return vec;
    }
    
    enum class BallState {
        None,
        Far,
        Middle,
        Near
    };
    
}

#endif /* SharedObjects_h */
