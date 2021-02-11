#ifndef CUB3D_H
# define CUB3D_H

#include <stdio.h>
#include "mlx.h"
#include "./mlx/mlx_int.h"
#include <stdlib.h>
#include <math.h>

typedef struct s_mlx{
    void *mlx;
    void *img;
    void *win;
    char *addr;
    int bits_per_pixel;
    int line_length;
    int endian;
}               t_mlx;

typedef struct s_flags{
    int w;
    int a;
    int s;
    int d;
    int r;
    int l;
}               t_flags;
typedef struct s_tex {
    void    *img;
    char    *addr;
    int     width;
    int     height;
    int     bpp;
    int     ll;
    int     endian;
}           t_tex;

typedef struct  s_drw{
    double  posX;
    double  posY;
    double  dirX;
    double  dirY;
    double  planeX;
    double  planeY;

    int     screenWidth;
    int     screenHeight;

    double  cameraX;
    double  rayDirX;
    double  rayDirY;
    int     mapX;
    int     mapY;

    double sideDistX;
    double sideDistY;
    double deltaDistX;
    double deltaDistY;
    double perpWallDist;

    int stepX;
    int stepY;

    int hit;
    int side;

    int lineHeight;
    int drawStart;
    int drawEnd;

    double oldDirX;
    double oldPlaneX;
    char *map;

    double moveSpeed;
    double rotSpeed;
}               t_drw;


typedef struct s_all{
    t_mlx   mlx;
    t_drw   drw;
    t_tex   tex;
    t_flags flags;
}               t_all;

#endif