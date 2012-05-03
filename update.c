#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"grid.h"
#include"particles.h"
#include"update.h"

void updatefield(struct grid ***fields,
                 int size,
                 double dx, 
                 double dy, 
                 double dz,
                 double dt, 
                 int dump){

    // derivatives of E and B fields
    double ddx_Ey, ddx_Ez;
    double ddy_Ex, ddy_Ez;
    double ddz_Ex, ddz_Ey;
    double ddx_By, ddx_Bz;
    double ddy_Bx, ddy_Bz;
    double ddz_Bx, ddz_By;

    // loop variables
    int i, j, k;

    // permeability of the vacuum in SI units
    const double mu0 = 4.0*M_PI*10.0e-7;


    /* 
     * SPATIAL DERIVATIVES FOR ELECTRIC / MAGNETIC FIELD
     */
    for (i=0; i<size; i++){
        for (j=0; j<size; j++){
            for (k=0; k<size; k++){

                /* X DERIVATIVES */
                if (i==0){
                    ddx_Ey = ddx_Ez = 0.0;
                    ddx_By = ddx_Bz = 0.0;
                }
                else{
                    /* ELECTRIC FIELD*/
                    ddx_Ey = (fields[i][j][k].E[1] - fields[i-1][j][k].E[1])/dx;
                    ddx_Ez = (fields[i][j][k].E[2] - fields[i-1][j][k].E[2])/dx;

                    /* MAGNETIC FIELD*/
                    ddx_By = (fields[i][j][k].B[1] - fields[i-1][j][k].B[1])/dx;
                    ddx_Bz = (fields[i][j][k].B[2] - fields[i-1][j][k].B[2])/dx;
                }

                /* Y DERIVATIVES */
                if (j==0){
                    ddy_Ex = ddy_Ez = 0.0;
                    ddy_Bx = ddy_Bz = 0.0;
                }
                else{
                    /* ELECTRIC FIELD*/
                    ddy_Ex = (fields[i][j][k].E[0] - fields[i][j-1][k].E[0])/dy;
                    ddy_Ez = (fields[i][j][k].E[2] - fields[i][j-1][k].E[2])/dy;

                    /* MAGNETIC FIELD*/
                    ddy_Bx = (fields[i][j][k].B[0] - fields[i][j-1][k].B[0])/dy;
                    ddy_Bz = (fields[i][j][k].B[2] - fields[i][j-1][k].B[2])/dy;
                }

                /* Z DERIVATIVES */
                if (k==0){
                    ddz_Ex = ddz_Ey = 0.0;
                    ddz_Bx = ddz_By = 0.0;
                }
                else{
                    /* ELECTRIC FIELD*/ 
                    ddz_Ex = (fields[i][j][k].E[0] - fields[i][j][k-1].E[0])/dz;
                    ddz_Ey = (fields[i][j][k].E[1] - fields[i][j][k-1].E[1])/dz;

                    /* MAGNETIC FIELD*/
                    ddz_Bx = (fields[i][j][k].B[0] - fields[i][j][k-1].B[0])/dz;
                    ddz_By = (fields[i][j][k].B[1] - fields[i][j][k-1].B[1])/dz;
                }


                /* do the field updates */
                fields[i][j][k].E[0] += dt*(ddy_Bz - ddz_By - mu0*fields[i][j][k].J[0]);
                fields[i][j][k].E[1] += dt*(ddz_Bx - ddx_Bz - mu0*fields[i][j][k].J[1]);
                fields[i][j][k].E[2] += dt*(ddx_By - ddy_Bx - mu0*fields[i][j][k].J[2]);
                fields[i][j][k].B[0] += dt*(ddz_Ey - ddy_Ez);
                fields[i][j][k].B[1] += dt*(ddx_Ez - ddz_Ex);
                fields[i][j][k].B[2] += dt*(ddy_Ex - ddx_Ey);

                if (dump == 1) {
                    printf("%d %d %d %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf\n",i,j,k,fields[i][j][k].E[0],fields[i][j][k].E[1],fields[i][j][k].E[2],fields[i][j][k].B[0],fields[i][j][k].B[1],fields[i][j][k].B[2],fields[i][j][k].J[0],fields[i][j][k].J[1],fields[i][j][k].J[2]);
                } // end of if
            } //end of k loop
        } // end of j loop
    } // end of i loop
}

void updatecharges(struct particles *charges,
                   struct grid ***fields,
                   int nparticles,
                   double dt,
                   int dump){

    int i; // loop vars
    double Ex, Ey, Ez, Bx, By, Bz; // em fields
    double ax, ay, az; // accelerations
    const double q_to_m=1.75882017e11; 

    for (i=0; i<nparticles; i++){
        // Currently using (int) cast
        // probably can do something more complex here
        Ex=fields[(int)charges[i].x[0]][(int)charges[i].x[1]][(int)charges[i].x[2]].E[0];
	    Ey=fields[(int)charges[i].x[0]][(int)charges[i].x[1]][(int)charges[i].x[2]].E[1];
	    Ez=fields[(int)charges[i].x[0]][(int)charges[i].x[1]][(int)charges[i].x[2]].E[2];
	    Bx=fields[(int)charges[i].x[0]][(int)charges[i].x[1]][(int)charges[i].x[2]].B[0];
	    By=fields[(int)charges[i].x[0]][(int)charges[i].x[1]][(int)charges[i].x[2]].B[1];
	    Bz=fields[(int)charges[i].x[0]][(int)charges[i].x[1]][(int)charges[i].x[2]].B[2];

      /* Calculate accelerations */
	    ax = (q_to_m)*(Ex + charges[i].u[1]*Bz - charges[i].u[2]*By);
	    ay = (q_to_m)*(Ey + charges[i].u[2]*Bx - charges[i].u[0]*Bz);
	    az = (q_to_m)*(Ez + charges[i].u[0]*By - charges[i].u[1]*Bx);

	  /* update positions */
	    charges[i].x[0] += charges[i].u[0]*dt+(0.5)*ax*dt*dt;
	    charges[i].x[1] += charges[i].u[1]*dt+(0.5)*ay*dt*dt;
	    charges[i].x[2] += charges[i].u[2]*dt+(0.5)*az*dt*dt;

	  /* update velocities */
        charges[i].u[0] += ax*dt;
        charges[i].u[1] += ay*dt;
        charges[i].u[2] += az*dt;
	  
	    if (dump == 1){
	        printf("%lf %lf %lf\n",charges[i].x[0],charges[i].x[1],charges[i].x[2]);
	    }
    }
}