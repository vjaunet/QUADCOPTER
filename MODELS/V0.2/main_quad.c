#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"

#include "quad_dyn.h"
#include "euler_matrix.h"

//-------------------------------------
//--------- Main-----------------------

int main(int argc, char *argv[])
{
  printf("Hello world!\n");
  printf("QuadCopter Model v0.2\n");

  /* Variables d'etat */
  double x[DIM]={0,0,1};
  double v[DIM]={0};
  double a[DIM]={0};
  double theta[DIM]={0};       /* vecteur angle */
  double ddttheta[DIM]={0};    /* vecteur ddt angle */
  double omega[DIM]={0};       /* vecteur rotation */
  double ddtomega[DIM]={0};       /* ddt vecteur rotation */

  /*Variables Copter*/
  struct copter quad;
 const double g=9.81;

  /*Variables intermediaires*/
 int i,t;
 double dt=1.;

  /* Initialisation du calcul */
  // read parameter file
 quad.L = 0.3;
 quad.k = 5e-3;
 quad.b = quad.k/3;
 quad.mass = 0.8;
 quad.kd = 0.;
 quad.I[0] = 0.0026;
 quad.I[1] =   quad.I[0];
 quad.I[2] = 2*quad.I[0];
 for (i=0; i<NMOT; i++){
   quad.w[i] = sqrt(1./NMOT*g*quad.mass/quad.k);}

   quad.w[1] = 1;

 printf("%f \n",quad.w[0]*2*3.14*60);

 //loop
 for (t=1;t<5;t++){

   PRNTF(x[2]);
   printf("Theta :    %f %f %f \n",theta[0]*180/3.14,theta[1]*180/3.14,theta[2]*180/3.14);

   // read new inputs wi ?

   /* Computes acceleration in the main frame */
   accel(a,v,theta,quad);
   //PRNTF(a[0]);

   /* Computes Omega, the rotation vector in the quad frame */
   rotvector(theta, ddttheta, omega);

   /* Computes angular acceleration in the quad frame */
   angleaccel(omega, quad, ddtomega);

   /*Integration temporelle*/
   for (i=0; i<3; i++)
     {
       omega[i] += dt*ddtomega[i];
     }
   rotvector_inv(theta,omega,ddttheta);

   for (i=0; i<DIM; i++)
     {
       theta[i] += dt*ddttheta[i];
       v[i] += dt*a[i];
       x[i] += dt*v[i];
     }
 }
 return 0;
}

