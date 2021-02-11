#include "cub3D.h"

void            my_mlx_pixel_put(t_all *all, int x, int y, int color)
{
    char    *dst;

    dst = all->mlx.addr + (y * all->mlx.line_length + x * (all->mlx.bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

void draw_game(t_all *all){
    for (int x = 0; x < all->drw.screenWidth; x++){

        /*
         * calculate ray position and direction
         */
        all->drw.cameraX = 2 * x / (double)(all->drw.screenWidth - 1) - 1;
        all->drw.rayDirX = all->drw.dirX + all->drw.planeX * all->drw.cameraX;
        all->drw.rayDirY = all->drw.dirY + all->drw.planeY * all->drw.cameraX;

        /*
         * which box of the map we're in
         */
        all->drw.mapX = (int)(all->drw.posX);
        all->drw.mapY = (int)(all->drw.posY);

        all->drw.hit = 0;

        /*
         * length of ray from one x or y-side to next x or y-side
         */
        all->drw.deltaDistX = (all->drw.rayDirY == 0) ? 0 : ((all->drw.rayDirX == 0) ? 1 : fabs(1 / all->drw.rayDirX));
        all->drw.deltaDistY = (all->drw.rayDirX == 0) ? 0 : ((all->drw.rayDirY == 0) ? 1 : fabs(1 / all->drw.rayDirY));

        /*
         * calculate step and initial sideDist
         */
        if (all->drw.rayDirX < 0){
            all->drw.stepX = -1;
            all->drw.sideDistX = (all->drw.posX - all->drw.mapX) * all->drw.deltaDistX;
        }
        else{
            all->drw.stepX = 1;
            all->drw.sideDistX = (all->drw.mapX + 1.0 - all->drw.posX) * all->drw.deltaDistX;
        }
        if (all->drw.rayDirY < 0){
            all->drw.stepY = -1;
            all->drw.sideDistY = (all->drw.posY - all->drw.mapY) * all->drw.deltaDistY;
        }
        else{
            all->drw.stepY = 1;
            all->drw.sideDistY = (all->drw.mapY + 1.0 - all->drw.posY) * all->drw.deltaDistY;
        }

        while (all->drw.hit == 0){
            /*
             * jump to next map square OR in x-direction, OR in y-direction
             */
            if (all->drw.sideDistX < all->drw.sideDistY){
                all->drw.sideDistX += all->drw.deltaDistX;
                all->drw.mapX += all->drw.stepX;
                all->drw.side = 0;
            }
            else{
                all->drw.sideDistY += all->drw.deltaDistY;
                all->drw.mapY += all->drw.stepY;
                all->drw.side = 1;
            }

            /*
             * check if ray has hit a wall
             */
            if (all->drw.map[all->drw.mapY * 24 + all->drw.mapX] > 0)
                all->drw.hit = 1;
        }

        /*
         * claculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
         */
        if (all->drw.side == 0)
            all->drw.perpWallDist = (all->drw.mapX - all->drw.posX + (1 - all->drw.stepX) / 2.0) / all->drw.rayDirX;
        else
            all->drw.perpWallDist = (all->drw.mapY - all->drw.posY + (1 - all->drw.stepY) / 2.0) / all->drw.rayDirY;

        /*
         * calculate height of line to draw on screen
         */
        all->drw.lineHeight = (int)(all->drw.screenHeight / all->drw.perpWallDist);

        /*
         * calculate lowest and highest pixel to fill in current stripe
         */
        all->drw.drawStart = -all->drw.lineHeight / 2 + all->drw.screenHeight / 2;
        if (all->drw.drawStart < 0)
            all->drw.drawStart = 0;
        all->drw.drawEnd = all->drw.lineHeight / 2 + all->drw.screenHeight / 2;
        if (all->drw.drawEnd >= all->drw.screenHeight)
            all->drw.drawEnd = all->drw.screenHeight - 1;


        for (int i = 0; i < all->drw.drawStart; i++)
            my_mlx_pixel_put(all, x, i, 0x00000000);
        for (int i = all->drw.drawStart; i < all->drw.drawEnd; i++)
            if (all->drw.side == 1)
                my_mlx_pixel_put(all, x, i, 0x0000FF00);
            else
                my_mlx_pixel_put(all, x, i, 0x00FF0000);
        for (int i = all->drw.drawEnd; i < all->drw.screenHeight - 1; i++)
            my_mlx_pixel_put(all, x, i, 0x00FFFFFF);
    }
    mlx_put_image_to_window(all->mlx.mlx, all->mlx.win, all->mlx.img, 0, 0);
}

int key_press(int keycode, t_all *all){
    printf("< KEY_PRESS!!! >\n");

    if (keycode == 0xFF1B)
        exit(0);
    if (keycode == 'w'){
        all->flags.w = 'w';
    }
    if (keycode == 'a'){
        all->flags.a = 'a';
    }
    if (keycode == 's'){
        all->flags.s = 's';
    }
    if (keycode == 'd'){
        all->flags.d = 'd';
    }
    if (keycode == 65363){
        all->flags.r = 1;
    }
    if (keycode == 65361){
        all->flags.l = 1;
    }
//    printf("a = %d\ns = %d\nw = %d\nd = %d\n", all->flags.a, all->flags.s, all->flags.w, all->flags.d);
}

int key_release(int keycode, t_all *all){

    printf("< KEY_RELEASE!!! >\n");
    printf("%d\n", keycode);
    if (keycode == 'w'){
        all->flags.w = 0;
    }
    if (keycode == 'a'){
        all->flags.a = 0;
    }
    if (keycode == 's'){
        all->flags.s = 0;
    }
    if (keycode == 'd'){
        all->flags.d = 0;
    }
    if (keycode == 65363){
        all->flags.r = 0;
    }
    if (keycode == 65361){
        all->flags.l = 0;
    }
//    printf("a = %d\ns = %d\nw = %d\nd = %d\n", all->flags.a, all->flags.s, all->flags.w, all->flags.d);

}

int move(t_all *all){
    printf("< MOVE!!! >\n");
    if (all->flags.w == 'w'){
        if (all->drw.map[(int)(all->drw.posX + all->drw.dirX * all->drw.moveSpeed) + (int)(all->drw.posY) * 24] == 0)
            all->drw.posX += all->drw.dirX * all->drw.moveSpeed;
        if (all->drw.map[(int)(all->drw.posX) * 24 + (int)(all->drw.posY + all->drw.dirY * all->drw.moveSpeed)] == 0)
            all->drw.posY += all->drw.dirY * all->drw.moveSpeed;

    }
    if (all->flags.s == 's'){
        if (all->drw.map[(int)(all->drw.posX - all->drw.dirX * all->drw.moveSpeed) + (int)(all->drw.posY) * 24] == 0)
            all->drw.posX -= all->drw.dirX * all->drw.moveSpeed;
        if (all->drw.map[(int)(all->drw.posX) * 24 + (int)(all->drw.posY - all->drw.dirY * all->drw.moveSpeed)] == 0) {
            all->drw.posY -= all->drw.dirY * all->drw.moveSpeed;
        }
    }
    if (all->flags.d == 'd'){
        if (all->drw.map[(int)(all->drw.posX) * 24 + (int)(all->drw.posY - all->drw.dirX * all->drw.moveSpeed)] == 0)
            all->drw.posY -= all->drw.dirX * all->drw.moveSpeed;
        if (all->drw.map[(int)(all->drw.posX + all->drw.dirY * all->drw.moveSpeed) + (int)(all->drw.posY) * 24] == 0)
            all->drw.posX += all->drw.dirY * all->drw.moveSpeed;
    }
    if (all->flags.a == 'a'){
        if (!(all->drw.map[(int)(all->drw.posX - all->drw.dirY * all->drw.moveSpeed) + (int)(all->drw.posY) * 24]))
            all->drw.posX -= all->drw.dirY * all->drw.moveSpeed;
        if (!(all->drw.map[(int)(all->drw.posX) * 24 + (int)(all->drw.posY + all->drw.dirX * all->drw.moveSpeed)]))
            all->drw.posY += all->drw.dirX * all->drw.moveSpeed;
    }
    if(all->flags.r == 1){
        all->drw.oldDirX = all->drw.dirX;
        all->drw.dirX = all->drw.dirX * cos(-all->drw.rotSpeed) - all->drw.dirY * sin(-all->drw.rotSpeed);
        all->drw.dirY = all->drw.oldDirX * sin(-all->drw.rotSpeed) + all->drw.dirY * cos(-all->drw.rotSpeed);
        all->drw.oldPlaneX = all->drw.planeX;
        all->drw.planeX = all->drw.planeX * cos(-all->drw.rotSpeed) - all->drw.planeY * sin(-all->drw.rotSpeed);
        all->drw.planeY = all->drw.oldPlaneX * sin(-all->drw.rotSpeed) + all->drw.planeY * cos(-all->drw.rotSpeed);
    }
    if (all->flags.l == 1){
        all->drw.oldDirX = all->drw.dirX;
        all->drw.dirX = all->drw.dirX * cos(all->drw.rotSpeed) - all->drw.dirY * sin(all->drw.rotSpeed);
        all->drw.dirY = all->drw.oldDirX * sin(all->drw.rotSpeed) + all->drw.dirY * cos(all->drw.rotSpeed);
        all->drw.oldPlaneX = all->drw.planeX;
        all->drw.planeX = all->drw.planeX * cos(all->drw.rotSpeed) - all->drw.planeY * sin(all->drw.rotSpeed);
        all->drw.planeY = all->drw.oldPlaneX * sin(all->drw.rotSpeed) + all->drw.planeY * cos(all->drw.rotSpeed);
    }
    if (all->flags.a || all->flags.w || all->flags.s || all->flags.d || all->flags.r || all->flags.l)
        draw_game(all);
    return (1);
}
//int rotate(int keycode, ){
//
//}

int escape(int keycode, void *p){
    if (keycode == 0xFF1B)
        exit(0);
    return (1);
}

int main() {
    t_all all;
    void *img;
    char *addr;
    all.flags.a = 0;
    all.flags.w = 0;
    all.flags.s = 0;
    all.flags.d = 0;

    int worldMap[24][24]=
            {
                    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
                    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
                    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
            };

    if (!(all.drw.map = (char*)malloc(sizeof(char) * (24 * 24))))
        return (1);
    for (int i = 0; i < 24; i++)
        for (int j = 0; j < 24; j++)
            all.drw.map[i * 24 + j] = worldMap[i][j];
//    img = put

    all.drw.screenWidth = 720;
    all.drw.screenHeight = 480;
    all.drw.posX = 22;
    all.drw.posY = 12;
    all.drw.dirX = -1;
    all.drw.dirY = 0;
    all.drw.planeX = 0;
    all.drw.planeY = 0.66;
    all.drw.moveSpeed = 0.01;
    all.drw.rotSpeed = 0.005;

    all.mlx.mlx = mlx_init();
    all.mlx.win = mlx_new_window(all.mlx.mlx, all.drw.screenWidth, all.drw.screenHeight, "Hello world!");
    all.mlx.img = mlx_new_image(all.mlx.mlx, all.drw.screenWidth, all.drw.screenHeight);
    all.mlx.addr = mlx_get_data_addr(all.mlx.img, &all.mlx.bits_per_pixel, &all.mlx.line_length, &all.mlx.endian);
    draw_game(&all);
    printf("%d\n", all.drw.map[all.drw.mapY * 24 + all.drw.mapX]);
//    mlx_key_hook(all.mlx.win, move, &all);
    mlx_hook(all.mlx.win, KeyPress, KeyPressMask, key_press, &all);
    mlx_hook(all.mlx.win, KeyRelease, KeyReleaseMask, key_release, &all); //    mlx_key_hook(all.mlx.win, escape, &all);
    mlx_loop_hook(all.mlx.mlx, move, &all);
//    mlx_hook(all.mlx.mlx, 2, 1L<<0, move, &all);
//    mlx_loop_hook(all.mlx.mlx, draw_game, &all);
//    mlx_mouse_hook(all.mlx.win, rotate, &all);
    mlx_loop(all.mlx.mlx);
    return 0;
}