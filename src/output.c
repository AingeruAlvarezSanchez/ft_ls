//
// Created by aalvarez on 4/01/25.
//

#include <ft_ls.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * Prints the names of files from a linked list of file information structures.
 *
 * This function iterates through a linked list pointed to by the given `fileinfo`
 * parameter and prints the `name` of each file to the standard output stream.
 * Each file name is followed by two spaces. After printing all file names in
 * the list, the function outputs a newline character. If the provided `fileinfo`
 * pointer is `NULL`, no output is produced.
 *
 * @param fileinfo A pointer to the head of a linked list of file information
 * structures (`t_fileinfo`). Each node in the list contains the file metadata,
 * including its name.
 */
void
print_files(const t_fileinfo *fileinfo) {
    const t_fileinfo *current = fileinfo;
    if (fileinfo != NULL) {
        while (current) {
            ft_putstr_fd(current->name, STDOUT_FILENO);
            ft_putstr_fd("  ", STDOUT_FILENO);
            current = current->next;
        }
        ft_putchar_fd('\n', STDOUT_FILENO);
    }
}

/**
 * Reads and processes the contents of a directory using the provided comparison function.
 *
 * This function iterates through the entries in the given directory stream and collects
 * information about each directory entry. The collected file information is then sorted
 * using the specified comparison function before being printed and cleared from memory.
 *
 * The function performs the following steps:
 * - Reads entries in the directory stream using readdir.
 * - For each valid entry, gathers its file information using stat and
 *   stores the collected data in a linked list structure.
 * - If any entries are collected, sorts them using merge_sort with the
 *   provided comparison function.
 * - Prints the sorted file information using print_files.
 * - Cleans up allocated memory by clearing the list of collected file information.
 *
 * @param dir A pointer to an open directory stream to be read.
 *            The directory stream must be valid and previously opened using opendir.
 * @param cmp_function A function pointer that defines the comparison logic used for sorting
 *                     the directory contents. The function must accept two parameters of type
 *                     t_fileinfo and return an integer indicating the sorting order.
 */
void
print_directory(DIR *dir, const t_compare cmp_function) {
    struct dirent *entry;
    t_fileinfo *contents = NULL;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            struct stat file_status;
            stat(entry->d_name, &file_status);
            fileinfo_add_back(&contents, ft_fileinfo_new(ft_strdup(entry->d_name), file_status, 0));
        }
    }
    if (contents != NULL) {
        merge_sort(&contents, cmp_function);
        print_files(contents);
        fileinfo_clear(&contents);
    }
}

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
