//
// Created by aalvarez on 4/01/25.
//

#include <errno.h>
#include <flags.h>
#include <ft_ls.h>
#include <stdlib.h>

/**
 * @brief Process and add a series of flags from a command-line argument.
 *
 * This function iterates through each character in the given argument
 * (excluding the first '-' character), verifies if it is part of the allowed
 * flags, and sets the corresponding flag in the program's parameters. If an
 * invalid flag character is encountered, the function handles cleanup,
 * displays an error message, and terminates the program with an exit code.
 *
 * @param arg The command-line argument containing flags (e.g., "-abc").
 * @param params A pointer to the structure holding the program's parameter
 *               flags to be updated.
 * @param fileinfo A double pointer to the file information list, used for
 *                 cleanup in case invalid flags are encountered.
 */
void
add_flag(const char *arg, t_program_params *params, t_fileinfo **fileinfo) {
    for (int i = 1; arg[i]; i++) {
        if (ft_strchr(ALLOWED_FLAGS, arg[i]) == NULL) {
            fileinfo_clear(fileinfo);
            print_error_msg(ENOFLAG, UNRECOGNIZED_OPTION, (char[2]){arg[i], '\0'});
            exit(2);
        }
        set_flag(arg[i], params);
    }
}

/**
 * Adds a file to the `fileinfo` linked list, capturing its status information.
 * If the file does not exist or cannot be accessed, the error code is stored.
 *
 * @param arg The file path to be added.
 * @param fileinfo A pointer to the pointer of the fileinfo linked list where the file's information will be stored.
 * @return An integer status where `2` indicates an error occurred while retrieving the file's status, or the static status value.
 */
int
add_file(const char *arg, t_fileinfo **fileinfo) {
    static int status;
    struct stat file_status;

    if (lstat(arg, &file_status) == -1) {
        fileinfo_add_back(fileinfo, ft_fileinfo_new(ft_strdup(arg), file_status, errno));
        status = 2;
    } else {
        fileinfo_add_back(fileinfo, ft_fileinfo_new(ft_strdup(arg), file_status, 0));
    }
    return status;
}

/**
 * Removes invalid files from the given fileinfo list.
 *
 * This function iterates through the linked list of file information and removes
 * entries that are marked as invalid (i.e., have a non-zero error code).
 * For invalid entries, an error message is printed, and the entry is deleted.
 * The function adjusts the list to maintain proper linkage after removal of an invalid entry.
 *
 * @param fileinfo A pointer to the head of the file info linked list. This list
 *                 contains information about files, including names, error codes,
 *                 and next nodes in the list.
 */
void
clean_invalid_files(t_fileinfo **fileinfo) {
    t_fileinfo **current = fileinfo;

    while (*current) {
        t_fileinfo *next = (*current)->next;
        if ((*current)->code != 0) {
            print_error_msg((*current)->code, CANNOT_ACCESS, (*current)->name);
            fileinfo_del_one(*current);
            *current = next;
        } else {
            current = &(*current)->next;
        }
    }
}

/**
 * Parse the command-line arguments to populate program parameters and file information.
 *
 * This function processes the command-line arguments to extract flags, detect invalid files,
 * and prepare file information required for subsequent operations. If no arguments are provided,
 * it defaults to adding the current directory as a file.
 *
 * @param argc The number of arguments passed to the program, including the program name.
 * @param argv An array of null-terminated strings representing the command-line arguments.
 * @param params A pointer to a structure that holds the program parameters, such as flags.
 * @param fileinfo A pointer to a pointer to the head of the file information linked list.
 * @return An integer indicating the status of the operation:
 *         - EXIT_SUCCESS (0) if all arguments are successfully parsed.
 *         - A non-zero value, such as 2, if there are failures in processing arguments/files.
 */
int
parse_arguments(const int argc, const char **argv, t_program_params *params, t_fileinfo **fileinfo) {
    int status = EXIT_SUCCESS;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] != '\0') {
            add_flag(argv[i], params, fileinfo);
        } else {
            status = add_file(argv[i], fileinfo);
        }
    }
    if (*fileinfo == NULL) {
        struct stat file_status;
        lstat(".", &file_status);
        fileinfo_add_back(fileinfo, ft_fileinfo_new(ft_strdup("./"), file_status, 0));
    }
    clean_invalid_files(fileinfo);
    return status;
}
