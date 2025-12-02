#ifndef WorldCoordinate_H_INCLUDED
#define WorldCoordinate_H_INCLUDED
#include "Camera.h"
#include <allegro5/allegro.h>


class WorldCoordinate
{
public:
    //This x-coordinate is world coordinate.
    float x;
    //This y-coordinate is world coordinate.
    float y;
private:
    static Camera *camera;
    static ALLEGRO_TRANSFORM world;
public:
    WorldCoordinate():x{0}, y{0}{};
    ~WorldCoordinate() {};
    //轉變成世界座標系(警告:此座標系會持續生效直到使用switch_to_camera_coordinate())
    static void WorldCoordinate::switch_to_world_coordinate();
    //轉變成相機(視窗)座標系(警告:此座標系會持續生效直到使用switch_to_world_coordinate())
    static void WorldCoordinate::switch_to_camera_coordinate();
    //設定關注的相機
    static bool set_camera(Camera *cam);
};



#endif