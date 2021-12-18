// Hashtable implementation for integers

#include <stdbool.h>
#include <stdint.h>

typedef int key_t;
typedef int value_t;

typedef struct {
    bool present;
    key_t key;
    value_t value;
} entry_t;

typedef struct {
    int num_entries;
    entry_t *entries;
} bucket_t;

typedef struct {
    int num_buckets;
    int default_bucket_size;
    int total_entries;
    bucket_t *buckets;
} hashtable_t;

void ht_init(hashtable_t *ht, int buckets, int default_bucket_size);
void ht_deinit(hashtable_t *ht);
void ht_put(hashtable_t *ht, int key, int value);
bool ht_get(hashtable_t *ht, int key, int *value);
bool ht_delete(hashtable_t *ht, int key);
bool ht_has_key(hashtable_t *ht, int key);

// Function declaration for traversing all entries. Invoked for all entries.
// Traversal stops if function return false.
typedef bool (*ht_traverse_func_t)(hashtable_t *ht, entry_t *entry, void *data);

// Invoke func for all entries in the table. Entry values and keys can be
// modified. To delete the entry being traversed, set "entry->present" to false.
// Added keys may or may not be visible during the traversal.
void ht_traverse(hashtable_t *ht, ht_traverse_func_t func, void *data);

int ht_get_num_entries(hashtable_t *ht);
entry_t *ht_get_entries(hashtable_t *ht);

void hashtable_self_test();