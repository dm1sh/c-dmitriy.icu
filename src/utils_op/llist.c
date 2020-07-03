#include "../../include/utils_op/llist.h"

/**
 * @brief Find item by number
 * 
 * @param list 
 * @param n 
 * @return llist_t* 
 */
llist_t *find_item(llist_t *list, int n)
{
    for (int i = 0; i < n; i++)
        list = list->next;
    return list;
}

/**
 * @brief Fill llist with array
 * 
 * @param list 
 * @param arr 
 * @param n 
 */
void fill_with_arr(llist_t **list, char **arr, size_t n)
{
    llist_t *current = *list;
    for (int i = 0; i < n; i++)
    {
        current->value = arr[i];
        current = current->next;
    }
}

/**
 * @brief Print whole list
 * 
 * @param list 
 */
void print_llist(llist_t *list)
{
    llist_t *current = list;
    int i = 0;

    while (current != NULL)
    {
        printf("%d - %s\n", i + 1, current->value);
        current = current->next;
        i++;
    }

    printf("\n");
}

/**
 * @brief Add value to the specified pos of llist
 * 
 * @param head 
 * @param pos 
 * @param value 
 * @return llist_t* 
 */
llist_t *add_to_list(llist_t **head, int pos, char *value)
{
    if (pos == 1)
    {
        llist_t *new = malloc(sizeof(llist_t));
        new->value = value;
        new->next = *head;

        *head = new;
        return new;
    }

    llist_t *current = *head;

    for (int i = 0; i < pos - 2; i++)
        current = current->next;

    llist_t *next = current->next;

    current->next = malloc(sizeof(llist_t));
    current->next->value = value;
    current->next->next = next;

    return current->next;
}

// Example of usage
// int main()
// {
//     llist_t *head = malloc(sizeof(llist_t));

//     head->value = "first";

//     head->next = malloc(sizeof(llist_t));
//     head->next->value = "second";

//     head->next->next = malloc(sizeof(llist_t));
//     head->next->next->value = "third";

//     head->next->next->next = malloc(sizeof(llist_t));
//     head->next->next->next->value = "forth";

//     head->next->next->next->next = malloc(sizeof(llist_t));
//     head->next->next->next->next->value = "fifth";

//     head->next->next->next->next->next = malloc(sizeof(llist_t));
//     head->next->next->next->next->next->value = "sixth";
//     head->next->next->next->next->next->next = NULL;

//     print_llist(head);

//     add_to_list(&head, 9, "Hola");

//     print_llist(head);

//     return 0;
// }