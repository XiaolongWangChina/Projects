#include "util.h"
#include "cache.h"

Cache::Cache(int l_, long int size_):l(l_),size(size_) {
	head = (head_t *)calloc(l, sizeof(head_t));

	size /= sizeof(float);
	size -= l * sizeof(head_t) / sizeof(float);
	size = max(size, 2 * (long int)l); // cache must be large enough for two columns, it is just a lower limit

	lru_head.next = lru_head.prev = &lru_head; // double linked circular list
}

Cache::~Cache() {
	for (head_t *h = lru_head.next; h != &lru_head; h = h->next) {
		free(h->data);
	}
	free(head);
}

// delete h from circular list
void Cache::lru_delete(head_t *h) {
	h->prev->next = h->next;
	h->next->prev = h->prev;
}

// insert h into circular list just after lru_head
void Cache::lru_insert(head_t *h) {
	h->next = &lru_head;
	h->prev = lru_head.prev;
	h->prev->next = h;
	h->next->prev = h;
}

// request the index column data[0, len)
// return some position p where [p, len) need to be filled
int Cache::get_data(const int index, float **data, int len) {
	head_t *h = &head[index];
	//delete from current position
	if (h->len) {
		lru_delete(h);
	}

	int more = len - h->len;
	if (more > 0) {
		// if there has not enough space, just free some
		// old space use the lru algorithm
		while (size < more) {
			head_t *old = lru_head.next;
			lru_delete(old);
			free(old->data);
			size += old->len;
			old->data = 0;
			old->len = 0;
		}

		//allocate new space
		h->data = (float *)realloc(h->data, sizeof(float) * len);
		size -= more;
		swap(h->len, len);
	}

	// insert just after the lru_head to realize the LRU algorithm
	lru_insert(h);
	*data = h->data;

	return len;
}

// swap the i and j index of the cached data
void Cache::swap_index(int i, int j) {
	//no
	if (i == j) {
		return;
	}

	// swap the i and j column
	if (head[i].len) { lru_delete(&head[i]); }
	if (head[j].len) { lru_delete(&head[j]); }
	swap(head[i].data, head[j].data);
	swap(head[i].len, head[j].len);
	if (head[i].len) { lru_insert(&head[i]); }
	if (head[j].len) { lru_insert(&head[j]); }

	// swap the i and j row	
	if (i > j) {
		swap(i, j);
	}
	for (head_t *h = lru_head.next; h != &lru_head; h = h->next) {
		if (h->len > i) {
			if (h->len > j) {
				swap(h->data[i], h->data[j]);
			} else {
				// i < h->len < j, then the i and j row
				// can not be swaped, so delete it!
				lru_delete(h);
				free(h->data);
				size += h->len;
				h->data = 0; 
				h->len = 0;
			}
		}
	}
}

