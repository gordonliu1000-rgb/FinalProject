#ifndef ENERMY_BULLET_H_INCLUDED
#define ENERMY_BULLET_H_INCLUDED
#include "../Object.h"
#include <allegro5/allegro.h>
#include <vector>
#include "../algif5/algif.h"


enum class BulletType {
    VAMPIRE, BULLETTYPE_MAX
};

class EnermyBullet : public Object{
public:
    float speed;
    double atk;
    int bitmap_switch_counter;
	int bitmap_switch_freq;
	int bitmap_img_id = 0;
    static std::unique_ptr<EnermyBullet> create_enermy_bullet(const float &atk, const float &speed, BulletType type);
    static void init();
    BulletType type;
    float target_x, target_y;
    virtual void update() = 0;
};

#endif
