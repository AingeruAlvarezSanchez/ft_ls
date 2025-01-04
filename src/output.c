//
// Created by aalvarez on 4/01/25.
//

#include <ft_ls.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * Prints a formatted error message to the standard error stream.
 *
 * This function constructs an error message by combining a predefined
 * label, the provided error message, the cause, and optionally the
 * system error message corresponding to the given error code.
 * It ensures proper memory management by dynamically allocating and
 * freeing memory used for intermediate strings.
 *
 * @param error_code The error code representing the type of error.
 *        If error_code is not equal to ENOFLAG, the corresponding
 *        system error message (retrieved with strerror) will be included
 *        in the final output.
 * @param error_msg A descriptive message about the error to be displayed.
 * @param cause A string specifying the cause or related context of the error.
 */
void
print_error_msg(const int error_code, const char *error_msg, const char *cause) {
    char *tmp = ft_strjoin("ft_ls: ", error_msg);
    char *msg = ft_strjoin(tmp, " '");
    free(tmp);
    tmp = ft_strjoin(msg, cause);
    free(msg);
    msg = ft_strjoin(tmp, "'");
    free(tmp);

    if (error_code != ENOFLAG) {
        tmp = ft_strjoin(msg, ": ");
        free(msg);
        msg = ft_strjoin(tmp, strerror(error_code));
        free(tmp);
    }
    ft_putendl_fd(msg, STDERR_FILENO);
    free(msg);
}
