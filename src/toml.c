#include "toml.h"

void scene2toml_read(struct scene * const sce, FILE * const fptr) {
    sce->objects = NULL;
    sce->object_count = 0;
    size_t size = 1024;
    char *buffer = malloc(size * sizeof(*buffer));
    ssize_t nread = 0;
    while ((nread = getline(&buffer, &size, fptr)) != -1) {
        if (nread < 2) break;
        buffer[nread - 1] = '\0';
        char *ptr = buffer;
        while (*ptr == ' ' || *ptr == '\t') ptr++;
        size_t len = strcspn(ptr, " =");
        char *start = ptr + len + strspn(ptr + len, " =");
        if (strcmp("[scene]", buffer) == 0) {
            continue;
        } else if (strncmp("height", ptr, len) == 0) {
            sscanf(start, "%d", &(sce->height));
        } else if (strncmp("width", ptr, len) == 0) {
            sscanf(start, "%d", &(sce->width));
        } else if (strncmp("depth", ptr, len) == 0) {
            sscanf(start, "%d", &(sce->max_depth));
        } else if (strncmp("light.ambient", ptr, len) == 0) {
            sscanf(start, "[%f,%f,%f]",
                   &(sce->light.ambient.r),
                   &(sce->light.ambient.g),
                   &(sce->light.ambient.b));
        } else if (strncmp("light.position", ptr, len) == 0) {
            sscanf(start, "[%lf,%lf,%lf]",
                   &(sce->light.position.x),
                   &(sce->light.position.y),
                   &(sce->light.position.z));
        } else if (strncmp("light.diffuse", ptr, len) == 0) {
            sscanf(start, "[%f,%f,%f]",
                   &(sce->light.diffuse.r),
                   &(sce->light.diffuse.g),
                   &(sce->light.diffuse.b));
        } else if (strncmp("light.specular", ptr, len) == 0) {
            sscanf(start, "[%f,%f,%f]",
                   &(sce->light.specular.r),
                   &(sce->light.specular.g),
                   &(sce->light.specular.b));
        } else if (strncmp("camera", ptr, len) == 0) {
            sscanf(start, "[%lf,%lf,%lf]",
                   &(sce->camera.x),
                   &(sce->camera.y),
                   &(sce->camera.z));
        }
    }

    sce->ratio = (double) sce->width / (double) sce->height;
    sce->screen[0] = -1.0;
    sce->screen[1] =  1.0 / sce->ratio;
    sce->screen[2] =  1.0;
    sce->screen[3] = -1.0 / sce->ratio;

    struct object obj;
    int read = 0;
    while ((nread = getline(&buffer, &size, fptr)) != -1) {
        if (nread < 2) {
            if (read) { 
                scene_object(sce, &obj);
                read = 0;
            }
            continue;
        }
        buffer[nread - 1] = '\0';
        char *ptr = buffer;
        while (*ptr == ' ' || *ptr == '\t') ptr++;
        size_t len = strcspn(ptr, " =");
        char *start = ptr + len + strspn(ptr + len, " =");

        if (strncmp("[objects.", ptr, 9) == 0) {
            if (read) {
                scene_object(sce, &obj);
                read = 0;
            } else {
                read = 1;
            }
        } else if (strncmp("radius", ptr, len) == 0) {
            sscanf(start, "%lf", &(obj.radius));
        } else if (strncmp("height", ptr, len) == 0) {
            sscanf(start, "%lf", &(obj.height));
        } else if (strncmp("shineness", ptr, len) == 0) {
            sscanf(start, "%lf", &(obj.shineness));
        } else if (strncmp("reflection", ptr, len) == 0) {
            sscanf(start, "%lf", &(obj.reflection));
        } else if (strncmp("type", ptr, len) == 0) {
            sscanf(start, "%hhu", &(obj.type));
        } else if (strncmp("center", ptr, len) == 0) {
            sscanf(start, "[%lf,%lf,%lf]", &(obj.center.x),
                   &(obj.center.y), &(obj.center.z));
        } else if (strncmp("normal", ptr, len) == 0) {
            sscanf(start, "[%lf,%lf,%lf]",
                   &(obj.normal.x),
                   &(obj.normal.y),
                   &(obj.normal.z));
        } else if (strncmp("ambient", ptr, len) == 0) {
            sscanf(start, "[%f,%f,%f]",
                   &(obj.ambient.r),
                   &(obj.ambient.g),
                   &(obj.ambient.b));
        } else if (strncmp("diffuse", ptr, len) == 0) {
            sscanf(start, "[%f,%f,%f]",
                   &(obj.diffuse.r),
                   &(obj.diffuse.g),
                   &(obj.diffuse.b));
        } else if (strncmp("specular", ptr, len) == 0) {
            sscanf(start, "[%f,%f,%f]",
                   &(obj.specular.r),
                   &(obj.specular.g),
                   &(obj.specular.b));
        }
    }

    if (read) scene_object(sce, &obj);
}

void scene2toml_write(struct scene * const sce, FILE * const fptr) {
    fprintf(fptr, "[scene]\n");
    fprintf(fptr, "depth = %d\n", sce->max_depth);
    fprintf(fptr, "width = %d\n", sce->width);
    fprintf(fptr, "height = %d\n", sce->height);
    fprintf(fptr, "light.ambient = [%lf, %lf, %lf]\n",
            sce->light.ambient.r,
            sce->light.ambient.g,
            sce->light.ambient.b);
    fprintf(fptr, "light.diffuse = [%lf, %lf, %lf]\n",
            sce->light.diffuse.r,
            sce->light.diffuse.g,
            sce->light.diffuse.b);
    fprintf(fptr, "light.specular = [%lf, %lf, %lf]\n",
            sce->light.specular.r,
            sce->light.specular.g,
            sce->light.specular.b);
    fprintf(fptr, "light.position = [%lf, %lf, %lf]\n",
            sce->light.position.x,
            sce->light.position.y,
            sce->light.position.z);
    fprintf(fptr, "camera = [%lf, %lf, %lf]\n\n",
            sce->camera.x,
            sce->camera.y,
            sce->camera.z);
    fprintf(fptr, "[objects]\n\n");
    size_t i;
    for (i = 0; i < sce->object_count; i ++) {
        struct object *obj = &(sce->objects[i]);
        fprintf(fptr, "[objects.%lu]\n", i);
        fprintf(fptr, "type = %u\n", obj->type);
        fprintf(fptr, "radius = %lf\n", obj->radius);
        fprintf(fptr, "height = %lf\n", obj->height);
        fprintf(fptr, "shineness = %lf\n", obj->shineness);
        fprintf(fptr, "reflection = %lf\n", obj->reflection);
        fprintf(fptr, "center = [%lf, %lf, %lf]\n",
                obj->center.x,
                obj->center.y,
                obj->center.z);
        fprintf(fptr, "normal = [%lf, %lf, %lf]\n",
                obj->normal.x,
                obj->normal.y,
                obj->normal.z);
        fprintf(fptr, "ambient = [%lf, %lf, %lf]\n",
                obj->ambient.r,
                obj->ambient.g,
                obj->ambient.b);
        fprintf(fptr, "diffuse = [%lf, %lf, %lf]\n",
                obj->diffuse.r,
                obj->diffuse.g,
                obj->diffuse.b);
        fprintf(fptr, "specular = [%lf, %lf, %lf]\n\n",
                obj->specular.r,
                obj->specular.g,
                obj->specular.b);
    }
}

