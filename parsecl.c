/*
 * $Id$
 *
 * See copyright in copyright.h and the accompanying file COPYING
 *
 */

#include "dieharder.h"

void parsecl(int argc, char **argv)
{
 int imax,i,c,errflg=0;
 extern char *optarg;
 extern int optind, opterr, optopt;

 /*
  * Within reason, the following user-controllable options are referenced
  * by a flag with the same first letter.  In order:
  */
 all = NO;              /* Default is to NOT do all the tests */
 binary = NO;           /* Do output a random stream in binary (with -o) */
 bits = 128;            /* Maximum size bitstring is default */
 diehard = 0;           /* Diehard test number */
 filename[0] = (char)0; /* No input file */
 fromfile = 0;          /* Not from an input file */
 output = 0;            /* No output file */
 overlap = 0;           /* Default is NOT to use overlapping samples */
 generator = 13;        /* Default is mt19937 as a "good" generator */
 help_flag = NO;        /* No help requested */
 hist_flag = YES;       /* For the moment, histograms on */
 iterations = -1;	/* For timing loop, set iterations to be timed */
 list = NO;             /* List all generators */
 ntuple = 0;            /* n-tuple size for n-tuple tests (0 means all) */
 psamples = 100;        /* Number of test runs in final KS test */
 quiet = 0;		/* Default is to be not be quiet -- full report. */
 rgb = 0;               /* rgb test number */
 sts = 0;               /* sts test number */
 seed = 0;              /* saves the current (possibly randomly selected) seed */
 Seed = 0;              /* user selected seed.  != 0 surpresses reseeding per sample.*/
 tsamples = 10000;	/* Generally should be "a lot". */
 user = 0;              /* user defined test number */
 verbose = 0;		/* Default is not to be verbose. */
 x_user = 0.0;          /* x,y,z_user are for "arbitrary" input controls */
 y_user = 0.0;          /* and can be used by any test without having to */
 z_user = 0.0;          /* rewrite parsecl() or add global variables */

 while ((c = getopt(argc,argv,"aBb:d:f:g:H:hi:ln:Oop:qr:S:s:t:u:v:x:y:z:")) != EOF){
   switch (c){
     case 'a':
       all = YES;
       break;
     case 'B':
       binary = 1;
       break;
     case 'b':
       bits = strtol(optarg,(char **) NULL,10);
       break;
     case 'd':
       diehard = strtol(optarg,(char **) NULL,10);
       break;
     case 'f':
       strncpy(filename,optarg,128);
       fromfile = 1;
       break;
     case 'g':
       generator = strtol(optarg,(char **) NULL,10);
       break;
     case 'H':
       hist_flag = strtol(optarg,(char **) NULL,10);
       break;
     case 'h':
       help_flag = YES;
       break;
     case 'i':
       iterations = strtol(optarg,(char **) NULL,10);
       break;
     case 'l':
       list = YES;
       break;
     case 'n':
       ntuple = strtol(optarg,(char **) NULL,10);
       break;
     case 'O':
       overlap = 1;
       break;
     case 'o':
       output = 1;
       break;
     case 'p':
       psamples = strtol(optarg,(char **) NULL,10);
       break;
     case 'q':
       quiet = 1;
       break;
     case 'r':
       rgb = strtol(optarg,(char **) NULL,10);
       break;
     case 'S':
       Seed = strtol(optarg,(char **) NULL,10);
       break;
     case 's':
       sts = strtol(optarg,(char **) NULL,10);
       break;
     case 't':
       tsamples = strtol(optarg,(char **) NULL,10);
       break;
     case 'u':
       user = strtol(optarg,(char **) NULL,10);
       break;
     case 'v':
       verbose = strtol(optarg,(char **) NULL,10);
       printf("# Verbose is now %d\n",verbose);
       break;
     case 'x':
       x_user = strtod(optarg,(char **) NULL);
       break;
     case 'y':
       y_user = strtod(optarg,(char **) NULL);
       break;
     case 'z':
       z_user = strtod(optarg,(char **) NULL);
       break;
     case '?':
       errflg++;
   }
 }

 if(errflg){
   Usage();
   exit(0);
 }

 /*
  * If there are no more arguments, we are done.
  */
 if(argc-optind == 0){
   /*
    * If help was requested, call the help routine.  This routine does
    * different things depending on the other flags and variables, so
    * we defer calling it until they are all set.  help() will exit.
    */
   if(help_flag) help();
   return;
 }

 /*
  * Anything else left is an erroneous call and should cause the Usage
  * message to be printed.
  */

 Usage();

}

