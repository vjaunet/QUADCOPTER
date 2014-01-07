#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"


void rotvector(double theta[DIM], double Vin[DIM], double Vout[DIM])
{/*Transform vector into rotation vector*/
  double cphi,ctet;
  double sphi,stet;

  cphi = cos(theta[0]);
  ctet = cos(theta[1]);
  //  cpsi = cos(theta[3]);
  sphi = sin(theta[0]);
  stet = sin(theta[1]);
  //  spsi = sin(theta[3]);

  Vout[0] = Vin[0]               -      stet*Vin[2];
  Vout[1] =          cphi*Vin[1] + ctet*sphi*Vin[2];
  Vout[2] =         -sphi*Vin[1] + ctet*cphi*Vin[2];


}


void rotvector_inv(double theta[DIM], double Vin[DIM], double Vout[DIM])
{/*Transform vector into rotation vector*/
  double cphi,sphi;
  double ttet,sectet;

  cphi = cos(theta[0]);
  ttet = tan(theta[1]);
  //  cpsi = cos(theta[3]);
  sphi = sin(theta[0]);
  sectet = 1./cos(theta[1]);
  //  spsi = sin(theta[3]);

  Vout[0] = Vin[0] + sphi*ttet*Vin[1] + cphi*ttet*Vin[2];
  Vout[1] = cphi*Vin[1] - sphi*Vin[2];
  Vout[2] = sphi*sectet*Vin[1] + sectet*cphi*Vin[2];
}

void Rmq(double theta[DIM], double Vin[DIM], double Vout[DIM])
{/*transform from Main frame to quad frame*/
  double cpsi,cphi,ctet;
  double spsi,sphi,stet;

  cphi = cos(theta[0]);
  ctet = cos(theta[1]);
  cpsi = cos(theta[2]);
  sphi = sin(theta[0]);
  stet = sin(theta[1]);
  spsi = sin(theta[2]);

  Vout[0] = cpsi*ctet*Vin[0] + ctet*spsi*Vin[1] -stet*Vin[2];
  Vout[1] = (cpsi*sphi*stet-cphi*spsi)*Vin[0] +
    (cphi*cpsi+sphi*spsi*stet)*Vin[1]+
    ctet*sphi*Vin[2];
  Vout[2] = (sphi*spsi+cphi*cpsi*stet)*Vin[0] +
    (cphi*spsi*stet-cpsi*stet)*Vin[1] +
    cphi*ctet*Vin[2];
}

void Rqm(double theta[DIM], double Vin[DIM], double Vout[DIM])
{ /*transform from quad frame to main frame*/
  double cpsi,cphi,ctet;
  double spsi,sphi,stet;

  cphi = cos(theta[0]);
  ctet = cos(theta[1]);
  cpsi = cos(theta[2]);
  sphi = sin(theta[0]);
  stet = sin(theta[1]);
  spsi = sin(theta[2]);

  Vout[0] = cpsi*ctet*Vin[0] +
    (cpsi*sphi*stet-cphi*spsi)*Vin[1] +
    (sphi*spsi+cphi*cpsi*stet)*Vin[2];
  Vout[1] = ctet*spsi*Vin[0] +
    (cphi*cpsi+sphi*spsi*stet)*Vin[1]+
    (cphi*spsi*stet-cpsi*stet)*Vin[2];
  Vout[2] = -stet*Vin[0] +
    ctet*sphi*Vin[1] +
    cphi*ctet*Vin[2];
}
