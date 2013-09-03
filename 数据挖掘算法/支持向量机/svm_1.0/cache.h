#ifndef _CACHE_H_
#define _CACHE_H_

/*
 * LRU Cache
 */
class Cache {
	public:
		// l is the number of total data items, size is the cache size in byte
		Cache(int l, long int size);
		~Cache();

		// request the index column data[0, len)
		// return some position p where [p, len) need to be filled
		int get_data(const int index, float **data, int len);
		// swap the cache content of index i and j
		void swap_index(int i, int j);

	private:
		int l; // data length
		long int size; // cache size count in size of float

		// double linked circular list node
		struct head_t {
			head_t *prev, *next;
			float *data; 
			int len;//data[0, len) is cached in this entry
		};

		head_t *head; // data storage
		head_t lru_head; // circular list head
		void lru_delete(head_t *h);
		void lru_insert(head_t *h);
};

#endif
