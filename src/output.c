//
// Created by aalvarez on 4/01/25.
//

#include <ft_ls.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <asm-generic/errno-base.h>

/**
 * Prints an error message to standard error based on the provided error code and cause.
 *
 * This function handles displaying error messages for different error codes.
 * It either prints a custom message or utilizes `perror` for system error reporting.
 *
 * @param error_code An integer representing the error code.
 * @param cause A string representing the cause of the error, such as the invalid option or inaccessible file.
 */
void
print_error_msg(const int error_code, const char *cause) {
    char *msg = NULL, *tmp = NULL;

    if (error_code == ESERIOUS) {
        tmp = ft_strjoin("ft_ls: unrecognized option '", (char[2]){cause[0], '\0'});
        msg = ft_strjoin(tmp, "'");
        ft_putendl_fd(msg, STDERR_FILENO);
    } else {
        if (error_code == ENOENT) {
            tmp = ft_strjoin("ft_ls: cannot access '", cause);
            msg = ft_strjoin(tmp, "'");
        }
        perror(msg);
    }
    free(msg);
    free(tmp);
}
