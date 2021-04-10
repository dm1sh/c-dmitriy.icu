#ifndef _LIST_GAL_H
#define _LIST_GAL_H

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char *description;
    char *path;
    char **tags;
    time_t date_taken;
    short int rating;
} img_t;

typedef struct album_s
{
    char *title;
    img_t *images;
    int img_am;
    struct album_s *next;
} gallery_t;

gallery_t *get_album_list();
gallery_t *new_album_item(char *title);
void free_albums_list(gallery_t *albums_list);
img_t new_img_item(char *path);
void free_img_item(img_t img);
int get_album_imgs(img_t **images_arr, int *size, char *title);
char *gen_gallery_html();

#define GALLERY_ROOT "static/gallery/albums/"

#endif