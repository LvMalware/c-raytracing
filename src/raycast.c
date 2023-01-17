#include "raycast.h"

static inline double vnorm(const double x, const double y, const double z) {
    return sqrt(x * x + y * y + z * z);
}

static inline double vdot(const double x1, const double y1, const double z1,
                          const double x2, const double y2, const double z2) {
    return x1 * x2 + y1 * y2 + z1 * z2;
}

static inline double clip(const double v, const double l, const double h) {
    if (v > h)
        return h;
    else if (v < l)
        return l;
    else
        return v;
}

static inline void reflected(struct point * const vector,
                             struct point * const axis) {
    double product = vdot(vector->x, vector->y, vector->z,
                          axis->x, axis->y, axis->z);
    vector->x -= 2 * product * axis->x;
    vector->y -= 2 * product * axis->y;
    vector->z -= 2 * product * axis->z;
}

static inline void normalize(double * const x,
                             double * const y,
                             double * const z) {
    double norm = vnorm(*x, *y, *z);
    *x /= norm;
    *y /= norm;
    *z /= norm;
}

double intersect_cube(struct object * const cube,
                      struct point * const ray_origin,
                      struct point * const ray_direction) {
    return INFINITY;
}

double intersect_plane(struct object * const plane,
                       struct point * const ray_origin,
                       struct point * const ray_direction) {
    double t = -vdot(plane->normal.x, plane->normal.y, plane->normal.z,
                     ray_origin->x - plane->center.x,
                     ray_origin->y - plane->center.y,
                     ray_origin->z - plane->center.z) /
                vdot(plane->normal.x, plane->normal.y, plane->normal.z,
                     ray_direction->x, ray_direction->y, ray_direction->z);
    if (t >= 0) {
        return t;
    }
    return INFINITY;
}

double intersect_sphere(struct object * const sphere,
                        struct point * const ray_origin,
                        struct point * const ray_direction) {

    double a = 1.0; /*vdot(ray_direction->x, ray_direction->y,
                        ray_direction->z, ray_direction->x,
                        ray_direction->y, ray_direction->z); */
    double b = 2 * vdot(ray_direction->x, ray_direction->y, ray_direction->z,
                        ray_origin->x - sphere->center.x,
                        ray_origin->y - sphere->center.y,
                        ray_origin->z - sphere->center.z);
    double c = pow(vnorm(ray_origin->x - sphere->center.x,
                         ray_origin->y - sphere->center.y,
                         ray_origin->z - sphere->center.z), 2) -
               pow(sphere->radius, 2);
    double d = b * b - 4 * a * c;
    if (d > 0) {
        //printf("a=%lf b=%lf c=%lf delta=%lf\n", a, b, c, d);
        double t1 = (-b + sqrt(d)) / 2;
        double t2 = (-b - sqrt(d)) / 2;
        if (t1 > 0 && t2 > 0) return (t1 < t2) ? t1 : t2;
    }
    return INFINITY;
}

double intersect_cylinder(struct object * const cylinder,
                          struct point * const ray_origin,
                          struct point * const ray_direction) {
    double a = vdot(ray_direction->x, ray_direction->y, ray_direction->z,
                    ray_direction->x, ray_direction->y, ray_direction->z);
    double b = 2 * vdot(ray_direction->x, ray_direction->y, ray_direction->z,
                        ray_origin->x - cylinder->center.x,
                        ray_origin->y - cylinder->center.y,
                        ray_origin->z - cylinder->center.z);
    double c = pow(vnorm(ray_origin->x - cylinder->center.x,
                         ray_origin->y - cylinder->center.y,
                         ray_origin->z - cylinder->center.z), 2) -
               pow(cylinder->radius, 2);
    double d = b * b - 4 * a * c;
    if (d > 0) {
        double t1 = (-b + sqrt(d)) / (2 * a);
        double t2 = (-b - sqrt(d)) / (2 * a);
        double y1 = ray_origin->x + t1 * ray_direction->x;
        double y2 = ray_origin->x + t2 * ray_direction->x;
        if (y1 > 0 && y1 < cylinder->height) {
            if (t1 > 0) return t1;
        }
        if (y2 > 0 && y2 < cylinder->height) {
            if (t2 > 0) return t2;
        }
    }
    return INFINITY;
}

