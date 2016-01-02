/* sim.c: Simulation of a totally asymmetric attractive */
/*        interacting particle system                   */
/*                                                      */
/*	  Microscopic model: Markov jump process on a   */
/*        1-dim discrete N-torus with rates c between   */
/*        neighbouring lattice points                   */
/*                                                      */
/*        Initial data: step-function (Riemann problem) */
/*                                                      */
/* version 0.2                                          */
/*                                                      */
/* ==================================================== */

/* N = number of lattice points on the torus                  */
/* S = {-1, 0, 1} = "number" of particles on a lattice points */

#ifndef N
#define N 5000
#endif

/* Rates: c(-1, -1) = c(-1, 0) = c(-1, 1) = c(0, 1) = c(1, 1) = 0 */
/*        c(1, -1) = 1; c(0, -1) = (1-A)/2; c(1, 0) = (1+A)/2     */
/*        c(0, 0) = B                                             */

#ifndef A
#define A 0
#endif
#ifndef B
#define B 0.005
#endif

/* DT_SHOT = time elapsed between taking shots of the system      */
/* the unit (DT_SHOT = 1) is the time unit of the Markov process  */

#ifndef DT_SHOT
#define DT_SHOT 5000
#endif

/* SHOTS = total number of shots taken (till the end of a run) */

#ifndef SHOTS
#define SHOTS 3000
#endif

/* SUM = number of independent simulations ran to calc the average */

#ifndef SUM
#define SUM 100
#endif

/* Algorithm: 1.) select neighbouring lattice points uniformly   */
/*            2.) apply the state changes according to the rates */

/* state changes:			*/
/*  (a) by (1-A)/2 rate: 0- => -0	*/
/*  (b) by (1+A)/2 rate: +0 => 0+	*/
/*  (c)       by 1 rate: +- => 00	*/
/*  (d)       by B rate: 00 => -+	*/

/* the probabilites for the state changes are: pa, pb, pc, pd	*/
/* (according to the rates)                                     */

#define MAX(a,b)	(a>b?a:b)

/* ====================================================================== */

/* the code uses the GNU Scientific Library:	*/
/* http://www.gnu.org/software/gsl/		*/

#include <stdio.h>
#include <math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_rng.h>

gsl_rng *r;	/* global random number generator */

void init_fill(gsl_vector_char *vec, double vl, double vr);
/* step initial data: density 'vl' at left, 'vr' at right */
double p(double b);			/* a p(b) function           */
double exptheta(double v, double p);	/* a theta(v) function (exp) */

/* ====================================================================== */

/* argument list for calling 'sim' binary       */
/*	argv[1]: output file name		*/
/*	argv[2]: vl = initial density on left 	*/
/*	argv[3]: vr = initial density on right	*/

