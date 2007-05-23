/*
 * $Id: diehard_rank_6x8.c 230 2006-08-22 05:31:54Z rgb $
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *
 */

/*
 *========================================================================
 * This is the Diehard BINARY RANK 6x8 test, rewritten from the 
 * description in tests.txt on George Marsaglia's diehard site.
 *
 * This is the BINARY RANK TEST for 6x8 matrices.  From each of  ::
 * six random 32-bit integers from the generator under test, a   ::
 * specified byte is chosen, and the resulting six bytes form a  ::
 * 6x8 binary matrix whose rank is determined.  That rank can be ::
 * from 0 to 6, but ranks 0,1,2,3 are rare; their counts are     ::
 * pooled with those for rank 4. Ranks are found for 100,000     ::
 * random matrices, and a chi-square test is performed on        ::
 * counts for ranks 6,5 and <=4.                                 ::
 *
 *                          Comments
 *===================================================================
 */

#include <dieharder/libdieharder.h>

void diehard_rank_6x8(Test **test, int irun)
{


 int i,j,k,t,rank,offset;
 double r,smax,s;
 
 uint bitstring,rmask,mask;
 Vtest vtest;

 Vtest_create(&vtest,7,"diehard_rank_6x8",gsl_rng_name(rng));
 for(i=0;i<2;i++){
   vtest.x[0] = 0.0;
   vtest.y[0] = 0.0;
 }
 vtest.x[2] = 0.0;
 vtest.y[2] = test[0]->tsamples*0.149858E-06;
 vtest.x[3] = 0.0;
 vtest.y[3] = test[0]->tsamples*0.808926E-04;
 vtest.x[4] = 0.0;
 vtest.y[4] = test[0]->tsamples*0.936197E-02;
 vtest.x[5] = 0.0;
 vtest.y[5] = test[0]->tsamples*0.217439E+00;
 vtest.x[6] = 0.0;
 vtest.y[6] = test[0]->tsamples*0.773118E+00;

 for(t=0;t<test[0]->tsamples;t++){

   /*
    * We generate 6 random rmax_bits-bit integers and put a
    * randomly chosen byte into the LEFTMOST byte position
    * of the row/slot of diehard_rank_6x8_mtx.
    */
   if(verbose == D_DIEHARD_RANK_6x8 || verbose == D_ALL){
     printf("# diehard_rank_6x8(): Input random matrix = \n");
   }
   for(i=0;i<6;i++){
     if(verbose == D_DIEHARD_RANK_6x8 || verbose == D_ALL){
       printf("# ");
     }

     get_rand_bits(&bitstring,sizeof(uint),8,rng);
     diehard_rank_6x8_mtx[i][0] = bitstring;

     if(verbose == D_DIEHARD_RANK_6x8 || verbose == D_ALL){
       dumpbits(diehard_rank_6x8_mtx[i],32);
     }
   }

   rank = binary_rank(diehard_rank_6x8_mtx,6,8);
   if(verbose == D_DIEHARD_RANK_6x8 || verbose == D_ALL){
     printf("binary rank = %d\n",rank);
   }

   if(rank <= 2){
     vtest.x[2]++;
   } else {
     vtest.x[rank]++;
   }
 }

 /* for(i=0;i<33;i++) printf("vtest.x[%d] =  %f\n",i,vtest.x[i]); */

 Vtest_eval(&vtest);
 test[0]->pvalues[irun] = vtest.pvalue;
 MYDEBUG(D_DIEHARD_RANK_6x8) {
   printf("# diehard_rank_6x8(): test[0]->pvalues[%u] = %10.5f\n",irun,test[0]->pvalues[irun]);
 }

 Vtest_destroy(&vtest);

}

