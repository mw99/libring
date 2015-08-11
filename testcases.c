/**
 * @file Ring data structure unit tests
 * @author Markus Wanke 
 */

/* ---- System Header -------------------------------------------------------------- */
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* ---- Own Header ----------------------------------------------------------------- */
#include "ring.h"

/* ---- Helper Functions ----------------------------------------------------------- */
	
#define ES_none   "\033[0m"
#define ES_bold   "\033[1m"
#define ES_red    "\033[31m"
#define ES_yellow "\033[33m"
#define ES_blue   "\033[34m"
#define ES_white  "\033[37m"
#define pinfo(format, ...) fprintf(stderr, ES_bold ES_blue "INFO " ES_none ES_white format ES_none "\n", ## __VA_ARGS__)
#define pwarn(format, ...) fprintf(stderr, ES_bold ES_yellow "WARN " ES_none ES_yellow format ES_none "\n", ## __VA_ARGS__)
#define perr(format, ...)  fprintf(stderr, ES_bold ES_red "ERROR " ES_none ES_red format ES_none "\n", ## __VA_ARGS__)


void pfunc(FILE * f, cp c, uint64_t pos)
{
	fprintf(f, " Pos: %3ld = %6d\n", pos, *(int*)c );
}

/* ---- Test Functions ----------------------------------------------------------- */

#define TEST_FUNC_ARRAY_SIZE 20
#define TEST_ARRAY_SIZE 2000

void( *tests[TEST_FUNC_ARRAY_SIZE] )( void );

int32_t a[TEST_ARRAY_SIZE];

void t_01(void)
{
	int i = 0 ;
	int* p = NULL ;

	Ring r = ring_create();

	for(i = 0; i < 10; ++i)
		ring_push( r, a + i );
	
	for(i = 9; i >= 0; --i)
	{
		p = ring_pop(r);
		if( p != a + i  && *p != i )
		{
			perr("T1: Push & Pop at i = %d", i);
			return;
		}
	}

	if( !ring_is_empty( r ) )
	{
		perr("T1: Push & Pop: RING not empty");
		return;
	}

	ring_destroy(r, NULL);

	pinfo("T1: ring_push & ring_pop success");
}

void t_02(void)
{
	int i = 0;
	int* p = NULL ;
	Ring r = ring_create();

	for(i = 0; i < 20; ++i)
		ring_push(r, a+i);
	
	for(i = 0; i < 20; ++i)
	{
		p = ring_chop(r);

		if( p != a+i ){
			perr( "T2: push & chop address fail at i = %d", i); return;}

		if( *p != i )
			perr( "T2: push & chop contend fail at i = %d != %d", i, *p);
	}

	if(!ring_is_empty(r))
	{
		
		perr( "T2: push & chop: RING not empty");
		return;
	}

	ring_destroy(r, NULL);

	pinfo("T2: ring_push & ring_chop success");
}

void t_03(void)
{
	int i = 0;

	Ring r = ring_create();

	for(i = 0; i < 5; ++i)
	{
		ring_append(r, i+a);
	}

	i = 0;
	for( ring_iterator( r ) )
	{
		int * c = ring_index;

		if ( *c != i++ )
		{
			perr( "T3: iterator fail ");
			return;
		}
	}
	
	ring_destroy(r, NULL);

	pinfo( "T3: ring_iterator success");

}

void t_04(void)
{
	int i = 0;
	int * p = NULL;
	Ring r = ring_create();

	for(i = 0; i < 10; ++i)
		ring_append(r, a+i);
	
	for(i = 0; i < 10; ++i)
	{
		p = ring_at(r, i);

		if( p != a+i ) {
			perr( "T4: append & At i addresse fail = %d", i); return;}

		if( *p != i ){ 
			perr( "T4: append & At i contend fail = %d", i); return;}
	}

	int x = 77;

	ring_push(r, &x);
	if(ring_at(r, 0) != &x)
		perr( "T4: push & At addresse 77 fail");
		
	ring_append(r, &x);
	if(ring_at(r, ring_size(r)-1 ) != &x)
		perr( "T4: append & At addresse 77 fail");

	ring_destroy(r, NULL);
		
	pinfo( "T4: ring_mixed stuff success ");
}