int intersect_nearest_object(struct object * const objects, size_t count,
                              struct point * const ray_origin,
                              struct point * const ray_direction,
                              size_t * const index, double * const distance) {
    size_t m = 0;
    double d = 0;
    double dist = INFINITY;
    size_t i;
    for (i = 0; i < count; i ++) {
        switch (objects[i].type) {
            case OBJECT_CUBE:
                d = intersect_cube(&objects[i], ray_origin, ray_direction);
                break;
            case OBJECT_PLANE:
                d = intersect_plane(&objects[i], ray_origin, ray_direction);
                break;
            case OBJECT_SPHERE:
                d = intersect_sphere(&objects[i], ray_origin, ray_direction);
                break;
            case OBJECT_CYLINDER:
                d = intersect_cylinder(&objects[i], ray_origin, ray_direction);
                break;
        }
        if (d < dist) {
            dist = d;
            m = i;
        }
    }
    *index = m;
    *distance = dist;
    return dist != INFINITY;
}

void scene_create(struct scene * const scene, int width, int height) {
    scene->width = width;
    scene->height = height;
    scene->objects = NULL;
    scene->max_depth = 3;
    scene->object_count = 0;

    scene->ratio = (double) width / (double) height;
    scene->light = (struct light) {
        .ambient  = { 1, 1, 1 },
        .diffuse  = { 1, 1, 1 },
        .specular = { 1, 1, 1 },
        .position = { 5, 5, 5 }
    };
    scene->camera = (struct point) {0, 0, 1};
    scene->screen[0] = -1.0;
    scene->screen[1] =  1.0 / scene->ratio;
    scene->screen[2] =  1.0;
    scene->screen[3] = -1.0 / scene->ratio;
}

