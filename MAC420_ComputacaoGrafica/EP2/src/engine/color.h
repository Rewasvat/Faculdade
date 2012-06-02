
#ifndef COLOR_H_
#define COLOR_H_

namespace engine {

class Color {
  public:

    Color() : r(1.0), g(1.0), b(1.0), a(1.0) {}
    Color(float _r, float _g, float _b, float _a = 1.0) : r(_r), g(_g), b(_b), a(_a) {}

    ~Color() { }

    union {
        struct { float r, g, b, a; };
        struct { float val[4]; };
    };

};

static const Color WHITE(1.0, 1.0, 1.0);
static const Color BLACK(0.0, 0.0, 0.0);

} 
#endif
