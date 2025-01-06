//
// Created by aalvarez on 4/01/25.
//

#include <ft_ls.h>

/**
 * Converts all alphabetic characters of a string to lowercase while skipping
 * leading dots in the input string. The result is stored in the provided buffer.
 *
 * The function starts by ignoring all consecutive leading dots (`.`) in the
 * input string. It then processes the remaining characters by converting
 * alphabetic characters to their lowercase equivalent and copying them to
 * the provided buffer. The resulting string is null-terminated.
 *
 * @param str The input string to be converted to lowercase.
 *            It is assumed that the string is null-terminated.
 * @param buffer A pre-allocated buffer where the lowercase result will be stored.
 *               It should be large enough to hold the modified string, including
 *               the null-terminator.
 * @return A pointer to the buffer containing the lowercase result.
 */
static char *str_to_lower(const char *str, char *buffer) {
    int i = 0, j = 0;

    while (str[i] == '.')
        i++;
    while (str[i]) {
        buffer[j] = ft_tolower(str[i]);
        i++;
        j++;
    }
    buffer[j] = '\0';
    return buffer;
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
    char a_tolower[ft_strlen(a->name) + 1];
    char b_tolower[ft_strlen(b->name) + 1];
    str_to_lower(a->name, a_tolower);
    str_to_lower(b->name, b_tolower);
    return ft_strncmp(
        a_tolower,
        b_tolower,
        ft_strlen(a_tolower) > ft_strlen(b_tolower) ? ft_strlen(a_tolower) :
                                                      ft_strlen(b_tolower));
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