void scene_render(struct scene * const scene,
                  struct bitmap_image * const image) {
    bitmap_image_init(image, scene->width, scene->height, 3000);
    size_t i, j;
    double step_x = (scene->screen[2] - scene->screen[0]) / (double) scene->width,
           step_y = (scene->screen[3] - scene->screen[1]) / (double) scene->height;
    double px = scene->screen[0];
    for (i = 0; i < scene->width; i ++) {
        double py = scene->screen[1];
        for (j = 0; j < scene->height; j ++) {
            struct point origin = scene->camera;
            struct point direction = {
                .x = px - origin.x,
                .y = py - origin.y,
                .z =  0 - origin.z
            };
            struct color color = {
                .r = 0,
                .g = 0,
                .b = 0
            };
            normalize(&direction.x, &direction.y, &direction.z);
            double reflection = 1;
            int k;
            for (k = 0; k < scene->max_depth; k ++) {
                size_t obj = 0;
                double dist = INFINITY;
                if (!intersect_nearest_object(scene->objects,
                                              scene->object_count,
                                              &origin, &direction,
                                              &obj, &dist)) break;
                //printf("Intersection with object #%lu (distance: %lf)\n", obj, dist);

                struct point intersection = {
                    .x = origin.x + dist * direction.x,
                    .y = origin.y + dist * direction.y,
                    .z = origin.z + dist * direction.z
                };

                struct point normal_surface = {
                    .x = intersection.x - scene->objects[obj].center.x,
                    .y = intersection.y - scene->objects[obj].center.y,
                    .z = intersection.z - scene->objects[obj].center.z
                };

                normalize(&normal_surface.x,
                          &normal_surface.y,
                          &normal_surface.z);

                struct point shifted_point = {
                    .x = intersection.x + 1e-5 * normal_surface.x,
                    .y = intersection.y + 1e-5 * normal_surface.y,
                    .z = intersection.z + 1e-5 * normal_surface.z
                };

                struct point light_intersect = {
                    .x = scene->light.position.x - shifted_point.x,
                    .y = scene->light.position.y - shifted_point.y,
                    .z = scene->light.position.z - shifted_point.z
                };

                normalize(&light_intersect.x,
                          &light_intersect.y,
                          &light_intersect.z);

                size_t obj2 = 0;
                double dist2 = INFINITY;
                intersect_nearest_object(scene->objects, scene->object_count,
                                         &shifted_point, &light_intersect,
                                         &obj2, &dist2);

                double ldist = vnorm(scene->light.position.x - intersection.x,
                                     scene->light.position.y - intersection.y,
                                     scene->light.position.z - intersection.z);
                if (dist2 < ldist) break;

                double foo = vdot(light_intersect.x, light_intersect.y,
                                  light_intersect.z, normal_surface.x,
                                  normal_surface.y, normal_surface.z);

                struct color illumination = {
                    .r = scene->objects[obj].ambient.r *
                         scene->light.ambient.r +
                         scene->objects[obj].diffuse.r *
                         scene->light.diffuse.r * foo,
                    .g = scene->objects[obj].ambient.g *
                         scene->light.ambient.g +
                         scene->objects[obj].diffuse.g *
                         scene->light.diffuse.g * foo,
                    .b = scene->objects[obj].ambient.b *
                         scene->light.ambient.b +
                         scene->objects[obj].diffuse.b *
                         scene->light.diffuse.b * foo
                };

                struct point camera_intersect = {
                    .x = scene->camera.x - intersection.x,
                    .y = scene->camera.y - intersection.y,
                    .z = scene->camera.z - intersection.z
                };

                normalize(&camera_intersect.x,
                          &camera_intersect.y,
                          &camera_intersect.z);

                struct point H = {
                    .x = light_intersect.x + camera_intersect.x,
                    .y = light_intersect.y + camera_intersect.y,
                    .z = light_intersect.z + camera_intersect.z
                };

                normalize(&H.x, &H.y, &H.z);
                foo = pow(vdot(normal_surface.x, normal_surface.y,
                               normal_surface.z, H.x, H.y, H.z),
                          scene->objects[obj].shineness / 4);

                illumination.r += scene->objects[obj].specular.r *
                                  scene->light.specular.r * foo;
                illumination.g += scene->objects[obj].specular.g *
                                  scene->light.specular.g * foo;
                illumination.b += scene->objects[obj].specular.b *
                                  scene->light.specular.b * foo;

                color.r += reflection * illumination.r;
                color.g += reflection * illumination.g;
                color.b += reflection * illumination.b;
                reflection *= scene->objects[obj].reflection;
                origin = shifted_point;
                reflected(&direction, &normal_surface);
            }

            size_t pos = i * scene->width + j;
            image->pixels[pos].r = 255 * clip(color.r, 0, 1.0);
            image->pixels[pos].g = 255 * clip(color.g, 0, 1.0);
            image->pixels[pos].b = 255 * clip(color.b, 0, 1.0);
            /* if (image->pixels[pos].r != 0 || image->pixels[pos].g != 0 ||
                image->pixels[pos].b != 0)
                printf("Pixel(%lu, %lu) = RGB(%d, %d, %d)\n", i, j,
                       image->pixels[pos].r, image->pixels[pos].g,
                       image->pixels[pos].b); */
            py += step_y;
        }
        px += step_x;
    }
    printf("Done here?\n");
}

void scene_object(struct scene * const scene, struct object * const obj) {
    struct object *vec = realloc(scene->objects,
                                 sizeof(*vec) * (scene->object_count + 1));
    if (vec == NULL) return;
    scene->objects = vec;
    scene->objects[scene->object_count] = *obj;
    scene->object_count += 1;
}

void scene_destroy(struct scene * const scene) {
    free(scene->objects);
    scene->objects = NULL;
    scene->object_count = 0;
}
