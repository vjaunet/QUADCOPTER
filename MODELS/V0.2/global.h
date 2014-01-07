#define DIM 3
#define NMOT 4

#define PRNTF(X) printf("%f\n",X)

typedef struct copter {
  double L;
  double b;
  double k;
  double mass;
  double kd;
  double I[DIM];           /* Intertial moments */
  double w[NMOT];           /* vitesse des moteurs */
} copter;

