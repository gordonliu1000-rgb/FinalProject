#include "EnermyBullet.h"


class VampireBullet : public EnermyBullet{
public:
    VampireBullet(const double &atk, const float &speed){
        
    }
    static void init_img(){};
    static std::vector<ALLEGRO_BITMAP*> img;
    static ALGIF_ANIMATION* explode_img;
    void draw();
    void update();

};