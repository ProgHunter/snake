#include "Body.h"

Body::Body(Coord *init_part, Body *init_next_part) {
    part = init_part;
    next_part = init_next_part;
}

Body::~Body() {
    if(part)
        part->~Coord();
}