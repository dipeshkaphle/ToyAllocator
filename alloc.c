#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct free_list {
  size_t size;
  char used;
  struct free_list *next;
  struct free_list *prev;
  void *data;
};

void *my_malloc(size_t block_size) {
  static struct free_list root = {
      .size = 0, .used = 1, .next = NULL, .prev = NULL, .data = NULL};
  struct free_list *tmp = (&root)->next;
  struct free_list metadata = {
      .size = block_size, .used = 1, .next = NULL, .prev = NULL, .data = NULL};
  char *data;
  if (tmp == NULL) {
    data = (char *)sbrk(block_size + sizeof(struct free_list));
    if (data == (void *)-1) {
      return NULL;
    }
    metadata.prev = &root;
    metadata.data = data + sizeof(struct free_list);
    memcpy(data, &metadata, sizeof(struct free_list));
    memset(metadata.data, 0, block_size);
    root.next = (struct free_list *)data;
    return metadata.data;
  } else {
    while (tmp->next != NULL) {
      if (tmp->used == 0) {
        if (tmp->size >= block_size) {
          memset(tmp->data, 0, block_size);
          tmp->used = 1;
          return tmp->data;
        }
      }
      tmp = tmp->next;
    }
    data = (char *)sbrk(block_size + sizeof(struct free_list));
    if (data == (void *)-1) {
      return NULL;
    }
    metadata.data = data + sizeof(struct free_list);
    memset(metadata.data, 0, block_size);
    metadata.prev = tmp;
    memcpy(data, &metadata, sizeof(struct free_list));
    tmp->next = (struct free_list *)data;
    return metadata.data;
  }
  return NULL;
}

void my_free(void *ptr) {
  if (ptr == NULL) {
    exit(255);
  }
  char *data = (char *)ptr;
  struct free_list *metadata =
      (struct free_list *)(data - sizeof(struct free_list));
  if (metadata->prev->used == 0) {
    struct free_list *metadata_prev = metadata->prev;
    metadata_prev->size += (metadata->size + sizeof(struct free_list));
    metadata_prev->next = metadata->next;
    metadata = metadata_prev;
  }
  metadata->used = 0;
  if (metadata->next == NULL) {
    int size_to_decrease = metadata->size;
    size_to_decrease += (sizeof(struct free_list));
    metadata->prev->next = NULL;
    sbrk(-size_to_decrease);
  }
  return;
}
