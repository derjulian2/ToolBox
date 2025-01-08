#include "Thulium.hpp"

// make color transition stuff at edges of windows
// use std::list instead of std::vector, because this doesnt invalidate references/pointers to the objects when adding/removing/moving objects

int main(int argc, char** argv) {
    thl::Window window(500, 500, "iltam");


    window.Show();
    return 0;
}
