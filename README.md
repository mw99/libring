# libring
Easy to use single linked list data structure. Fifo and stack usage, as well as concatenation in O(1)


Usage example:
```
// allocates an integer
int* alloc_int(int i)
{
	int* tmp = malloc(sizeof(int));
	if(!tmp)
		abort();
	*tmp = i;
	return tmp;
}

// selector function for picking ints larger than 100 
bool gt100(cp item, void* _moot)
{
	return *(int*)item > 100;
}

void usage_example(void)
{
	// create a ring
	Ring r = ring_create();

	// append 4 items
	for(int i = 0; i < 4; ++i)
		ring_append(r, alloc_int(42 * i));

	// push 4 items
	for(int i = 0; i < 4; ++i)
		ring_push(r, alloc_int(67 * i));

	// insert at specific position
	ring_insert_at(r, alloc_int(300), 4);
	// remove at specific position
	int* a = ring_extract(r, 4);
	printf("At pos 4: %d\n", *a);
	free(a);

	// ring iterator usage
	for(ring_iterator(r))
	{
		int* c = ring_index;
		printf("Contend: %d\n", *c);
	}

	// remove a subgroup of elements. here ints larger then 100
	Ring b = ring_remove_selected(r, gt100, NULL);

	// concat them back together
	r = ring_concat(r, b);
	
	// destroy the ring, releasing memory simple with stdlib free()
	ring_destroy(r, free);
}
```
