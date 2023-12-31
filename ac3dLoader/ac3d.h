/*  Torcs Car Setup Editor. A visual car setup editor for torcs build using glui
    Copyright (C) 2007  Vicente Marti

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#include <string>
typedef struct ACPoint_t 
{
    float x, y, z;
} ACPoint;


typedef struct ACNormal_t
{
    float x, y, z;
} ACNormal;


typedef struct ACVertex_t 
{
    float x, y, z;
    ACNormal normal;
} ACVertex;

typedef struct ACUV_t
{
    float u, v;
} ACUV;


typedef struct ACSurface_t
{
    int *vertref;
    ACUV *uvs;
    int num_vertref;
    ACNormal normal;
    int flags;
    int mat;
} ACSurface;

typedef struct ACObject_t
{
    ACPoint loc;
    char *name;
    char *data;
    char *url;
    ACVertex *vertices;
    int num_vert;

    ACSurface *surfaces;
    int num_surf;
    float texture_repeat_x, texture_repeat_y;
    float texture_offset_x, texture_offset_y;

    int num_kids;
    struct ACObject_t **kids;
    float matrix[9];
    int type;
    int texture;
} ACObject;

typedef struct ACCol_t
{
    float r, g, b, a;
} ACCol;

typedef struct Material_t
{
    ACCol rgb; /* diffuse **/
    ACCol ambient;
    ACCol specular;
    ACCol emissive;
    float shininess;
    float transparency;
    char *name;
} ACMaterial;

typedef struct ACImage_t
{
    unsigned short width, height, depth;    
    void *data; 
    int index;
    char *name;
    int amask;
    char *origname; /** do not set - set automatically in texture_read function **/

} ACImage;

#define OBJECT_WORLD 999
#define OBJECT_NORMAL 0
#define OBJECT_GROUP 1
#define OBJECT_LIGHT 2

#define SURFACE_SHADED (1<<4)
#define SURFACE_TWOSIDED (1<<5)

#define SURFACE_TYPE_POLYGON (0)
#define SURFACE_TYPE_CLOSEDLINE (1)
#define SURFACE_TYPE_LINE (2)

#define Prototype
#define Private static
#define Boolean int

#ifndef TRUE
#define FALSE (0)
#define TRUE (!FALSE)
#endif

#define STRING(s)  (char *)(strcpy((char *)myalloc(strlen(s)+1), s))
#define streq(a,b)  (!strcmp(a,b))
#define myalloc malloc
#define myfree free

//Prototypes
ACObject *ac_load_ac3d(std::string filename);
ACMaterial *ac_palette_get_material(int index);
ACImage *ac_get_texture(int ind);
int ac_load_texture(char *name);
int ac_load_rgb_image(char *fileName);
void ac_prepare_render(void);
int ac_display_list_render_object(ACObject *ob);

