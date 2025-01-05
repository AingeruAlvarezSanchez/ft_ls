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

// File info related
typedef struct s_fileinfo {
    char *name;
    struct stat file_status;
    int code;
    struct s_fileinfo *next;
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

// Parser related
int
parse_arguments(int argc, const char **argv, t_program_params *params, t_fileinfo **fileinfo);

// Output related
void
print_error_msg(int error_code, const char *error_msg, const char *cause);

void
print_files(const t_fileinfo *fileinfo);

void
print_directory(DIR *dir, t_compare cmp_function);

#endif //FT_LS_H
