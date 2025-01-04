//
// Created by aalvarez on 3/01/25.
//

#include <flags.h>
#include <ft_ls.h>
#include <stdlib.h>

int
main(const int argc, const char **argv) {
    t_program_params params = {0, NULL, EXIT_SUCCESS};
    t_file file = {NULL, NULL};

    params.status = parse_arguments(argc, argv, &params, &file);
    return params.status;
}
