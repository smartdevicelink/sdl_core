#ifndef _EMHASHMAP_H_
#define _EMHASHMAP_H_

#include <sys/queue.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Public: An entry in the map.
 *
 * key - the entry key, currently supports only integers.
 * value - a pointer to the value. this must be allocated with malloc or
 *      be in global scope, and the map entry does not take ownership of its
 *      memory.
 */
struct MapEntry {
   char key[255];
   void* value;
   LIST_ENTRY(MapEntry) entries;
};
typedef struct MapEntry MapEntry;

LIST_HEAD(MapBucketList, MapEntry);
typedef struct MapBucketList MapBucketList;

/* Public: A struct encapsulating a map's state. All of the fields are private
 * - use the  emhashmap_ functions to interact with the map.
 *
 * bucket_count - The fixed count of buckets for elements in the map.
 * capacity - The total, fixed capacity of the map.
 * buckets - An array of MapBucketList lists, each a bucket in the map.
 * entries - An array of all entry slots, to be used in each bucket.
 * free_list - An array of all entries when not actively in a bucket.
 */
struct HashMap {
   int bucket_count;
   int capacity;
   MapBucketList* buckets;
   MapEntry* entries;
   MapBucketList free_list;
   size_t (*hash)(const char*, size_t);
};
typedef struct HashMap HashMap;

struct MapIterator {
   HashMap* map;
   int current_bucket;
   MapEntry* current_entry;
};
typedef struct MapIterator MapIterator;

/* Public: Initialize a map with the given capacity and load factor (determines
 * the number of buckets).
 *
 * This allocates memory for the buckets and entries, so make sure to call
 * emhashmap_destroy(HashMap*) after done with this map.
 *
 * map - a pointer to the map to initialize. It must already be allocated on the
 *      stack or heap.
 * capacity - the initial capacity for the map.
 * load_factor - The desired load factor when the map is at capacity.
 *
 * Returns true if the map was initialized, successfully, false if space could
 * not be allocated for the buckets or entries.
 */
bool emhashmap_initialize(HashMap* map, int capacity, float load_factor, size_t (*hash_function)(const char*, size_t));

/* Public: De-initialize a map, freeing memory for the buckets and entries.
 *
 * This will *not* free the memory associated with any values stored in the map
 * - only the buckets and map entry objects.
 *
 * map - a pointer to the map to deinitialize. It must already be allocated on
 *      the stack or heap.
 */
void emhashmap_deinitialize(HashMap* map);

/* Public: Retrive the entry for a given key from the map.
 *
 * map - the map to retrive the value.
 * key - the key for this value.
 *
 * Returns the MapEntry if found, otherwise NULL.
 */
MapEntry* emhashmap_get(HashMap* map, const char* key);

/* Public: Check if the given key is in the map.
 *
 * map - the map to query.
 * key - the key to check for membership.
 *
 * Returns true if the key is in the map.
 */
bool emhashmap_contains(HashMap* map, const char* key);

/* Public: Put the value in the map with the given key.
 *
 * If the key already exists in the map, its value will be overridden (so make
 * sure you've freed the memory associated with the existing value).
 *
 * Returns true if there was space in the map and the key-value pair was added
 * successfully. Returns false if the map is full.
 */
bool emhashmap_put(HashMap* map, const char* key, void* value);

/* Public: Remove a value with the given key from the map.
 *
 * map - the map to query.
 * key - the key to remove.
 *
 * Returns the value pointer if found in the map and removed - the map user is
 * responsible for freeing any memory associated with that pointer.
 * Returns NULL if the key was not in the map.
 */
void* emhashmap_remove(HashMap* map, const char* key);

/* Public: Get the number of keys in the map.
 *
 * map - the map to query.
 *
 * Returns the total number of keys in the map.
 */
int emhashmap_size(HashMap* map);

/* Public: Check if a map is empty.
 *
 * map - the map to query.
 *
 * Returns true if there are no entries in the map.
 */
bool emhashmap_is_empty(HashMap* map);

MapIterator emhashmap_iterator(HashMap* map);

MapEntry* emhashmap_iterator_next(MapIterator* iterator);

#ifdef __cplusplus
}
#endif

#endif // _EMHASHMAP_H_
