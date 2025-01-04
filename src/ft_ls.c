//
// Created by aalvarez on 3/01/25.
//

#include <ft_ls.h>
#include <stdlib.h>

int
main(const int argc, const char **argv) {
    t_fileinfo *fileinfo = NULL;
    t_program_params params = {0, NULL, EXIT_SUCCESS};

    params.status = parse_arguments(argc, argv, &params, &fileinfo);
    fileinfo_clear(&fileinfo);
    return params.status;
}
