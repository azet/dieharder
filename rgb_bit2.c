/*
 * $Id$
 *
 * See copyright in copyright.h and the accompanying file COPYING
 * See also accompanying file STS.COPYING
 *
 */

/*
 *========================================================================
 * This is a test that checks to see if the rng generates bits that are
 * binomially distributed.  It checks the actual distribution of bits for
 * relatively small strings (small enough that n! doesn't overflow and
 * 2^-n doesn't underflow, so we can evaluate meaningful probabilities).
 *
 * This (and all further rgb_ tests) are "my own" in that I'm BOTH
 * making them up AND writing them as I go.  If they turn out to be
 * equivalent to STS tests I'll document this and probably convert to
 * the STS versions.  The exercise of conceiving and writing tests is
 * still invaluable.
 *========================================================================
 */

#include "rand_rate.h"

void rgb_bit2()
{

 int i,j,k,nbits,npairs,npts;
 int first_bit,current_bit,pairmask = 3,current_pair;
 Ntest *ctest;   /* For bitpair (binomial) distribution  test results */
 /* Xtest *cxtest;  /* For cumulative bitpair distribution test results */

 nbits = bits;


 /*
  * ctest will test every successive bitpair AND wrap the final bit
  * back to the first bit for the final bitpair, so there are nbits
  * bitpairs.  There are exactly four distinct binary bitpairs:
  * 00, 01, 10, 11 (the integers 0-3).  We have to do the malloc
  * here since Ntest_create won't create a vector of Ntests.
  *
  * Each pair has p=0.25, and (1-p) = 0.75.
  *
  * cxtest accumulates the occurrence totals for each pairwise
  * pattern, to permit a check against a binomial distribution of
  * all four possible patterns with p = 0.25 overall.  This is
  * equivalent to sts_runs (with cyclic wraparound).
  */
 npairs = 4;
 ctest = (Ntest *) malloc(npairs*sizeof(Ntest));
 /* cxtest = (Xtest *) malloc(npairs*sizeof(Xtest)); */
 for(i = 0;i < npairs;i++){
   Ntest_create(&ctest[i],nbits,"rgb_bitpair",gsl_rng_name(rng));
   /*
    * This is pretty standard at this point.  In fact, we could
    * probably do this with the constructor...
    */
   ctest[i].npts = samples;
   ctest[i].p = 0.25;
   for(k=0;k<nbits;k++){
     ctest[i].x[k] = 0.0;
     ctest[i].y[k] = (double) ctest[i].npts*ctest[i].p;
     ctest[i].sigma[k] = sqrt(ctest[i].y[k]*(1.0 - ctest[i].p));
   }
   /*
    * This is pretty standard at this point.  In fact, we could
    * probably do this with a constructor...
   cxtest[i].npts = nbits*samples;
   cxtest[i].p = 0.25;
   cxtest[i].x = 0.0;
   cxtest[i].y = cxtest[i].npts*cxtest[i].p;
   cxtest[i].sigma = sqrt(cxtest[i].y*(1.0 - cxtest[i].p));
   strncpy(cxtest[i].testname,"rgb_bitpair_total",128);
   strncpy(cxtest[i].rngname,gsl_rng_name(rng),128);
    */
 }

 for(i=0;i<samples;i++){
   /*
    * If this flag is set, reseed the rng on each pass through.
    * This lets us see if some problems are either ameliorated
    * or exacerbated by utilizing a space of random seeds instead
    * of just one.
    */
   if(reseed_flag){
     seed = random_seed();
     gsl_rng_set(rng,seed);
   }

   /*
    * Fill rand_int with a string of random numbers long enough to
    * provide all requested bits (which could be quite long -- we
    * have no real restrictions on number of bits in this test).
    */
   for(j=0;j<size;j++) {
     rand_int[j] = gsl_rng_get(rng);
     if(verbose){
       printf("rand_int[%d] = %u\n",j,rand_int[j]);
       dumpbits(&rand_int[j],32);
     }
   }

   /*
    * Now we are off to work.  We cycle through the bit positions.
    * at EACH bit position we tally the bit position histogram,
    * the bit total, the bitpair position histograms, and the bitpair
    * totals (in the latter cases indexed by the pair values being
    * counted).
    */
   current_pair = 0;
   for(j=0;j<nbits;j++){
     /*
      * Get the bit at this position just once, as get_bit isn't so
      * cheap as all that.
      */
     current_bit = get_bit(j);

     /*
      * Now we do bitpairs.
      */
     if(j==0) {
       /*
        * Save the first bit for periodic wraparound at the end.  Also
	* initialize current pair register with the first bit; we start
	* counting bitpairs next (j>1).
	*/
       first_bit = current_bit;
       current_pair = current_bit;
     } else {
       /*
        * For all bits AFTER the first bit, we left-shift current_pair,
        * add the current bit, & with pairmask (discarding bits that
        * have shifted out of the first two slots).
	*
	* NOTE THAT THIS
        * general methodology can be used for ANY number of bits, provided
        * that we dynamically generate the mask.  Our eventual design
        * goal here is to put all of this in a single loop over the
        * size of the bit subsequence being studied, although we'll have to
        * work a bit to manage the periodic wraparound at the end for
        * the general case and will probably need to put most of the
	* btest, bxtest, ctest, cxtest startup code above into
	* associated create() routines.
        */
       current_pair <<= 1;
       current_pair += current_bit;
       current_pair = current_pair & pairmask;
       /*
        * Note that current pair ends up being the INDEX of the ctest
	* or cxtest to be incremented (since it exists!).
        */
       ctest[current_pair].x[j-1]++;
     }
   }
   /*
    * Finally, we have to deal with the last/first bit.
    */
   current_pair <<= 1;
   current_pair += first_bit;
   current_pair = current_pair & pairmask;
   ctest[current_pair].x[j-1]++;
 }

 /*
  * Next we evaluate the pvalue for each test statistic accumulated
  * above (btest, bxtest for bits, ctest and cxtest for bitpairs).
  * Again, looks like we could probably loop this.
  */
 for(i=0;i<=3;i++){
   Ntest_eval(&ctest[i]);
 }

 if(!quiet){
   printf("#==================================================================\n");
   printf("#                        rgb_bitdist\n");
   printf("# Here we create a vector of length bits, and increment each bit\n");
   printf("# slot by the contents of the bit (0 or 1).  The statistic\n");
   printf("# in the end is an extension of rgb_persist, but sensitive to cases\n");
   printf("# where a bit changes but is biased towards either 0 or 1.  We basically\n");
   printf("# compute the expected number of bits per slot (samples/2) and the\n");
   printf("# sigma of same (sqrt(samples)/2) and form a chisq of the measured \n");
   printf("# deviations in in the vector and turn it into a p-value.\n");
   printf("#\n");
   printf("# However, this p-value is NOT the primary result of interest from\n");
   printf("# this test -- far more interesting should be the actual output\n");
   printf("# frequency histogram, which should be flat within sqrt(samples) noise.\n");
   printf("# Whereever it is NOT flat it provides us with evidence of non-randomness,\n");
   printf("# quite possibly in ways that on average satisfy the expectations for\n");
   printf("# the total number of bits (one can systematically have balanced\n");
   printf("# surfeits and deficits of ones in certain bitslots).\n");
   printf("#\n");
   printf("#==================================================================\n");
 }

 /*
  * This prints out "conclusions" (e.g. pass test, fail test, maybe
  * pass, maybe fail, etc. for each test.
  */
 for(i=0;i<=3;i++){
   Ntest_conclusion(&ctest[i]);
 }

 /*
  * Finally, we do a full summary of the test results.  The DETAIL
  * printed above -- this just indicates the p-value, basically
  * for each test pattern.
  */
 printf("# Full rgb_bitdist test summary:\n");

 printf("#==================================================================\n");
 printf("# %s test of %s summary:\n",ctest[0].testname,ctest[0].rngname);
 printf("# %8s  %8s  %8s  %8s  %8s  %8s\n","samples","bits",
    "p(00)","p(01)","p(10)","p(11)");
 printf(" %10d  %8d  %8.4f  %8.4f  %8.4f  %8.4f\n",samples,nbits,
    ctest[0].pvalue,ctest[1].pvalue,ctest[2].pvalue,ctest[3].pvalue);


 /*
  * Free everything...
  */
 for(i=0;i<=3;i++){
   Ntest_destroy(&ctest[i]);
 }
 free(ctest);

}

