/**
 * Easy to use single linked list data structure. Fifo and stack usage, as well as concatenation in O(1)
 */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// HEADER
#include "ring.h"

#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// DEBUGGING
#if defined(INVARIANT_CHECKS) || defined(DEBUG)
	#include <stdio.h>
	bool ring_check_invariant(Ring r);
#endif

#ifdef INVARIANT_CHECKS
	#define perr(format, ...)  fprintf(stderr, "ERROR " format "\n", ## __VA_ARGS__)
	#define EXIT_FAILURE_ASSERT 110
	#define ASSERT(x, ...) \
		if (!(x)) \
		{\
			perr("ASSERT FAILED: " #__VA_ARGS__ " (" #x ")   File: " \
			__FILE__ "   Line: %d", __LINE__); \
			exit(EXIT_FAILURE_ASSERT); \
		}
#else
	#define ASSERT(x, ...)
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS INTERN

/**
 * Safe malloc. TODO implement user defind allocation function. 
 */
static inline void* _smalloc(uint64_t s)
{
	void * res = malloc(s);
	if (!res)
		abort();
	return res;
}

// -----------------------------------------------------------------------------
/**
 * Creates a new Node. 
 */
static inline struct _Node* _ring_create_node(struct _Node* n, cp c)
{
	struct _Node* res = _smalloc(sizeof(*res));
	res->next = n;
	res->contend = c;
	return res;
}




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// EXTERN INTERFACE FUNCTIONS

// -----------------------------------------------------------------------------
/**
 * Create a new Ring.
 * Complexity always O(1)
 */
Ring ring_create(void)
{
	Ring res = _smalloc(sizeof(*res));

	res->size = 0;
	res->first = NULL;
	res->last = NULL;

	ASSERT(ring_check_invariant(res));

	return res;	
}


// -----------------------------------------------------------------------------
/**
 * Destroys a Ring. All memory is released.
 * Complexity always O(n)
 */
void ring_destroy(Ring r, void(*free_contend)(cp))
{
	ASSERT(ring_check_invariant(r));

	struct _Node* tmp; 
		
	while(r->first != NULL)
	{
		tmp = r->first;
		r->first = r->first->next;

		if(free_contend)
			free_contend(tmp->contend);

		free(tmp);
	}

	free(r);
	r = NULL;

}


// -----------------------------------------------------------------------------
/**
 * Adds one element at the beginning of the ring.
 * Complexity always O(1)
 */
void ring_push(Ring r, cp c)
{
	ASSERT(ring_check_invariant(r));

	r->first = _ring_create_node(r->first, c);

	if (ring_is_empty(r))
		r->last = r->first;

	r->size += 1;

	ASSERT(ring_check_invariant(r));
}


// -----------------------------------------------------------------------------
/**
 * Adds one element at the end of the ring.
 * Complexity always O(1)
 */
void ring_append(Ring r, cp c)
{
	ASSERT(ring_check_invariant(r));

	struct _Node* tmp = _ring_create_node(NULL, c);

	if (ring_is_empty(r))
		r->first = tmp;
	else
		r->last->next = tmp;

	r->last = tmp;

	r->size += 1;

	ASSERT(ring_check_invariant(r));
}




// -----------------------------------------------------------------------------
/**
 * Removes and returns the first element of the ring. 
 * NULL if the ring is empty.
 * Complexity always O(1)
 */
cp ring_pop(Ring r)
{
	ASSERT(ring_check_invariant(r));

	if (ring_is_empty(r))
		return NULL;
	
	cp res = r->first->contend;

	struct _Node * delme = r->first;
	
	r->first = r->first->next;
	
	free(delme);

	if(ring_size(r) == 1)
		r->last = NULL;

	r->size -= 1;

	ASSERT(ring_check_invariant(r));

	return res;
}


// -----------------------------------------------------------------------------
/**
 * Removes and returns the last element of the ring. 
 * NULL if the ring is empty.
 * CAUTION!!!! Complexity always O(n)
 */
