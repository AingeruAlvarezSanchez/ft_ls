//
// Created by aalvarez on 4/01/25.
//

#include <errno.h>
#include <flags.h>
#include <ft_ls.h>
#include <stdlib.h>
#include <sys/stat.h>

/**
 * Processes a flag argument, validates its characters against allowed flags,
 * and sets corresponding flags in the program parameters.
 *
 * @param arg The flag argument to process. It must be a valid string starting
 *            with a '-' followed by one or more characters.
 * @param params A pointer to the program parameters structure where the
 *               flags will be set.
 */
void
add_flag(const char *arg, t_program_params *params, t_file *file) {
    for (int i = 1; arg[i] != '\0'; i++) {
        if (ft_strchr(ALLOWED_FLAGS, arg[i]) == NULL) {
            print_error_msg(ESERIOUS, &arg[i]);
            ft_lstclear(&file->files, free);
            ft_lstclear(&file->dirs, free);
            exit(2);
        }
        set_flag(arg[i], params);
    }
}

/**
 * Adds a file or directory to the appropriate list in the t_file structure.
 * If the given argument is a directory, it is added to the `dirs` list;
 * otherwise, it is added to the `files` list.
 *
 * @param arg The path of the file or directory to analyze and add.
 * @param file Pointer to a t_file structure containing lists of files and directories.
 * @return An integer status where 2 indicates an error (e.g., if the file or directory
 *         does not exist). Otherwise, returns the current status.
 */
int
add_file(const char *arg, t_file *file) {
    static int status;
    struct stat file_status;

    if (stat(arg, &file_status) == -1) {
        print_error_msg(errno, arg);
        status = 2;
    } else if (status != 2) {
        S_ISDIR(file_status.st_mode) ?
            ft_lstadd_back(&file->dirs, ft_lstnew(ft_strdup(arg))) :
            ft_lstadd_back(&file->files, ft_lstnew(ft_strdup(arg)));
    }
    return status;
}

/**
 * Parses command-line arguments and categorizes them into flags or file entries.
 *
 * @param argc The number of arguments passed to the program.
 * @param argv The array of arguments passed to the program.
 * @param params A pointer to the data structure where flags and other program parameters will be stored.
 * @param file A pointer to the data structure where directories and file entries will be stored.
 * @return An integer representing the status of the parsing. Returns EXIT_SUCCESS on success or an error code otherwise.
 */
int
parse_arguments(const int argc, const char **argv, t_program_params *params, t_file *file) {
    int status = EXIT_SUCCESS;

    if (argc == 1) {
        ft_lstadd_back(&file->dirs, ft_lstnew(ft_strdup("./")));
    } else {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] == '-' && argv[i][1] != '\0') {
                add_flag(argv[i], params, file);
            } else {
                status = add_file(argv[i], file);
            }
        }
    }
    return status;
}
