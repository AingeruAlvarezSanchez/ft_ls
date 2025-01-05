//
// Created by aalvarez on 3/01/25.
//

#include <errno.h>
#include <ft_ls.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Extracts non-directory files from the provided linked list of file information.
 *
 * This function scans through the linked list of file information, removes
 * entries corresponding to non directories, and builds a new linked list containing
 * only files.
 *
 * @param fileinfo A pointer to the head of a linked list of file information structures (`t_fileinfo`).
 *                 This list may be modified as non directory entries are removed.
 * @return A pointer to a new linked list (`t_fileinfo`) containing only non-directory files.
 *         The original list is updated to exclude the non-file entries.
 */
t_fileinfo *
extract_non_dir_files(t_fileinfo **fileinfo) {
    t_fileinfo *files_only = NULL;
    t_fileinfo **current = fileinfo;

    while (*current) {
        t_fileinfo *next = (*current)->next;
        if (!S_ISDIR((*current)->file_status.st_mode)) {
            t_fileinfo *to_move = *current;
            *current = next;
            to_move->next = files_only;
            files_only = to_move;
        } else {
            current = &(*current)->next;
        }
    }
    return files_only;
}


/**
 * Processes the given file information to sort and print directory contents,
 * handling errors for inaccessible directories.
 *
 * @param fileinfo Pointer to the linked list of file information structures. This list is sorted in place.
 * @param cmp_function Function pointer used to compare file information for sorting.
 * @param print_header Integer flag indicating whether to print directory headers before listing their contents.
 *                     A value of 1 enables the header print, 0 disables it.
 * @return Returns an integer status code. A value of 0 indicates success, while 2 indicates an error.
 */
int
ft_ls(t_fileinfo **fileinfo, const t_compare cmp_function, const int print_header) {
    static int status;
    (void)print_header;

    merge_sort(fileinfo, cmp_function);
    const t_fileinfo *current = *fileinfo;
    while (current) {
        DIR *dir = opendir(current->name);
        if (dir == NULL) {
            print_error_msg(errno, CANNOT_OPEN_DIRECTORY, current->name);
            status = 2;
        } else {
            if (current != *fileinfo) {
                ft_putchar_fd('\n', STDOUT_FILENO);
            }
            if (print_header == 1) {
                ft_putstr_fd(current->name, STDOUT_FILENO);
                ft_putendl_fd(":", STDOUT_FILENO);
            }
            print_directory(dir, cmp_function);
        }
        closedir(dir);
        current = current->next;
    }
    return status;
}

int
main(const int argc, const char **argv) {
    t_fileinfo *fileinfo = NULL;
    t_program_params params = {0, NULL, EXIT_SUCCESS};

    params.status = parse_arguments(argc, argv, &params, &fileinfo);

    const t_compare cmp_function = compare_name;
    t_fileinfo *files_only = extract_non_dir_files(&fileinfo);
    if (files_only != NULL) {
        merge_sort(&files_only, cmp_function);
        print_files(files_only);
    }

    if (fileinfo != NULL) {
        if (files_only != NULL) {
            ft_putchar_fd('\n', STDOUT_FILENO);
        }
        const int print_header = fileinfo->next != NULL || files_only != NULL ? 1 : 0;
        params.status = ft_ls(&fileinfo, cmp_function, print_header);
    }
    fileinfo_clear(&files_only);
    fileinfo_clear(&fileinfo);

    return params.status;
}
