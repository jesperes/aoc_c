// Simple hashtable implementation for plain int->int mappings. Uses a very
// simple linear probing with fixed number of buckets.
//
// Originally written to be able to solve 2021 day 13 with reasonable
// performance.
//
// Possible improvements
// * Deallocate empty buckets when needed
// * Automatically expanding buckets on-demand
// * Replace this with glibc's hsearch() family of functions?

#include "hashtable.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// https://stackoverflow.com/a/12996028/13051/
uint32_t __ht_hash(ht_key_t x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

uint32_t __ht_bucket_idx(ht_key_t key, int num_buckets) {
    return __ht_hash(key) % num_buckets;
}

void ht_init(hashtable_t *ht, int num_buckets, int default_bucket_size) {
    ht->num_buckets = num_buckets;
    ht->default_bucket_size = default_bucket_size;
    ht->total_entries = 0;
    ht->buckets = calloc(ht->num_buckets, sizeof(bucket_t));
    assert(ht->buckets != NULL);
}

int ht_get_num_entries(hashtable_t *ht) { return ht->total_entries; }

void ht_deinit(hashtable_t *ht) {
    for (int b = 0; b < ht->num_buckets; b++) {
        bucket_t *bucket = &ht->buckets[b];
        if (bucket->entries != NULL) {
            free(bucket->entries);
        }
    }
    free(ht->buckets);
}

void ht_put(hashtable_t *ht, ht_key_t key, ht_key_t value) {
    uint32_t bucket_idx = __ht_bucket_idx(key, ht->num_buckets);
    bucket_t *bucket = &ht->buckets[bucket_idx];

    // allocate entries
    if (bucket->entries == NULL) {
        bucket->num_entries = ht->default_bucket_size;
        bucket->entries = calloc(bucket->num_entries, sizeof(entry_t));
    }

    for (int entry_idx = 0; entry_idx < bucket->num_entries; entry_idx++) {
        entry_t *entry = &bucket->entries[entry_idx];
        if (!entry->present || (entry->present && entry->key == key)) {
            entry->present = true;
            entry->key = key;
            entry->value = value;
            ht->total_entries++;
            return;
        }
    }

    // bucket full
    assert("bucket full" && false);
}

bool ht_get(hashtable_t *ht, ht_key_t key, ht_value_t *value) {
    uint32_t bucket_idx = __ht_bucket_idx(key, ht->num_buckets);
    bucket_t *bucket = &ht->buckets[bucket_idx];
    if (bucket == NULL) {
        return false;
    } else {
        for (int entry_idx = 0; entry_idx < bucket->num_entries; entry_idx++) {
            if (bucket->entries == NULL) {
                // bucket is empty
                return false;
            }

            entry_t *entry = &bucket->entries[entry_idx];
            if (!entry->present) {
                continue;
            } else if (entry->key == key) {
                *value = entry->value;
                return true;
            }
        }
        return false;
    }
}

bool ht_delete(hashtable_t *ht, ht_key_t key) {
    uint32_t bucket_idx = __ht_bucket_idx(key, ht->num_buckets);
    bucket_t *bucket = &ht->buckets[bucket_idx];
    if (bucket == NULL) {
        return false;
    } else {
        for (int entry_idx = 0; entry_idx < bucket->num_entries; entry_idx++) {
            entry_t *entry = &bucket->entries[entry_idx];
            if (!entry->present) {
                continue;
            } else if (entry->key == key) {
                entry->present = false;
                ht->total_entries--;
                return true;
            }
        }
        return false;
    }
}

entry_t *ht_get_entries(hashtable_t *ht) {
    entry_t *out = calloc(ht->total_entries, sizeof(entry_t));
    int i = 0;

    for (int b = 0; b < ht->num_buckets; b++) {
        bucket_t *bucket = &ht->buckets[b];
        for (int e = 0; e < bucket->num_entries; e++) {
            entry_t *entry = &bucket->entries[e];
            if (entry == NULL) {
                continue;
            } else if (entry->present == false) {
                continue;
            } else {
                int idx = i++;
                out[idx].key = entry->key;
                out[idx].value = entry->value;
                out[idx].present = true;
            }
        }
    }

    return out;
}

void ht_traverse(hashtable_t *ht, ht_traverse_func_t func, void *data) {
    for (int b = 0; b < ht->num_buckets; b++) {
        bucket_t *bucket = &ht->buckets[b];
        for (int e = 0; e < bucket->num_entries; e++) {
            entry_t *entry = &bucket->entries[e];
            if (entry != NULL && entry->present) {
                bool should_continue = (*func)(ht, entry, data);
                if (!entry->present) {
                    // Traverse function marked entry for deletion.
                    ht->total_entries--;
                }

                if (!should_continue)
                    return;
            }
        }
    }
}

bool ht_has_key(hashtable_t *ht, ht_key_t key) {
    uint32_t bucket_idx = __ht_bucket_idx(key, ht->num_buckets);
    bucket_t *bucket = &ht->buckets[bucket_idx];
    if (bucket == NULL) {
        return false;
    } else {
        for (int entry_idx = 0; entry_idx < bucket->num_entries; entry_idx++) {
            if (bucket->entries == NULL) {
                // bucket is empty
                return false;
            }

            entry_t *entry = &bucket->entries[entry_idx];
            if (!entry->present) {
                continue;
            } else if (entry->key == key) {
                return true;
            }
        }
        return false;
    }
}

void hashtable_self_test() {
    hashtable_t ht;
    ht_init(&ht, 20, 10);
    for (int i = 42; i < 1000; i += 13) {
        ht_put(&ht, i, i * 2);
    }

    ht_put(&ht, 123, 456);
    ht_value_t value;
    assert(ht_get(&ht, 123, &value));
    assert(value == 456);

    // delete and check that the key is no longer present
    assert(ht_delete(&ht, 123));
    assert(!ht_get(&ht, 123, &value));

#if 0
    int num_entries;
    entry_t *entries = ht_get_entries(&ht, &num_entries);
    for (int i = 0; i < num_entries; i++) {
        entry_t *entry = &entries[i];
        printf("Entry[%d] = %d\n", entry->key, entry->value);
    }
#endif
    ht_deinit(&ht);
}
