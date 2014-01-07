#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "euler_matrix.h"

void crossprod(double A[DIM],double B[DIM], double C[DIM])
{/*Cross product AxB*/

  C[0] = A[1]*B[2]-A[2]*B[1];
  C[1] = A[2]*B[0]-A[0]*B[2];
  C[2] = A[0]*B[1]-A[1]*B[0];

}

void accel(double a[] , double v[], double theta[], copter quad)
{
  /* constantes */
  const double g=9.81;

  double grav[DIM]={0,0,-g};
  double Tq[DIM]={0},Tm[DIM]={0};
  int i;

  /*Computes thrust in the quad Frame*/
  /*Revert to the main frame*/
  for (i=0;i<NMOT;i++){
    Tq[2] +=  quad.k*quad.w[i]*quad.w[i];
  }
  Rqm(theta,Tq,Tm);

  //PRNTF(Tm[2]);

  /*Suming for acceleration*/
  for (i=0;i<DIM;i++){
    a[i] = Tm[i]/quad.mass + grav[i] + quad.kd*v[i];
  }
  //PRNTF(a[2]);
}

void torques(double tau[], copter quad)
{/*Computes torques*/
  tau[0] = quad.L*quad.k*
    (quad.w[0]*quad.w[0]-quad.w[2]*quad.w[2]);
  tau[1] = quad.L*quad.k*
    (quad.w[1]*quad.w[1]-quad.w[3]*quad.w[3]);
  tau[2] = quad.b*
    (quad.w[0]*quad.w[0]-quad.w[1]*quad.w[1]+
     quad.w[2]*quad.w[2]-quad.w[3]*quad.w[3]);
}


void angleaccel(double omega[DIM], copter quad, double ddtomega[DIM])
{/*Computes angular acceleration vector of quad*/
  double tau[DIM];
  double Iomega[DIM];
  double C[DIM];
  int i;

  for (i=0;i<DIM;i++){
    Iomega[i] = quad.I[i]*omega[i];
  }
  crossprod(omega,Iomega,C);

  /*Compute Torques*/
  torques(tau,quad);

  /*Computes angle acceleration*/
  for (i=0;i<DIM;i++){
  ddtomega[i] = 1/quad.I[i]*(tau[i]-C[i]);
  }

  //printf("ddtOmega : %f %f %f \n",ddtomega[0],ddtomega[1],ddtomega[2]);
}
