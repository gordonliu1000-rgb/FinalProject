#ifndef BUFFITEM_H_INCLUDED
#define BUFFITEM_H_INCLUDED

#include "Buff.h"
#include "../Object.h"
#include "../shapes/Rectangle.h"
#include "../shapes/Point.h"

/**
 * @brief 地上可以被撿起的 Buff 道具。
 * @details Buffitem 繼承 Object，使用 Rectangle 做 hit box。
 */


 
class Buffitem : public Object{
    public:
        Buffitem(BuffType type, const Point &pos);
        void update();
        void draw();
        ALLEGRO_BITMAP *bitmap;
        BuffType get_type() const {return type;}

    private:
        BuffType type;
        int bitmap_map_id;
};

#endif