int main(int argc, char *argv[]) {
  gsl_vector_char *v;	/* lattice points on the torus */
  gsl_matrix_short *m;	/* averagesd shots in time  */
  unsigned int s;	/* counter of shots */
  unsigned int dt;	/* counter of time between shosts */
  unsigned int k;	/* number of finished runs */
  FILE *outfile;	/* output file pointer */

  double vl, vr;	/* initial densities */

  int ia, ib;		/* randomly selected lattice point indexes */
  char a, b;		/* particle number on the selected lattice points */

  double pa, pb, pc, pd;	/* probabilites of state changes */
  double p0;			/* temp var for probabilites	 */

  unsigned int i;	/* loop counter on the lattice points of the torus */

  pa = 1.0 * (1-A)/2 / (2 + B);
  pb = 1.0 * (1+A)/2 / (2 + B);
  pc = 1.0 / (2 + B);
  pd = 1.0 * B / (2 + B);

  /* time scale (accelerate) */
  p0 = MAX(MAX(MAX(pa, pb), pc), pd);
  pa = pa / p0;
  pb = pb / p0;
  pc = pc / p0;
  pd = pd / p0;

  /* arg list */
  if (argc != 4) {
    fprintf(stderr, "%s [outfile] [vl] [vr]\n", argv[0]);
    exit(1);
  }

  vl = atof(argv[2]);
  vr = atof(argv[3]);

  /* open output file for writing */
  if (!(outfile = fopen(argv[1], "wb"))) {
    fprintf(stderr, "%s: i/o error\n", argv[0]);
    exit(1);
  }

  gsl_rng_env_setup();		/* prepare the random number generator */

  /* init variables */
  v = gsl_vector_char_calloc(N);
  m = gsl_matrix_short_calloc(SHOTS, N);
  r = gsl_rng_alloc(gsl_rng_default);
  k = dt = s = 0;

  printf("[+] sim.c: Running sims: ");

  while (k < SUM) {
    init_fill(v, vl, vr);
    s = 0;
    dt = DT_SHOT;	/* save the init state as well */

    while (s < SHOTS) {

      if (dt == DT_SHOT) {	/* take shot? */
	for (i = 0; i < N; ++i)
	  gsl_matrix_short_set(m, s, i, gsl_matrix_short_get(m, s, i) +
			       gsl_vector_char_get(v, i));
	dt = 0;
	++s;
      }

      /* uniformly select the neighbouring lattice points */
      ia = (int)floor(gsl_rng_uniform(r) * N);
      a = gsl_vector_char_get(v, ia);

      /* state changes */
      if (a != -1) {	/* if the 1st is not '-', then happens state change */
	++dt;
	ib = (ia + 1) % N;	
	b = gsl_vector_char_get(v, ib);

	if (a == 0) {
	  if (b == -1) { 	/* 0- => -0 */
	    p0 = gsl_rng_uniform(r);
	    if (p0 < pa) {
	      gsl_vector_char_set(v, ia, -1);
	      gsl_vector_char_set(v, ib, 0);
	    }
	  } else if (b == 0) {	/* 00 => -+ */
	    p0 = gsl_rng_uniform(r);
	    if (p0 < pd) {
	      gsl_vector_char_set(v, ia, -1);
	      gsl_vector_char_set(v, ib, 1);
	    }
	  }
	} else if (a == 1) {
	  if (b == -1) {	/* +- => 00 */
	    p0 = gsl_rng_uniform(r);
	    if (p0 < pc) {
	      gsl_vector_char_set(v, ia, 0);
	      gsl_vector_char_set(v, ib, 0);
	    }
	  } else if (b == 0) {	/* +0 => 0+ */
	    p0 = gsl_rng_uniform(r);
	    if (p0 < pb) {
	      gsl_vector_char_set(v, ia, 0);
	      gsl_vector_char_set(v, ib, 1);
	    }
	  }
	}
      }
    }

    putchar('.');
    ++k;
    if ((k % 50) == 0) {
      printf("%d", k);
    }
    fflush(stdout);
  }
  printf("done.\n");

  /* output to file (as txt) */
  printf("[+] sim.c: Saving data to '%s'...", argv[1]);

  for (k = 0; k < SHOTS; ++k) {
    for (i = 0; i < N; ++i) {
      fprintf(outfile, "%0.5f\n", 1.0 * gsl_matrix_short_get(m, k, i) / SUM);
    }
  }

  fclose(outfile);

  printf("done\n");
  exit(0);
}

/* ====================================================================== */

void init_fill(gsl_vector_char *vec, double vl, double vr) {
  double t;		/* the exp(theta) parameter */
  double p1, p2;	/* probabilities for the initial distribution */
  double z;		/* norm factor */
  unsigned int i;	/* loop counter on the lattice points of the torus */
  double p0;		/* uniformly distributed random number in (0,1) */

  t = exptheta(vl, p(B));
  z = 1 + p(B) * (exptheta(vl, p(B)) + 1 / exptheta(vl, p(B)) - 2);
  p1 = p(B) / exptheta(vl, p(B)) / z;
  p2 = p1 + (1 - 2 * p(B)) / z;

  for (i = 0; i < N/2; ++i) {
    p0 = gsl_rng_uniform(r);
    if (p0 < p1)
      gsl_vector_char_set(vec, i, -1);
    else if (p0 < p2)
      gsl_vector_char_set(vec, i, 0);
    else
      gsl_vector_char_set(vec, i, 1);
  }

  t = exptheta(vr, p(B));
  z = 1 + p(B) * (exptheta(vr, p(B)) + 1 / exptheta(vr, p(B)) - 2);
  p1 = p(B) / exptheta(vr, p(B)) / z;
  p2 = p1 + (1 - 2 * p(B)) / z;

  for (i = N/2; i < N; ++i) {
    p0 = gsl_rng_uniform(r);
    if (p0 < p1)
      gsl_vector_char_set(vec, i, -1);
    else if (p0 < p2)
      gsl_vector_char_set(vec, i, 0);
    else
      gsl_vector_char_set(vec, i, 1);
  }
}

double p(double b) {
  return (1 / (1 / sqrt(b) + 2));
}

double exptheta(double v, double p) {
  return (( v * (1 - 2 * p) + sqrt(4 * p*p + v*v - 4 * p * v*v)) /
          (2 * p * (1 - v)));
}
