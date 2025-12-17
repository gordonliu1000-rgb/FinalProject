#include "EnermyBullet.h"
#include "VampireBullet.h"
#include "../data/ImageCenter.h"
#include "../data/GIFCenter.h"

void draw(){

}

void EnermyBullet::init(){

}

void VampireBullet::init_img(){
    ImageCenter *IC = ImageCenter::get_instance();
    char buffer[60];
    for(int i=0;i<4;i++){
        sprintf(
            buffer, "./assets/image/mob/vampire/bullet/%d.png",
            i
        );
        img.emplace_back(IC->get(buffer));
    }

    GIFCenter *GC = GIFCenter::get_instance();
    explode_img = GC->get("./assets/image/mob/vampire/bullet/%d.png");
    
}

void VampireBullet::update(){
    if(bitmap_switch_counter!=0){
        bitmap_switch_counter--;
        return;
    }

    
}

void VampireBullet::draw(){

}