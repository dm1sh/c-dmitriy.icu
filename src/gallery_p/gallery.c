#include "../../include/gallery_p/gallery.h"
#include "../../include/utils_op/utils.h"
#include "../../include/file_op/file.h"
#include "../../include/utils_op/arr.h"

/**
 * @brief Get the list of albums on server
 * 
 * @return gallery_t* 
 */
gallery_t *get_album_list()
{
    gallery_t *list = NULL, *curr = list;

    char **albums_titles_list = malloc(0);

    ssize_t albums_am = get_dir_list(&albums_titles_list, GALLERY_ROOT);

    if (albums_am < 0)
    {
        err_msg("couldn't read albums list");
        return list;
    }
    if (albums_am == 0)
        return list;

    list = new_album_item(albums_titles_list[0]);
    get_album_imgs(&list->images, &list->img_am, albums_titles_list[0]);

    curr = list;

    for (unsigned int i = 1; i < albums_am; i++)
    {
        curr->next = new_album_item(albums_titles_list[i]);

        curr = curr->next;

        get_album_imgs(&curr->images, &curr->img_am, albums_titles_list[i]);
    }

    free_arr(albums_titles_list, albums_am);

    return list;
}

/**
 * @brief Free gallery_t structure
 * 
 * @return void
 */
void free_albums_list(gallery_t *albums_list)
{
    gallery_t *curr_item = albums_list;

    while (curr_item != NULL)
    {
        free(curr_item->title);
        for (int i = 0; i < curr_item->img_am; i++)
            free_img_item(curr_item->images[i]);
        free(curr_item->images);
        curr_item = curr_item->next;
    }

    free(albums_list);
}

/**
 * @brief Generates new album item
 * 
 * @param title 
 * @return gallery_t* 
 */
gallery_t *new_album_item(char *folder_name)
{
    gallery_t *new = calloc(1, sizeof(gallery_t));

    char *title = strdup(folder_name);
    title[strlen(title) - 1] = '\0';

    new->title = strdup(repair_spaces(title));
    free(title);

    new->img_am = 0;

    new->images = NULL;
    new->next = NULL;

    return new;
}

/**
 * @brief Generates new image item
 * 
 * @param path 
 * @return img_t 
 */
img_t new_img_item(char *path)
{
    img_t img;

    img.date_taken = 0;
    img.description = NULL;
    img.path = strdup(path);
    img.rating = 0;
    img.tags = NULL;

    return img;
}

/**
 * @brief Free img_t structure
 * 
 * @return void
 */
void free_img_item(img_t img)
{
    free(img.description);
    free(img.path);
    free(img.tags);
}

int get_album_imgs(img_t **images_arr, int *size, char *title)
{
    char **photos_list = calloc(0, sizeof(char *));

    char *album_path = strdup(GALLERY_ROOT);
    album_path = realloc(album_path, strlen(album_path) + strlen(title) + 1);
    album_path = strcat(album_path, title);

    ssize_t photos_am = get_dir_list(&photos_list, album_path);
    free(album_path);

    *images_arr = calloc(photos_am, sizeof(img_t));

    for (int j = 0; j < photos_am; j++)
    {
        char *img_path = strdup("/gallery/albums/");
        img_path = realloc(img_path, strlen(img_path) + strlen(title) + strlen(photos_list[j]) + 1);
        img_path = strcat(img_path, title);
        img_path = strcat(img_path, photos_list[j]);

        (*images_arr)[j] = new_img_item(img_path);

        free(img_path);
    }

    *size = photos_am;

    free_arr(photos_list, photos_am);

    return photos_am;
}

char *gen_gallery_html()
{
    FILE *album_template_fp = fopen("static/gallery/album.html", "r");
    FILE *image_template_fp = fopen("static/gallery/image.html", "r");

    size_t album_file_size = get_file_size(album_template_fp) + 1;
    char *album_template = calloc(1, album_file_size);

    size_t image_file_size = get_file_size(image_template_fp) + 1;
    char *image_template = calloc(1, image_file_size);

    fread(album_template, album_file_size, 1, album_template_fp);
    fclose(album_template_fp);

    fread(image_template, image_file_size, 1, image_template_fp);
    fclose(image_template_fp);

    gallery_t *albums_list = get_album_list();
    gallery_t *albums_list_item = albums_list;

    char *gallery_content = strdup("");

    while (albums_list_item != NULL)
    {
        if (albums_list_item->img_am <= 0)
        {
            albums_list_item = albums_list_item->next;
            continue;
        }

        char *album_content = strdup("");

        for (int i = 0; i < albums_list_item->img_am; i++)
        {
            char *link = albums_list_item->images[i].path;
            size_t line_length = snprintf(NULL, 0, image_template, link, link) + 1;

            char *image_content = calloc(1, line_length);
            if (image_content == NULL)
                return "500 Internal Error\n";

            sprintf(image_content, image_template, link, link);

            char *tmp = realloc(album_content, strlen(album_content) + strlen(image_content) + 1);
            if (tmp == NULL)
                return "500 Internal Error\n";

            album_content = tmp;
            album_content = strcat(album_content, image_content);

            free(image_content);
        }

        size_t line_length = snprintf(NULL, 0, album_template, albums_list_item->title, album_content) + 1;

        char *album_html = calloc(1, line_length);
        if (album_html == NULL)
            return "500 Internal Error\n";

        sprintf(album_html, album_template, albums_list_item->title, album_content);

        gallery_content = realloc(gallery_content, strlen(gallery_content) + line_length);

        gallery_content = strcat(gallery_content, album_html);

        free(album_content);
        free(album_html);
        free(image_template);

        albums_list_item = albums_list_item->next;
    }

    free(album_template);
    free_albums_list(albums_list);

    return gallery_content;
}