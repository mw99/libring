/**
 * Easy to use single linked list data structure. Fifo and stack usage, as well as concatenation in O(1)
 */

#ifndef _RING_H_
#define _RING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// TYPES

// contend pointer
typedef void* cp;

// Node structure (Can't be opaque because of macro based interface)
struct _Node
{
	// Node Contend 
	cp contend;
	// The next Node  
	struct _Node* next;
};

// Base structure (Can't be opaque because of macro based interface)
struct _Ring
{
	uint64_t size;
	struct _Node* first;
	struct _Node* last;
}; 

// Just 'Ring' for the main data structure
typedef struct _Ring* Ring;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// INTERFACE 

// -----------------------------------------------------------------------------
/**
 * Returns the ring size. 
 */
#define ring_size(r) (r->size)

// -----------------------------------------------------------------------------
/**
 * True if Ring is empty.
 */
#define ring_is_empty(r) (!ring_size(r))


// -----------------------------------------------------------------------------
/**
 * Creates a new Ring.
 * Complexity always O(1)
 */
Ring ring_create(void);


// -----------------------------------------------------------------------------
/**
 * Destroys a Ring. All memory is released. Implement and provide a custom free()
 * function to release the content elements.
 * Complexity always O(n)
 */
void ring_destroy(Ring r, void(*free_contend)(cp));


// -----------------------------------------------------------------------------
/**
 * Returns the first element. NULL if the ring is empty.
 * Complexity always O(1)
 */
#define ring_first(r) ( (ring_is_empty(r) ) ? NULL : r->first->contend )


// -----------------------------------------------------------------------------
/**
 * Returns the last element. NULL if the ring is empty.
 * Complexity always O(1)
 */
#define ring_last(r) ( (ring_is_empty(r) ) ? NULL : r->last->contend )


// -----------------------------------------------------------------------------
/**
 * Adds one element at the beginning of the ring.
 * Complexity always O(1)
 */
void ring_push(Ring r, cp c);


// -----------------------------------------------------------------------------
/**
 * Adds one element at the end of the ring.
 * Complexity always O(1)
 */
void ring_append(Ring r, cp c);


// -----------------------------------------------------------------------------
/**
 * Removes and returns the first element of the ring. 
 * NULL if the ring is empty.
 * Complexity always O(1)
 */
cp ring_pop(Ring r);


// -----------------------------------------------------------------------------
/**
 * Removes and returns the last element of the ring. 
 * NULL if the ring is empty.
 * WARNING!!!! Complexity always O(n). If you rely on this function you should
 * switch to another data structure.
 */
cp ring_chop(Ring r);


// -----------------------------------------------------------------------------
/**
 * Iterator over all members of the ring. Here an example how to
 * use these two macros. Never Use them in an other way.
 *
 *	Ring r = ring_create();
 *  
 *	// ... fill the ring
 *	
 *	for( ring_iterator( r ) )
 *	{
 *		int * c = ring_index;
 *		printf("%d\n", *c);
 *	}
 *
 * Use ring_iterator only in a for loop and use ring_index
 * only in the body of this for loop. It is safe to use 
 * ring_index more than one time in this body. It is also
 * possible do make an assingnment to ring_index to
 * change the content like this:
 *
 * ring_index = new_content;
 *
 * Don't perform any operation on the ring that will add or remove
 * members in this loop. 
 *
 * It is save to use 'break', 'continue' or 'goto' in this loop.
 * There macros need to be compiled with --std=gnu99 or --std=c99.
 * If this is not possible you can compile the code, but you can't 
 * use these iterator macros.
 *
 * Complexity O(n) if no break or goto is used.
 */
#define ring_iterator(r) struct _Node* _iterat_ = r->first; \
		_iterat_ != NULL; _iterat_ = _iterat_->next


#define ring_index (_iterat_->contend)


// -----------------------------------------------------------------------------
/**
 * Return the contend of a specified position. 
 * NULL if out of bounds or the contend was NULL in the first place. 
 * Complexity always O(i)
 */
cp ring_at (Ring r, uint64_t i);


// -----------------------------------------------------------------------------
/**
 * Extracts an element on a specified position.
 * NULL if out of bounds or the contend was NULL in the first place. 
 * Complexity always O(i)
 */
cp ring_extract(Ring r, uint64_t i);


// -----------------------------------------------------------------------------
/**
 * Inserts an element on a specified position.
 * Complexity always O(i)
 */
bool ring_insert_at(Ring r, cp c, uint64_t i);


// -----------------------------------------------------------------------------
/**
 * Removes a group of elements out of the ring and stores them in another
 * Ring. del_func defines which element will be removed. Own userdata can 
 * be passed to the del_func with the last parameter.
 * Complexity always O(n)
 */
Ring ring_remove_selected(Ring r, bool(*del_func)(cp c, void* ud), void* ud);


// -----------------------------------------------------------------------------
/**
 * Concatenation of two rings. Don't use r1 & r2 after the call of this function.
 * Only the returned ring is supposed to be touched again.
 * Complexity always O(1)
 */
Ring ring_concat(Ring r1, Ring r2);


// -----------------------------------------------------------------------------
/**
 * Distributes the contend of one ring over N other rings.
 * Complexity always O(N)
 * @return A ring containing N rings.
 */
Ring ring_distribute(Ring r, uint64_t n);


// -----------------------------------------------------------------------------
/**
 * Ring Invariant check.
 * Complexity always O(n)
 * @return If NULL -> Ring Ok. Else an error msg.
 */
char* ring_invariant(Ring r);


#ifdef DEBUG
#include <stdio.h>
// -----------------------------------------------------------------------------
/**
 * Print stats about the ring.
 */
void ring_print(FILE* f, Ring r, void(*pfunc)(FILE* f, cp c, uint64_t pos));
#endif



#ifdef __cplusplus
}
#endif

#endif
