//
// Created by aalvarez on 3/01/25.
//

#ifndef FT_LS_H
#define FT_LS_H

#include <flags.h>

// Base program related
#define ALLOWED_FLAGS "alrtR"

/**
 * @typedef t_file
 * @brief Represents a structure to hold lists of files and directories.
 *
 * This structure contains two separate linked lists for managing
 * file paths and directory paths independently.
 *
 * @var files
 * Pointer to a linked list containing file paths.
 *
 * @var dirs
 * Pointer to a linked list containing directory paths.
 */
typedef struct s_file {
    /**
     * @brief A linked list to store files.
     *
     * This list is intended to keep track of file paths or file-related information.
     * It is used in operations like adding files or directories during processing.
     * Each element in the list contains a string representing the file path.
     */
    t_list *files;
    /**
     * @brief Linked list of directory paths.
     *
     * This list is used to store the paths of directories
     * that are added during the parsing process. Each entry
     * within the list represents a single directory path.
     *
     * @note This variable is utilized in conjunction with the `add_file`
     * function to differentiate between files and directories.
     */
    t_list *dirs;
} t_file;

// Error related
#define ESERIOUS 999

void
print_error_msg(int error_code, const char *cause);

// Parsing related
int
parse_arguments(int argc, const char **argv, t_program_params *params, t_file *file);

#endif //FT_LS_H
