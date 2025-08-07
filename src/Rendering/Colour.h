//
// Created by Ben Gotts on 07/08/2025.
//

#ifndef VOXL_COLOUR_H
#define VOXL_COLOUR_H

struct Colour {
    int R;
    int G;
    int B;

    Colour() {}
    Colour(int r, int g, int b) : R(r), G(g), B(b) {}

    // static const Colour RED = Colour(1, 0, 0);
};

#endif //VOXL_COLOUR_H