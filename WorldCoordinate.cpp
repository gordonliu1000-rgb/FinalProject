#include "WorldCoordinate.h"
#include "Camera.h"
#include <allegro5/allegro.h>

Camera* WorldCoordinate::camera = nullptr;

bool WorldCoordinate::set_camera(Camera *cam){
    if(cam==nullptr) return false;
    camera = cam;
    return true;
}

void WorldCoordinate::switch_to_camera_coordinate(){
    al_identity_transform(&world);
    al_use_transform(&world);
}

void WorldCoordinate::switch_to_world_coordinate(){
    al_identity_transform(&world);
    al_translate_transform(&world, -camera->x, -camera->y);
    al_use_transform(&world);
}