bool ret_true(cp c, cp ud) {return true;}
bool ret_false(cp c, cp ud) {return false;}

bool c_eq_ud(cp c, cp ud) 
{
	return (*(int*)c == *(int*)ud);
}

bool even(cp c, cp ud) 
{
	return ((*(int*)c) % 2 == 0);
}

bool odd(cp c, cp ud) 
{
	return ((*(int*)c) % 2 != 0);
}

bool dunknow(cp c, cp ud) 
{
	return (rand() % 2 == 0);
}

void t_05(void)
{
	int i = 0;
	int * p = NULL;
	Ring r = ring_create();
	Ring res = NULL;

// FOR ERROR PRINT USE	
// ring_print(stderr, r, pfunc);
	int x = 11;
	int y = 44;
	int z = 66;

	ring_append(r, &z);

	res = ring_remove_selected(r, ret_false, NULL);
	if( ring_size(r) != 1 || ring_size(res) != 0 || ring_first(r) != &z )
	{
		perr( "T5: Case 01 failed" ); return;
	}
	ring_destroy( res, NULL);

// ------
	res = ring_remove_selected(r, ret_true, NULL);
	if( ring_size(r) != 0 || ring_size(res) != 1 || ring_first(res) != &z )
	{
		perr( "T5: Case 02 failed" ); return;
	}
	// ring_print(stderr, res, pfunc);
	ring_destroy( res, NULL);

// ------
	ring_append(r, &x);
	ring_append(r, &y);

	res = ring_remove_selected(r, c_eq_ud, &x);
	if( ring_size(r) != 1 || ring_size(res) != 1 
	 || ring_first(res) != &x || ring_first(r) != &y )
	{
		perr( "T5: Case 03 failed" ); return;
	}
	ring_destroy( res, NULL);
	ring_pop(r);


// ------
	ring_append(r, &x);
	ring_append(r, &y);

	res = ring_remove_selected(r, c_eq_ud, &y);
	if( ring_size(r) != 1 || ring_size(res) != 1 
	 || ring_first(r) != &x || ring_first(res) != &y )
	{
		perr( "T5: Case 04 failed" ); return;
	}
	ring_destroy( res, NULL);
	ring_pop(r);

// ------
	ring_append(r, &x);
	ring_append(r, &y);
	ring_append(r, &z);

	res = ring_remove_selected(r, c_eq_ud, &z);
	if( ring_size(r) != 2 || ring_size(res) != 1 
	 || ring_first(r) != &x || ring_first(res) != &z
	 || ring_last(r) != &y) 
	{
		perr( "T5: Case 05 failed" ); return;
	}
	ring_destroy( res, NULL);
	ring_destroy( r, NULL);
// ------
// ------
	
	r = ring_create();

	for(i = 0; i < 6; ++i)
		ring_append(r, a + i);

	
	// ring_print(stderr, r, pfunc);
	res = ring_remove_selected(r, even, NULL);

	if( ring_size( r ) != 3 || ring_size( res ) != 3 ) 
	{
		perr( "T5: Case 06 failed" ); return;
	}

	// ring_print(stderr, r, pfunc);
	// ring_print(stderr, res, pfunc);
	for(i = 0; i < 6; ++i)
	{
		if( i % 2 == 0 )
		{
			int * p = ring_pop(res);
			if ( i != *p )
				perr( "T5: Case 07 failed" );
				
		}
		else 
		{
			int * p = ring_pop(r);
			if ( i != *p )
				perr( "T5: Case 07 failed" );
		}
	}
	ring_destroy( res, NULL);
	ring_destroy( r, NULL);
			

// ------
	r = ring_create();

	for(i = 0; i < 6; ++i)
		ring_append(r, a + i);

	
	// ring_print(stderr, r, pfunc);
	res = ring_remove_selected(r, odd, NULL);

	if( ring_size( r ) != 3 || ring_size( res ) != 3 ) 
	{
		perr( "T5: Case 08 failed" ); return;
	}

	// ring_print(stderr, r, pfunc);
	// ring_print(stderr, res, pfunc);
	for(i = 0; i < 6; ++i)
	{
		if( i % 2 == 0 )
		{
			int * p = ring_pop(r);
			if ( i != *p )
				perr( "T5: Case 9 failed" );
				
		}
		else 
		{
			int * p = ring_pop(res);
			if ( i != *p )
				perr( "T5: Case 9 failed" );
		}
	}

	ring_destroy( res, NULL);
	ring_destroy( r, NULL);

// ------

	for(int times = 0; times < 100; ++times)
	{
		int32_t erg = rand() % 401;

		r = ring_create();


		for(i = 0; i < erg; ++i)
			ring_append(r, a + i);
		
	
		res = ring_remove_selected(r, dunknow, NULL);

		if( ring_size( r ) + ring_size( res ) != (uint32_t)erg ) 
		{
			perr( "T5: Case 10 failed" ); return;
		}

		char * msg1 = ring_invariant( r );
		char * msg2 = ring_invariant( res );
		if( msg1 )
			perr( "%s", msg1 );
		if( msg2 )
			perr( "%s", msg2 );

		ring_destroy( res, NULL);
		ring_destroy( r, NULL);
	}
	

	pinfo( "T5: ring_remove_selected success");
}

