/*
 * $Id$
 *
 * See copyright in copyright.h and the accompanying file COPYING
 */

/*
 *========================================================================
 * This is the Kolmogorov-Smirnov test for uniformity on the interval
 * [0,1).  p-values from a (large) number of independent trials should
 * be uniformly distributed on the interval [0,1) if the underlying
 * result is consistent with the hypothesis that the rng in use is
 * not correlated.  Deviation from uniformity is a sign of the failure
 * of the hypothesis that the generator is "good".  Here we simply
 * input a vector of p-values and a count, and output an overall
 * p-value for the set of tests.
 *========================================================================
 */


#include "rand_rate.h"


double kstest(double *pvalue,int count)
{

 int i,j,k;
 double y,d,dmax,csqrt;
 double p,x;

 /*
  * We start by sorting the list of pvalues.
  */
 gsl_sort(pvalue,1,count);

 /*
  * Here's the test.  For each (sorted) pvalue, its index is the
  * number of values cumulated to the left of it.  d is the distance
  * between that number and the straight line representing a uniform
  * accumulation.  We save the maximum d across all cumulated samples
  * and transform it into a p-value at the end.
  */
 dmax = 0.0;
 if(verbose){
   printf("    p       y       d       dmax\n");
 }
 for(i=0;i<count;i++){
   y = (double) i/count;
   d = fabs(pvalue[i] - y);
   if(verbose){
     printf("%8.3f   %8.3f    %8.3f   %8.3f\n",pvalue[i],y,d,dmax);
   }
   if(d > dmax) dmax = d;
 }

 csqrt = sqrt(count);
 x = (csqrt + 0.12 + 0.11/csqrt)*dmax;
 if(verbose){
   printf("Kolmogorov-Smirnov D = %8.3f, evaluating q_ks(%6.2f)\n",dmax,x);
 }
 p = q_ks(x);

 return(p);

}


double q_ks(double x)
{

 int i,sign;
 double qsum;
 double kappa;

 kappa = -2.0*x*x;
 sign = -1;
 qsum = 0.0;
 for(i=1;i<100;i++){
   sign *= -1;
   qsum += (double)sign*exp(kappa*(double)i*(double)i);
   if(verbose){
     printf("Q_ks %d: %f\n",i,2.0*qsum);
   }
 }

 if(verbose){
   printf("Q_ks returning %f\n",2.0*qsum);
 }
 return(2.0*qsum);

}

/*
 * This is the Kuiper variant of KS, that is a bit more symmetric.
 */
double kstest_kuiper(double *pvalue,int count)
{

 int i,j,k;
 double y,v,vmax,vmin,csqrt;
 double p,x;

 /*
  * We start by sorting the list of pvalues.
  */
 gsl_sort(pvalue,1,count);

 /*
  * Here's the test.  For each (sorted) pvalue, its index is the
  * number of values cumulated to the left of it.  d is the distance
  * between that number and the straight line representing a uniform
  * accumulation.  We save the maximum d across all cumulated samples
  * and transform it into a p-value at the end.
  */
 if(verbose){
   printf("    obs       exp           v        vmin         vmax\n");
 }
 vmin = 0.0;
 vmax = 0.0;
 for(i=0;i<count;i++){
   y = (double) i/count;
   v = pvalue[i] - y;
   /* can only do one OR the other here, not AND the other. */
   if(v > vmax) {
     vmax = v;
   } else if(v < vmin) {
     vmin = v;
   }
   if(verbose){
     printf("%8.3f   %8.3f    %8.3f   %8.3f    %8.3f\n",pvalue[i],y,v,vmin,vmax);
   }
 }
 v = fabs(vmax) + fabs(vmin);
 csqrt = sqrt(count);
 x = (csqrt + 0.155 + 0.24/csqrt)*v;
 if(verbose){
   printf("Kuiper's V = %8.3f, evaluating q_ks_kuiper(%6.2f)\n",v,x);
 }
 p = q_ks_kuiper(x);

 return(p);

}

double q_ks_kuiper(double x)
{

 int i,sign;
 double qsum = 0;

 if(x<0.4){
   qsum = 1.0;
   if(verbose){
     printf("(cutoff): Q_ks %d: %f\n",i,qsum);
   }
 } else {
   for(i=1;i<100;i++){
     qsum += (4.0*i*i*x*x - 1.0)*exp(-2.0*i*i*x*x);
     if(verbose){
       printf("Q_ks %d: %f\n",i,2.0*qsum);
     }
   }
 }

 if(verbose){
   printf("Q_ks returning %f\n",2.0*qsum);
 }
 return(2.0*qsum);

}