cp ring_chop(Ring r)
{
	ASSERT(ring_check_invariant(r));

	if (ring_is_empty(r))
		return NULL;

	cp res = r->last->contend;
    
	if(ring_size(r) == 1)
	{
		free(r->last);
		r->last = NULL;
		r->first = NULL;
	}
	else
	{
		struct _Node* tmp = r->first;

		while(tmp->next->next != NULL)
		{
			tmp = tmp->next;
		}

		r->last = tmp;

		free(tmp->next);
		tmp->next = NULL;
	}

	r->size -= 1;

	ASSERT(ring_check_invariant(r));

	return res;
}


// -----------------------------------------------------------------------------
/**
 * Return the contend of a specified position. 
 * NULL if out of bounds or the contend was NULL in the first place. 
 * Complexity always O(i)
 */
cp ring_at (Ring r, uint64_t i)
{
	ASSERT(ring_check_invariant(r));

	if (i >= ring_size(r))
		return NULL;
	
	struct _Node* step = r->first;

	for(; i > 0; --i)
		step = step->next;

	return step->contend;
}


// -----------------------------------------------------------------------------
/**
 * Extracts an element on a specified position.
 * NULL if out of bounds or the contend was NULL in the first place. 
 * Complexity always O(i)
 */
cp ring_extract(Ring r, uint64_t i)
{
	ASSERT(ring_check_invariant(r));

	if (i >= ring_size(r))
		return NULL;
	
	if(i == 0)
	{
		return ring_pop(r);
	}
	else
	{
		struct _Node* step = r->first;
		struct _Node* delme; 
		cp res;

		for(; i > 1; --i)
			step = step->next;

		res = step->next->contend;
		delme = step->next;
		step->next = step->next->next;

		if(delme == r->last)
			r->last = step;

		free(delme);

		r->size -= 1;

		ASSERT(ring_check_invariant(r));

		return res; 
	}
}


// -----------------------------------------------------------------------------
/**
 * Inserts an element on a specified position.
 * Complexity always O(i)
 */
bool ring_insert_at(Ring r, cp c, uint64_t i)
{
	ASSERT(ring_check_invariant(r));

	if (i > ring_size(r))
		return false;

	if (i == 0)
	{
		ring_push(r, c);
	}
	else if (i == ring_size(r))
	{
		ring_append(r, c);
	}
	else
	{
		struct _Node * step = r->first;

		for(; i > 1; --i)
			step = step->next;

		step->next = _ring_create_node(step->next, c); 

		r->size += 1;
	}

	ASSERT(ring_check_invariant(r));

	return true;
}

// -----------------------------------------------------------------------------
/**
 * Removes a group of elements out of the ring and stores them in an other
 * Ring. del_func defines which element will be removed. Own userdata can 
 * be passed to the del_func with the last parameter.
 * Complexity always O(n)
 */
Ring ring_remove_selected(Ring r, bool(*del_func)(cp c, void* ud), void* ud)
{
	ASSERT(ring_check_invariant(r));

	Ring res = ring_create();


	while (!ring_is_empty(r))
	{ 
		if (del_func(ring_first(r), ud))
			ring_append(res, ring_pop(r));
		else
			break;
	}

	// r->first guaranteed not in res
	struct _Node* step = r->first;

	struct _Node* delme = NULL;

	while (step != NULL && step->next != NULL)
	{
		if (del_func(step->next->contend, ud))
		{
			ring_append(res, step->next->contend);

			delme = step->next;
			step->next = step->next->next;

			if(delme == r->last)
				r->last = step;

			free(delme);

			r->size -= 1;
		}
		else
		{
			step = step->next;
		}
	}

	ASSERT(ring_check_invariant(r));

	return res;
}


// -----------------------------------------------------------------------------
/**
 * Concatenation of two rings. Don't use r1 & r2 after the call of this funktion.
 * Only the returned ring is supposed to be touched again.
 * Complexity always O(1)
 */
