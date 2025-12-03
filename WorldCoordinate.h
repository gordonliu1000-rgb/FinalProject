#ifndef WorldCoordinate_H_INCLUDED
#define WorldCoordinate_H_INCLUDED
#include <allegro5/allegro.h>

class Camera;

class WorldCoordinate
{
public:
    //轉變成世界座標系(警告:此座標系會持續生效直到使用switch_to_camera_coordinate())
    static void switch_to_world_coordinate();

    //轉變成相機(視窗)座標系(警告:此座標系會持續生效直到使用switch_to_world_coordinate())
    static void switch_to_camera_coordinate();
};


#endif