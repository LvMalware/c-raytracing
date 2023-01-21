#ifndef __TOML_H
#define __TOML_H

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "objects.h"
#include "raycast.h"

void scene2toml_read(struct scene * const, FILE * const);
void scene2toml_write(struct scene * const, FILE * const);

#endif /* toml.h */
