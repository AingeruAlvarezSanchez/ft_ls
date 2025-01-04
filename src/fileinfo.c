//
// Created by aalvarez on 4/01/25.
//

#include <ft_ls.h>
#include <stdlib.h>

/**
 * Creates a new t_fileinfo structure and initializes its attributes with the provided parameters.
 *
 * @param name The name of the file or directory.
 * @param file_status The file status information returned by the stat system call.
 * @param code An integer code representing the status or error code for the file.
 * @return A pointer to the newly allocated and initialized t_fileinfo structure.
 *         Returns NULL if memory allocation fails.
 */
t_fileinfo
*ft_fileinfo_new(char *name, const struct stat file_status, const int code) {
	t_fileinfo *new = malloc(sizeof(t_fileinfo));
	if (new != NULL) {
		new->name = name;
		new->code = code;
		new->file_status = file_status;
		new->next = NULL;
	}
	return new;

}

/**
 * Traverses the linked list starting from the given fileinfo node
 * and retrieves the last node in the list.
 *
 * @param fileinfo Pointer to the head of the linked list.
 * @return Pointer to the last node in the linked list.
 */
t_fileinfo
*fileinfo_last(t_fileinfo *fileinfo) {
	t_fileinfo *last = fileinfo;
	while (last->next != NULL) {
		last = last->next;
	}
	return last;
}

/**
 * Adds a new file info node to the end of a linked list of file info structures.
 *
 * If the file info list is empty (i.e., `*fileinfo` is NULL), the new node becomes
 * the first element of the list.
 *
 * @param fileinfo A pointer to the pointer of the first node in the file info linked list.
 *                 If the list is empty, the new node is assigned as the first element.
 * @param new A pointer to the new file info node that needs to be added to the list.
 *            This node is appended at the end of the list.
 */
void
fileinfo_add_back(t_fileinfo **fileinfo, t_fileinfo *new)
{
	if (fileinfo != NULL) {
		if (*fileinfo == NULL) {
			*fileinfo = new;
		} else {
			t_fileinfo *last = fileinfo_last(*fileinfo);
			last->next = new;
		}
	}
}

/**
 * Deletes a single fileinfo object and frees allocated memory.
 *
 * @param fileinfo Pointer to the fileinfo object to be deleted. If the pointer is not NULL,
 *                 the function frees the memory allocated for the fileinfo name and
 *                 the fileinfo itself.
 */
void
fileinfo_del_one(t_fileinfo *fileinfo)
{
	if (fileinfo != NULL)
	{
		free(fileinfo->name);
		free(fileinfo);
	}
}

/**
 * @brief Frees and clears all elements in a linked list of file information structures.
 *
 * This function traverses through a linked list of `t_fileinfo` structures,
 * deallocates memory for each node, and sets the list head pointer to NULL.
 *
 * @param fileinfo A pointer to the head of the linked list. The pointer must
 * point to a valid `t_fileinfo` linked list or NULL. If the list is not empty,
 * the function will deallocate memory for all nodes and set the pointer to NULL.
 */
void
fileinfo_clear(t_fileinfo **fileinfo)
{
	if (fileinfo != NULL) {
		while (*fileinfo)
		{
			t_fileinfo *tmp = (*fileinfo)->next;
			fileinfo_del_one(*fileinfo);
			*fileinfo = tmp;
		}
	}
}