void t_06( void )
{
	int i = 0;
	int * p = NULL;
	Ring res = NULL;
	Ring r1 = NULL;
	Ring r2 = NULL;

// FOR ERROR PRINT USE	
// ring_print(stderr, r, pfunc);
	int x = 11;
	int y = 44;
	int z = 66;

	r1 = ring_create( );
	r2 = ring_create( );

	res = ring_concat( r1, r2 );

	if( ring_size(res) != 0 )
	{
		perr( "T6: Case 01 failed" ); return;
	}
	ring_destroy( res, NULL);

// ---------

	r1 = ring_create( );
	r2 = ring_create( );

	ring_append(r1, &x);
	ring_append(r2, &y);

	res = ring_concat( r1, r2 );

	if( ring_size(res) != 2 || ring_first(res) != &x 
    ||  ring_last(res) != &y)
	{
		perr( "T6: Case 02 failed" ); return;
	}
	ring_destroy( res, NULL);

// ---------

	r1 = ring_create( );
	r2 = ring_create( );

	for(i = 1; i < 5; ++i)
		ring_append(r1, a+i);

	for(i = 5; i < 9; ++i)
		ring_append(r2, a+i);

	res = ring_concat(r1, r2);


	if(ring_size(res) != 8)
	{
		perr( "T6: Case 03 failed" ); return;
	}


	for(i = 1; i < 9; ++i)
	{
		if(*((int*)ring_pop(res)) != i)
		{
			perr( "T6: Case 04 failed" ); return;
		}
	}

	ring_destroy( res, NULL);

	pinfo( "T6: ring_concat success");
}

void t_07(void)
{
	int i = 0;
	Ring r1 = ring_create();

	ring_append(r1, a+10);
	ring_append(r1, a+11);
	ring_append(r1, a+12);

	ring_extract(r1, 1);


	if(ring_size(r1) != 2)
	{
		 perr( "T7: Extract Size fail"); return;
	}

	ring_extract(r1, 1);

	if(ring_size(r1) != 1)
	{
		 perr( "T7: Extract Size fail"); return;
	}

	ring_append(r1, a+11);
	ring_append(r1, a+12);
	ring_append(r1, a+13);
	ring_append(r1, a+14);

	if(*(int*)ring_extract(r1, 2) != 12)
	{
		 perr( "T7: extract (1) Contend fail"); return;
	}

	if(*(int*)ring_extract(r1, 3) != 14)
	{
		 perr( "T7: extract (2) Contend fail"); return;
	}

	if(*(int*)ring_extract(r1, 1) != 11)
	{
		 perr( "T7: extract (3) Contend fail"); return;
	}

	if(*(int*)ring_extract(r1, 0) != 10)
	{
		 perr( "T7: extract (4) Contend fail"); return;
	}

	if(ring_size(r1) != 1)
	{
		 perr( "T7: extract (5) Size fail"); return;
	}

	ring_destroy(r1, NULL);

	pinfo( "T7: ring_extract success");
}


