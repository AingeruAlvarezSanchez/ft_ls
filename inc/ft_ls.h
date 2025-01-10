//
// Created by aalvarez on 3/01/25.
//

#ifndef FT_LS_H
#define FT_LS_H
#include <dirent.h>
#include <flags.h>
#include <sys/stat.h>

// Error related
#define ENOFLAG 998
#define ESERIOUS 999

#define UNRECOGNIZED_OPTION "unrecognized option"
#define CANNOT_ACCESS "cannot access"
#define CANNOT_OPEN_DIRECTORY "cannot open directory"

// Flags related
#define ALLOWED_FLAGS "alrtR"

/**
 * @typedef t_fileinfo
 * @brief Represents information about a file.
 *
 * This structure holds various properties about a file,
 * including its name, file status, a related code, and
 * pointers to neighboring elements for linked list manipulation.
 *
 * The fields are:
 * - `name`: Pointer to the file name as a string.
 * - `file_status`: Stat structure containing metadata of the file
 *   (e.g., size, permissions, timestamps, etc.).
 * - `code`: An integer code that might represent an error or status.
 * - `next`: Pointer to the next t_fileinfo element in a linked list.
 * - `previous`: Pointer to the previous t_fileinfo element in a linked list.
 */
typedef struct s_fileinfo {
    /**
     * A character pointer representing the name of a file or entity.
     */
    char *name;
    /**
     * Represents the file status information captured from the file system.
     *
     * Commonly used with functions like `stat()` or `fstat()` to populate details
     * about a specific file or file descriptor.
     */
    struct stat file_status;
    /**
     * The `code` variable represents the status or error code related to the file
     * or process being performed. It is typically associated with a file, such as
     * a success status or an error indicator, used to determine further processing
     * or cleanup operations in the program.
     */
    int code;
    /**
     * Pointer to the next node in the linked list or collection
     * of s_fileinfo structs. This is used to traverse through
     * elements in the structure.
     */
    struct s_fileinfo *next;
    /**
     * Pointer to a structure of type s_fileinfo representing the previous file
     * in a linked list or chain. It allows traversal to the preceding element
     * in the list.
     */
    struct s_fileinfo *previous;
} t_fileinfo;

void
fileinfo_add_back(t_fileinfo **fileinfo, t_fileinfo *new);

t_fileinfo *
fileinfo_last(t_fileinfo *fileinfo);

t_fileinfo *
ft_fileinfo_new(char *name, struct stat file_status, int code);

void
fileinfo_del_one(t_fileinfo *fileinfo);

void
fileinfo_clear(t_fileinfo **fileinfo);

// Sorting related
typedef int (*t_compare)(const t_fileinfo *, const t_fileinfo *);

void
merge_sort(t_fileinfo **head_ref, t_compare cmp_function);

int
compare_name(const t_fileinfo *a, const t_fileinfo *b);

int
compare_mtime(const t_fileinfo *a, const t_fileinfo *b);

// Parser related
int
parse_arguments(int argc, const char **argv, t_program_params *params, t_fileinfo **fileinfo);

// Output related
void
print_error_msg(int error_code, const char *error_msg, const char *cause);

void
print_files(const t_program_params *params, t_fileinfo *fileinfo);

void
print_directory(const t_program_params *params, DIR *dir, t_compare cmp_function, const char *base_path);

// Core definition
int
ft_ls(t_fileinfo **fileinfo, const t_program_params *params, t_compare cmp_function, int print_header);

#endif //FT_LS_H
