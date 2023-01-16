#ifndef __OBJECTS_H
#define __OBJECTS_H

#define OBJECT_CUBE     0x01 << 0x00
#define OBJECT_PLANE    0x01 << 0x01
#define OBJECT_SPHERE   0x01 << 0x02
#define OBJECT_CYLINDER 0x01 << 0x03

struct point {
    double x, y, z;
};

struct color {
    float r, g, b;
};

struct object {
    double radius;
    double shineness;
    double reflection;
    unsigned char type;
    struct point center;
    struct color ambient;
    struct color diffuse;
    struct color specular;
};

struct light {
    struct color ambient;
    struct color diffuse;
    struct color specular;
    struct point position;
};

#endif /* objects.h */
