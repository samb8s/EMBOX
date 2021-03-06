#ifndef INITIALISE_H
#define INITIALISE_H

void initialise_distn_box(struct particles *charges,
			  int nparticles,
			  int size,
			  double dx,
			  double dy,
			  double dz,
			  FILE *charges_fp);

void initialise_distn_sphere(struct particles *charges,
			     int nparticles,
			     int size,
			     double dx,
			     FILE *charges_fp);

#endif
