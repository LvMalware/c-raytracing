/*
 * Copyright 2023 Lucas V. Araujo (https://github.com/lvmalware)
 *
 * This file is part of c-raytracing.
 * c-raytracing is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * c-raytracing is distributed in the hope that it will be useful,but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this file. If not, see <https://www.gnu.org/licenses/>.
 */

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

    double half = cube->height / 2;
    double min_x = cube->center.x - half;
    double min_y = cube->center.y - half;
    double min_z = cube->center.z - half;
    double max_x = cube->center.x + half;
    double max_y = cube->center.y + half;
    double max_z = cube->center.z + half;

    double tx1 = (min_x - ray_origin->x) / ray_direction->x;
    double tx2 = (max_x - ray_origin->x) / ray_direction->x;
    double ty1 = (min_y - ray_origin->y) / ray_direction->y;
    double ty2 = (max_y - ray_origin->y) / ray_direction->y;
    double tz1 = (min_z - ray_origin->z) / ray_direction->z;
    double tz2 = (max_z - ray_origin->z) / ray_direction->z;

    double tmin = fmax(fmax(fmin(tx1, tx2), fmin(ty1, ty2)), fmin(tz1, tz2));
    double tmax = fmin(fmin(fmax(tx1, tx2), fmax(ty1, ty2)), fmax(tz1, tz2));

    if (tmin < tmax) {
        if (tmin < 0) tmin = tmax;
        if (tmin >= 0) return tmin;
    }
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
    if (t >= 0) return t;
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
        double t1 = (-b + sqrt(d)) / 2;
        double t2 = (-b - sqrt(d)) / 2;
        if (t1 > 0 && t2 > 0) return (t1 < t2) ? t1 : t2;
    }
    return INFINITY;
}

double intersect_cylinder(struct object * const cylinder,
                           struct point * const ray_origin,
                           struct point * const ray_direction) {
    double r  = cylinder->radius, h = cylinder->height;
    double cx = cylinder->center.x,
           cy = cylinder->center.y,
           cz = cylinder->center.z;

    double ox = ray_origin->x,
           oy = ray_origin->y,
           oz = ray_origin->z;

    double dx = ray_direction->x,
           dy = ray_direction->y,
           dz = ray_direction->z;

    double a = dx * dx + dy * dy;
    double b = 2 * (dx * (ox - cx) + dy * (oy - cy));
    double c = pow(ox - cx, 2) + pow(oy - cy, 2) - r * r;

    double d = b * b - 4 * a * c;
    if (d < 0) return INFINITY;
    double t0 = (-b - sqrt(d)) / (2 * a);
    double t1 = (-b + sqrt(d)) / (2 * a);

    if (t1 < 0 || t0 < 0) return INFINITY;

    double x0 = ox + t0 * dx;
    double y0 = oy + t0 * dy;
    double z0 = oz + t0 * dz;
    double x1 = ox + t1 * dx;
    double y1 = oy + t1 * dy;
    double z1 = oz + t1 * dz;

    /* check intersection using dot product with normal of the cylinder */
    double dp0 = pow(x0 - cx, 2) + pow(y0 - cy, 2) + pow(z0 - cz, 2);
    double dp1 = pow(x1 - cx, 2) + pow(y1 - cy, 2) + pow(z1 - cz, 2);
    if (dp0 <= r * r && z0 <= cz + h && z0 >= cz) return t0;
    if (dp1 <= r * r && z1 <= cz + h && z1 >= cz) return t0;

    /* project intersection point on the cylinder axis */
    double p0 = (x0 - cx) * dx + (y0 - cy) * dy;
    double p1 = (x1 - cx) * dx + (y1 - cy) * dy;
    if (p0 <= r && p0 >= -r && z0 <= cz + h && z0 >= cz) return t0;
    if (p1 <= r && p1 >= -r && z1 <= cz + h && z1 >= cz) return t1;

    if (dx == 0 && dy == 0) {
        if (fabs(ox - cx) > r || oz > cz + h || oz < cz) return INFINITY;
        return (cz - oz) / dz;
    }

    double t_cap0 = (cz + h - oz) / dz;

    if (t_cap0 > t0 && t_cap0 < t1) {
        double x = ox + t_cap0 * dx;
        double y = oy + t_cap0 * dy;
        if (pow(x - cx, 2) + pow(y - cy, 2) <= r * r) return t_cap0;
    }

    double t_cap1 = (cz - oz) / dz;
    if (t_cap1 > t0 && t_cap1 < t1) {
        double x = ox + t_cap1 * dx;
        double y = oy + t_cap1 * dy;
        if (pow(x - cx, 2) + pow(y - cy, 2) <= r * r) return t_cap1;
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

            py += step_y;
        }
        px += step_x;
    }
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
