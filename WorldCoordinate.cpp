#include "WorldCoordinate.h"
#include "Camera.h"
#include <allegro5/allegro.h>


void WorldCoordinate::switch_to_camera_coordinate(){
    static ALLEGRO_TRANSFORM world;
    al_identity_transform(&world);
    al_use_transform(&world);
}

void WorldCoordinate::switch_to_world_coordinate(){
    static ALLEGRO_TRANSFORM world;
    static Camera *camera = Camera::get_instance();
    al_identity_transform(&world);
    al_translate_transform(&world, -camera->top_left_x(), -camera->top_left_y());
    al_use_transform(&world);
}

