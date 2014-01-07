#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/* Variables d'etat */
double x[3];
double v[3];
double a[3];
double theta[3];       /* vecteur angle */
double ddttheta[3];    /* vecteur ddt angle */
double d2dt2theta[3];  /* vecteur d2dt2 angle */
double omega[3];       /* vecteur rotation */
double ddtomega[3];       /* ddt vecteur rotation */

/* constantes */
const double g=9.81;
double C[3];            /**/

struct copter {
  double L;
  double b;
  double k;
  double mass;
  double kd;
  double I[3];           /* Intertial moments */
  double w[4];           /* vitesse des moteurs */
  };
struct copter quad;

void rotvector(double theta[3], double Vin[3], double Vout[3])
{/*Transform vector into rotation vector*/
  int cphi,ctet;
  int sphi,stet;

  cphi = cos(theta[1]);
  ctet = cos(theta[2]);
  //  cpsi = cos(theta[3]);
  sphi = sin(theta[1]);
  stet = sin(theta[2]);
  //  spsi = sin(theta[3]);

  Vout[1] = Vin[1] - stet*Vin[3];
  Vout[2] = cphi*Vin[2] + ctet*sphi*Vin[3];
  Vout[3] = -sphi*Vin[2] + ctet*cphi*Vin[3];
}

void Rmq(double theta[3], double Vin[3], double Vout[3])
{/*transform from Main frame to quad frame*/
  int cpsi,cphi,ctet;
  int spsi,sphi,stet;

  cphi = cos(theta[1]);
  ctet = cos(theta[2]);
  cpsi = cos(theta[3]);
  sphi = sin(theta[1]);
  stet = sin(theta[2]);
  spsi = sin(theta[3]);

  Vout[1] = cpsi*ctet*Vin[1] + ctet*spsi*Vin[2] -stet*Vin[3];
  Vout[2] = (cpsi*sphi*stet-cphi*spsi)*Vin[1] +
    (cphi*cpsi+sphi*spsi*stet)*Vin[2]+
    ctet*sphi*Vin[3];
  Vout[3] = (sphi*spsi+cphi*cpsi*stet)*Vin[1] +
    (cphi*spsi*stet-cpsi*stet)*Vin[2] +
    cphi*ctet*Vin[3];
}

void Rqm(double theta[3], double Vin[3], double Vout[3])
{ /*transform from quad frame to main frame*/
  int cpsi,cphi,ctet;
  int spsi,sphi,stet;

  cphi = cos(theta[1]);
  ctet = cos(theta[2]);
  cpsi = cos(theta[3]);
  sphi = sin(theta[1]);
  stet = sin(theta[2]);
  spsi = sin(theta[3]);


  Vout[1] = cpsi*ctet*Vin[1] +
    (cpsi*sphi*stet-cphi*spsi)*Vin[2] +
    (sphi*spsi+cphi*cpsi*stet)*Vin[3];
  Vout[2] = ctet*spsi*Vin[1] +
    (cphi*cpsi+sphi*spsi*stet)*Vin[2]+
    (cphi*spsi*stet-cpsi*stet)*Vin[3];
  Vout[3] = -stet*Vin[1] +
    ctet*sphi*Vin[2] +
    cphi*ctet*Vin[3];
}

void accel(double *a , double *v, double *theta, struct copter quad)
{
  double grav[3]={0,0,-g};
  double Tq[3],Tm[3];
  int i;

  /*Computes thrust in the quad Frame*/
  /*Revert to the main frame*/
  for (i=1;i<5;i++){
    Tq[3] = Tq[3] + quad.k*quad.w[i];
  }
  Rqm(theta,Tq,Tm);


  /*Suming for acceleration*/
  for (i=1;i<4;i++){
    a[i] = Tm[i] + grav[i] + quad.kd*v[i];
  }
}

void torques(double *tau, struct copter quad)
{/*Computes torques*/
  tau[1] = quad.L*quad.k*(quad.w[1]-quad.w[2]);
  tau[2] = quad.L*quad.k*(quad.w[3]-quad.w[4]);
  tau[3] = quad.b*(quad.w[1]-quad.w[2]+quad.w[3]-quad.w[4]);
}

double* crossprod(double *A,double *B)
{/*Cross product AxB*/

  C[1] = A[2]*B[3]-A[3]*B[2];
  C[2] = A[3]*B[1]-A[1]*B[3];
  C[3] = A[0]*B[2]-A[2]*B[0];

  return C;
}

double* angleaccel(double omega[3], struct copter quad)
{/*Computes angular acceleration vector of quad*/
  double tau[3];
  double Iomega[3];
  int i;

  for (i=1;i<4;i++){
    Iomega[i] = quad.I[i]*omega[i];
  }
  C = crossprod(omega,Iomega);

  /*Compute Torques*/
  torques(tau,quad);
  ddtomega[1] = 1/quad.I[1]*(tau[1]);

  return ddtomega;
}


//-------------------------------------
//--------- Main-----------------------

int main(int argc, char *argv[])
{
  printf("Hello world!\n");
  printf("QuadCopter Model!\n");

  /* Initialisation du calcul */
  // read parameter file


  //loop
  // read new inputs wi ?

  /* Computes acceleration in the main frame */
  accel(a,v,theta,quad);

  /* Computes Omega, the rotation vector in the quad frame */
  rotvector(ddttheta, theta, omega);

  /* Computes angular acceleration in the quad frame */
  ddtomega = angleaccel(omega,quad);


  return 0;
}

