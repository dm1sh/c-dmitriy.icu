#include <stdio.h>

/** 
 * @brief Prints error
 * 
 * @param {char *} msg
*/
void err_msg (char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
}