void t_08(void)
{
	int i = 0;
	Ring r1 = ring_create();

	ring_insert_at(r1, a+10, 0);
	ring_insert_at(r1, a+12, 1);
	ring_insert_at(r1, a+11, 1);

	if(ring_size(r1) != 3)
	{
		 perr( "T08: insert_at Size fail"); return;
	}


	for(i = 10; i < 13; ++i)
	{
		if(*(int*)ring_pop(r1) != i)
		{
			 perr( "T08: insert_at contend fail"); return;
		}
	}

	if(!ring_is_empty(r1))
	{
		 perr( "T08: insert_at (pop) is_empty fail"); return;
	}

	ring_insert_at(r1, a+10, 0);
	ring_insert_at(r1, a+11, 1);
	ring_insert_at(r1, a+14, 2);

	ring_insert_at(r1, a+12, 2);
	ring_insert_at(r1, a+13, 3);

	for(i = 10; i < 15; ++i)
	{
		if(*(int*)ring_pop(r1) != i)
		{
			 perr( "T08: insert_at contend fail"); return;
		}
	}

	ring_destroy(r1, NULL);
		
	pinfo( "T8: ring_insert_at success");
}

void t_09(void)
{
	Ring r1 = ring_create();

	if ( ring_at(r1, 0) != NULL
	||   ring_at(r1, 1) != NULL 
	||   ring_at(r1, 2) != NULL)
	{
		 perr( "T09: at empty fail"); return;
	}

	ring_append(r1, a+1);
	ring_append(r1, a+2);
	ring_append(r1, a+3);

	if ( ring_at(r1, 0) != a+1
	||   ring_at(r1, 1) != a+2 
	||   ring_at(r1, 2) != a+3)
	{
		 perr( "T09: at fail"); return;
	}

	ring_destroy(r1, NULL);
	pinfo( "T9: ring_at success");
}

void t_0A( void )
{
	Ring r = ring_create();
	for( uint32_t i = 0; i < 64; ++i )
	{
		ring_append( r, a+i );
	}



	Ring multi = ring_distribute(r, 4);

	if ( ring_size(multi) != 4 )
	{
		 perr( "T0A: multi size fail"); return;
	}

	Ring m1 = ring_pop(multi);
	Ring m2 = ring_pop(multi);
	Ring m3 = ring_pop(multi);
	Ring m4 = ring_pop(multi);

	ring_destroy(multi, NULL);

	if ( ring_size(m1) + ring_size(m2) + ring_size(m3) + ring_size(m4) != 64 )
	{
		 perr( "T0A: distribute fail"); return;
	}

	for( uint32_t i = 0; i < 64; ++i )
	{
		int * tmp = NULL;
		int c = i % 4;

		switch( c )
		{
			break; case 0:
				tmp = ring_pop( m1 );
			break; case 1:
				tmp = ring_pop( m2 );
			break; case 2:
				tmp = ring_pop( m3 );
			break; case 3:
				tmp = ring_pop( m4 );
		}
		
		if( *tmp != (int32_t)i )
		{
			perr( "T0A: distribute contend fail, case: %d", i); return;
		}

	}
	// ring_print(stderr, m1, pfunc);
	// ring_print(stderr, m2, pfunc);
	// ring_print(stderr, m3, pfunc);
	// ring_print(stderr, m4, pfunc);

	ring_destroy(r, NULL);
	ring_destroy(m1, NULL);
	ring_destroy(m2, NULL);
	ring_destroy(m3, NULL);
	ring_destroy(m4, NULL);

	pinfo( "TA: ring_distribute success");
}





int main( void )
{
	srand( time( NULL ) );

	// just in case
	for ( int i = 0; i < TEST_FUNC_ARRAY_SIZE ; ++i )
		tests[i] = NULL;

	for ( int i = 0; i < TEST_ARRAY_SIZE ; ++i )
		a[i] = i;

	// 0 reserved
	tests[1] = t_01;
	tests[2] = t_02;
	tests[3] = t_03;
	tests[4] = t_04;
	tests[5] = t_05;
	tests[6] = t_06;
	tests[7] = t_07;
	tests[8] = t_08;
	tests[9] = t_09;
	tests[10] = t_0A;

	for ( int i = 0; i < TEST_FUNC_ARRAY_SIZE ; ++i )
		if( tests[i] )
			( *tests[i] )( );
	
	return 0;
}
