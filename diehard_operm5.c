/*
 * See copyright in copyright.h and the accompanying file COPYING
 */

/*
 *========================================================================
 * This is the Diehard OPERM5 test, rewritten from the description
 * in tests.txt on George Marsaglia's diehard site.
 *
 * Rewriting means that I can standardize the interface to
 * gsl-encapsulated routines more easily.  It also makes this
 * my own code.  Finally, since the C versions Marsaglia provides
 * are the result of f2c running on Fortran sources, they are really
 * ugly code and the rewrite should be much more manageable.
 *
 * Here is the test description from diehard_tests.txt:
 *
 *          THE OVERLAPPING 5-PERMUTATION TEST                 ::
 * This is the OPERM5 test.  It looks at a sequence of one mill- ::
 * ion 32-bit random integers.  Each set of five consecutive     ::
 * integers can be in one of 120 states, for the 5! possible or- ::
 * derings of five numbers.  Thus the 5th, 6th, 7th,...numbers   ::
 * each provide a state. As many thousands of state transitions  ::
 * are observed,  cumulative counts are made of the number of    ::
 * occurences of each state.  Then the quadratic form in the     ::
 * weak inverse of the 120x120 covariance matrix yields a test   ::
 * equivalent to the likelihood ratio test that the 120 cell     ::
 * counts came from the specified (asymptotically) normal dis-   ::
 * tribution with the specified 120x120 covariance matrix (with  ::
 * rank 99).  This version uses 1,000,000 integers, twice.       ::
 *
 *========================================================================
 */


#include "dieharder.h"

double diehard_operm5()
{

 double *pvalue,pks;
 uint tempsamples;
 uint est_num_rands;

 /*
  * This is the merest shell to set any test-specific variables, call
  * the main test routine (which fills one or more slots in ks_pvalue[]
  * and increments kspi accordingly), and run a Kuiper Kolmogorov-Smirnov
  * test on the vector of pvalues produced and turn it into a single,
  * cumulative p-value (pks) for the entire test.  If the test parameters
  * are set properly, this will USUALLY yield an unambiguous signal of
  * failure.
  */

 /*
  * This test MAY require a fixed number of tsamples.  We'll see.
  */
 if(all == YES){
   tempsamples = tsamples;
   tsamples = 1000000;  /* Standard value from diehard */
 }

 if(!quiet){
   help_diehard_operm5();
   printf("# Random number generator tested: %s\n",gsl_rng_name(rng));
   printf("# Number of rands required is around 2^23 for 100 samples.\n");
 }

 kspi = 0;  /* Always zero first */
 pks = sample((void *)diehard_operm5_test);

 /*
  * Display histogram of ks p-values (optional)
  */
 if(hist_flag){
   histogram(ks_pvalue,psamples,0.0,1.0,10,"p-values");
 }
 printf("# p = %8.6f for diehard_operm5 test from Kuiper Kolmogorov-Smirnov\n",pks);
 printf("#     test on %u pvalues.\n",kspi);
 if(pks < 0.0001){
   printf("# Generator %s FAILS at 0.01%% for diehard_operm5.\n",gsl_rng_name(rng));
 }

 /*
  * Put back tsamples
  */
 tsamples = tempsamples;

 return(pks);

}

/*
 * These are the r and s matrices from diehard, all ready to go
 * with no I/O required or wierd indexing or anything.
 */
#include "diehard_operm5.h"

void diehard_operm5_test()
{

 uint i,j,k,t;
 uint count[120];
 double pvalue;

 if(verbose == D_DIEHARD_OPERM5 || verbose == D_ALL){
   printf("int r[][] = {\n");
   for(i=0;i<60;i++){
     printf("{");
     for(j=0;j<59;j++){
       printf("%10d, ",r[i][j]);
     }
     printf("%10d},\n",r[i][59]);
   }
   printf("};\n");
   printf("int s[][] = {\n");
   for(i=0;i<60;i++){
     printf("{");
     for(j=0;j<59;j++){
       printf("%10d, ",s[i][j]);
     }
     printf("%10d},\n",s[i][59]);
   }
   printf("};\n");
 }

 /*
  * Zero count vector, was t(120) in diehard.f90.
  */
 memset(count,0,120*sizeof(uint));
 for(t=0;t<tsamples;t++){
   

 }

 ks_pvalue[kspi] = pvalue;

 if(verbose == D_DIEHARD_OPERM5 || verbose == D_ALL){
   printf("# diehard_operm5(): ks_pvalue[%u] = %10.5f\n",kspi,ks_pvalue[kspi]);
 }

 kspi++;

}

void help_diehard_operm5()
{

 printf("\n\
#==================================================================\n\
#                Diehard \"OPERM5\" test.\n\
# This is the OPERM5 test.  It looks at a sequence of one mill- \n\
# ion 32-bit random integers.  Each set of five consecutive     \n\
# integers can be in one of 120 states, for the 5! possible or- \n\
# derings of five numbers.  Thus the 5th, 6th, 7th,...numbers   \n\
# each provide a state. As many thousands of state transitions  \n\
# are observed,  cumulative counts are made of the number of    \n\
# occurences of each state.  Then the quadratic form in the     \n\
# weak inverse of the 120x120 covariance matrix yields a test   \n\
# equivalent to the likelihood ratio test that the 120 cell     \n\
# counts came from the specified (asymptotically) normal dis-   \n\
# tribution with the specified 120x120 covariance matrix (with  \n\
# rank 99).  This version uses 1,000,000 integers, twice.       \n\
#==================================================================\n");

}
