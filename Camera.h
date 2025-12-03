#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED
#include "Hero.h"
#include "WorldCoordinate.h"
class WorldCoordinate;

class Camera
{

private:
    Hero *hero;
    double x;
    double y;
    Camera(){};
    ~Camera(){};
public:
    void set_target(Hero *target);
    static Camera *get_instance(){
        static Camera camera;
        return &camera;
    }
    double top_left_x() const { return x; }
    double top_left_y() const { return y; }
    void update();
    void init();
};

#endif