Ring ring_concat(Ring r1, Ring r2)
{
	ASSERT(ring_check_invariant(r1));
	ASSERT(ring_check_invariant(r2));

	if(ring_is_empty(r1))
	{
		free(r1);
		return r2;
	}
	else if(ring_is_empty(r2))
	{
		free(r2);
		return r1;
	}

	r1->last->next = r2->first;
	r1->last = r2->last;
	r1->size += r2->size;

	free(r2);

	ASSERT(ring_check_invariant(r1));

	return r1;
}

// -----------------------------------------------------------------------------
/**
 * Distributes the contend of one ring over M other rings.
 * Complexity always O(n). A call with M=0 will result in M=1.
 * The input ring is still usable after this function.
 *
 * @return A ring containing M rings which comtain an nth part of the input ring.
 */
Ring ring_distribute(Ring r, uint64_t m)
{
	ASSERT(ring_check_invariant(r));
	if (!m)
		m = 1;
	
	Ring * ring_vector = _smalloc(sizeof(Ring*) * m);
	uint64_t i = 0;

	for(uint64_t i = 0; i < m; ++i)
	{
		ring_vector[i] = ring_create();
	}

	for(ring_iterator(r))
	{
		ring_append(ring_vector[ i%m ], ring_index);
		++i;
	}


	Ring res = ring_create();

	for(uint64_t i = 0; i < m; ++i)
	{
		ring_append(res, ring_vector[i]);
	}
	
	free(ring_vector);

	return res;
}


// -----------------------------------------------------------------------------
/**
 * Ring Invariant check.
 * Complexity always O(n)
 * @return If NULL -> Ring Ok. Else an error msg.
 */
char* ring_invariant(Ring r)
{
	if(!r)
		return "NULL POINTER EXCEP: Ring base struct undefinded";

	if(r->size == 0)
	{
		if(r->first || r->last)
			return "WRONG STRUCTURE: Ring size = 0, but pointer are not NULL";
	}
	else
	{
		uint64_t pos;
		struct _Node* tmp = r->first;

		if(!r->first)
			return "WRONG STRUCTURE: Ring size > 0, but first = NULL";
		if(!r->last)
			return "WRONG STRUCTURE: Ring size > 0, but last = NULL";

		if(r->last->next != NULL)
			return "WRONG STRUCTURE: Last Link->Next is not NULL";


		for(pos = 1; tmp->next != NULL; ++pos)
		{
			if(pos > r->size)
				return "WRONG STRUCTURE: No NULL pointer found before size end reached";

			tmp = tmp->next;
		}

		if(pos != r->size)
			return "WRONG STRUCTURE: Ring size != Counted Links";

		if(tmp != r->last)
			return "WRONG STRUCTURE: Last pointer != last Link";
	}

	return NULL;
}



#ifdef INVARIANT_CHECKS
// -----------------------------------------------------------------------------
/**
 * Invariant check suited for assert makros. 
 * Complexity always O(n)
 */
bool ring_check_invariant(Ring r)
{
	char* msg = ring_invariant(r);
	if(msg)
	{
		perr("%s", msg);
		return false;
	}
	
	return true;
}
#endif


#ifdef DEBUG
// -----------------------------------------------------------------------------
/**
 * Print stats about the ring.
 */
void ring_print(FILE* f, Ring r, void(*pfunc)(FILE* f, cp c, uint64_t pos))
{
	int i = 0;
	fprintf(f, "\n=========================================================\n");
	fprintf(f, " RING SIZE   : %u\n", ring_size(r));
	fprintf(f, " MEMORY USED : %u\n", ring_size(r) * (uint64_t)(sizeof(struct _Node) + sizeof(*r)));
	fprintf(f, "---------------------------------------------------------\n");

	for (ring_iterator(r))
	{
		if (!pfunc)
			fprintf(f, " Pos: %3d = %p\n", i++, ring_index);
		else
			pfunc(f, ring_index, i++);
	}
	fprintf(f, "---------------------------------------------------------\n");
}

#endif

