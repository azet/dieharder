/*
 *========================================================================
 * $Id$
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

/*
 *========================================================================
 * This routine does all the required initialization and startup,
 * including memory allocation and prefilling of vectors.  It is
 * COMPLETELY outside the timing loops.
 *========================================================================
 */

#include "rand_rate.h"

void rand_rate_startup()
{

 int i,imax,j,k;

 /*
  * Count and optionally list the available, built in gsl generators
  */
 types = gsl_rng_types_setup ();
 if(verbose == HELPGEN){
   printf("Listing available built-in gsl-linked generators:\n");
   printf("Test Number      Name\n");
   printf("================================\n");
 }
 i = 0;
 while(types[i] != NULL){
   if(verbose == HELPGEN){
     printf(" %d\t\t%s\n", i, types[i]->name);
   }
   i++;
 }
 num_gsl_rngs = i;

 /*
  * Now add my own types and count THEM.
  */
 add_my_types();
 if(verbose == HELPGEN){
   printf("Listing available non-gsl generators:\n");
   printf("Test Number      Name\n");
   printf("================================\n");
 }
 while(types[i] != NULL){
   if(verbose == HELPGEN){
     printf(" %d\t\t%s\n", i, types[i]->name);
   }
   i++;
 }

 num_rngs = i;
 num_my_rngs = num_rngs - num_gsl_rngs;

 if(verbose == HELPGEN) exit(0);

 /*
  * Initialize the selected gsl rng.  random_seed() seeds from
  * /dev/random.  Note that any locally defined rng's were "added"
  * to the gsl set above and can now be called with the gsl
  * wrapper!
  */
 random = gsl_rng_alloc (types[randnum]);
 random_max = gsl_rng_max(random);
 seed = random_seed();
 gsl_rng_set(random,seed);

 if(verbose == TST_RNG){
   printf("#==================================================================\n");
   printf("# generator type: %s\n", gsl_rng_name(random));
   printf("# seed value: %u, max value = %u  count = %d\n",seed, random_max,size);
   printf("# Count\t int rand\tuniform rand\n");
   printf("# ==================================================================\n");
   for(i = 1;i<=size;i++) printf("%d\t%u\t %10.8f\n",i,gsl_rng_get(random),gsl_rng_uniform(random));
   exit(0);
 }

 /*
  * We now need to allocate space for at least one vector:  one
  * for 32-bit integer deviates and/or a vector of 64-bit uniform
  * deviates.  This will need to be done from information passed on
  * the command line as to the size (number of ints, bits, doubles).
  *
  * We'll start by using "size" for this, and do just ints (into
  * the vector rand_int[]).
  */
 if(size < 1000) {
   fprintf(stderr,"Warning:  size = %d too small, using size = 1000\n",size);
   size = 1000;
 }
 rand_int = (int *) malloc((size_t) (size*sizeof(unsigned int)));

}
