//
// Created by aalvarez on 4/01/25.
//

#include <ft_ls.h>

/**
 * Compares the modification times of two files.
 *
 * The comparison is performed by first checking the `st_mtime` field in the
 * file status structure of each file. If the modification times are equal,
 * the comparison falls back to comparing the nanoseconds component in the
 * `st_mtim.tv_nsec` field for higher precision. If both time fields are equal,
 * the file names are compared using the `compare_name` function as a tiebreaker.
 *
 * @param a A pointer to the first file information structure containing file status information.
 * @param b A pointer to the second file information structure containing file status information.
 * @return An integer less than, equal to, or greater than zero if the modification time
 *         of the first file is, respectively, more recent than, equal to, or older than
 *         the modification time of the second file. If the modification times are identical,
 *         file names are compared as a secondary criterion.
 */
int
compare_mtime(const t_fileinfo *a, const t_fileinfo *b)
{
    if (a->file_status.st_mtime > b->file_status.st_mtime) {
        return -1;
    }
    if (a->file_status.st_mtime < b->file_status.st_mtime) {
        return 1;
    }
    if (a->file_status.st_mtim.tv_nsec > b->file_status.st_mtim.tv_nsec) {
        return -1;
    }
    if (a->file_status.st_mtim.tv_nsec < b->file_status.st_mtim.tv_nsec) {
        return 1;
    }
    return compare_name(a, b);
}

/**
 * Compares the names of two files in a case-insensitive manner.
 *
 * The comparison is performed by converting the file names to lowercase
 * and then using a string comparison function (`ft_strncmp`) to determine
 * the lexical order. The lengths of the strings are also considered to ensure
 * proper comparison.
 *
 * @param a A pointer to the first file information structure containing the file name.
 * @param b A pointer to the second file information structure containing the file name.
 * @return An integer less than, equal to, or greater than zero if the name of the first file
 *         is found, respectively, to be less than, match, or be greater than the name of the second file.
 */
int
compare_name(const t_fileinfo *a, const t_fileinfo *b) {
    return ft_strncmp(
        a->name,
        b->name,
        ft_strlen(a->name) > ft_strlen(b->name) ? ft_strlen(a->name) :
                                                  ft_strlen(b->name));
}

/**
 * Splits a linked list into two halves. The front half contains the first half
 * of the elements from the list, and the back half contains the remaining elements.
 *
 * This function modifies the input list by breaking it into two separate lists.
 * The split point is determined based on the length of the list. If the list has
 * an odd number of elements, the extra element is included in the front list.
 *
 * Additionally, the `previous` pointer of the first node in the back half is updated
 * to NULL, ensuring the split results in two independent sublists with proper
 * doubly-linked list consistency.
 *
 * @param source The pointer to the head of the list to be split.
 * @param front  The pointer to a pointer where the head of the front half will be stored.
 * @param back   The pointer to a pointer where the head of the back half will be stored.
 */
void
split_list(t_fileinfo *source, t_fileinfo **front, t_fileinfo **back) {
    t_fileinfo *slow = source;
    const t_fileinfo *fast = source->next;
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = source;
    *back = slow->next;
    if (*back != NULL) {
        (*back)->previous = NULL;
    }
    slow->next = NULL;
}

/**
 * Merges two sorted linked lists into a single sorted linked list.
 * This function assumes that both input lists are sorted according to
 * the given comparison function.
 *
 * During the merge process, the `previous` pointer of each node is updated
 * to correctly point to the previous node in the merged list, ensuring
 * the doubly-linked list integrity is maintained.
 *
 * If either input list is NULL, the resulting list's first node's `previous`
 * pointer is set to NULL.
 *
 * @param a Pointer to the head of the first sorted linked list.
 * @param b Pointer to the head of the second sorted linked list.
 * @param cmp_function Function pointer to the comparison function that
 *        determines the order of elements. The comparison function
 *        should return a value less than, equal to, or greater than 0
 *        to indicate whether the first argument is less than, equal to,
 *        or greater than the second argument, respectively.
 *
 * @return Pointer to the head of the newly merged sorted linked list.
 *         If both input lists are empty, the function returns NULL.
 */
t_fileinfo
*sorted_merge(t_fileinfo *a, t_fileinfo *b, const t_compare cmp_function) {
    if (a == NULL) {
        if (b != NULL) {
            b->previous = NULL;
        }
        return b;
    }
    if (b == NULL) {
        a->previous = NULL; // Ensure the first node's previous is NULL
        return a;
    }

    t_fileinfo *result = NULL;
    if (cmp_function(a, b) <= 0) {
        result = a;
        result->next = sorted_merge(a->next, b, cmp_function);
        if (result->next != NULL) {
            result->next->previous = result;
        }
    } else {
        result = b;
        result->next = sorted_merge(a, b->next, cmp_function);
        if (result->next != NULL) {
            result->next->previous = result;
        }
    }
    return result;
}

/**
 * Recursively sorts a linked list using the merge sort algorithm.
 *
 * After sorting, the `previous` pointer of the head of the final sorted list
 * is set to NULL, ensuring proper doubly-linked list behavior.
 * This relies on the logic in `split_list` and `sorted_merge` to maintain
 * doubly-linked list consistency throughout the split and merge steps.
 *
 * @param head_ref A double pointer to the head of the linked list to be sorted.
 *                 After sorting, the head pointer will point to the sorted list.
 * @param cmp_function A function pointer used for comparing two elements
 *                     in the linked list. The function should return an integer
 *                     less than, equal to, or greater than zero, depending on the
 *                     comparison.
 */
void
merge_sort(t_fileinfo **head_ref, const t_compare cmp_function) {
    t_fileinfo *head = *head_ref;
    t_fileinfo *a = NULL;
    t_fileinfo *b = NULL;

    if (head != NULL && head->next != NULL) {
        split_list(head, &a, &b);

        merge_sort(&a, cmp_function);
        merge_sort(&b, cmp_function);
        *head_ref = sorted_merge(a, b, cmp_function);
        if (*head_ref != NULL) {
            (*head_ref)->previous = NULL;
        }
    }
}