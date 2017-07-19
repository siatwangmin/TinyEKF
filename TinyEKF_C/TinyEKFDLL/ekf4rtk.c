#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "tinyekf_config.h"
#include "tiny_ekf.h"



const double q_x = 10.0;
const double q_y = 10.0;
const double q_theta = 10.0;


const double r_x = 10.0;
const double r_y = 10.0;
const double r_theta = 100.0;



#define M_PI 3.14159265358979323846 

double ks = 400.0 * M_PI / 300.0;
double kx = 0.00000000808798;
double ky = 0.000000008;


__declspec(dllexport) void init(ekf_t * ekf, double* z)
{
	ekf->Q[0][0] = pow(q_x, 2.0);
	ekf->Q[1][1] = pow(q_y, 2.0);
	ekf->Q[2][2] = pow(q_theta, 2.0);

    for (int i=0; i<Nsta; ++i)
        ekf->P[i][i] = 1.0;

   /* for (int i=0; i<4; ++i)
        ekf->R[i][i] = R0;*/

	ekf->R[0][0] = pow(r_x, 2.0);
	ekf->R[1][1] = pow(r_y, 2.0);
	ekf->R[2][2] = pow(r_theta, 2.0);

	ekf->x[0] = z[0];
	ekf->x[1] = z[1];
	ekf->x[2] = z[2];
}


__declspec(dllexport) void model(ekf_t * ekf, double odom)
{ 
    int j;

    ekf->fx[0] = ekf->x[0] + kx * ks * odom * cos(ekf->x[2]);
	ekf->fx[1] = ekf->x[1] + ky * ks * odom * sin(ekf->x[2]);
    ekf->fx[2] = ekf->x[2];



    for (j=0; j<3; ++j)
        ekf->F[j][j] = 1.0;

	ekf->F[0][2] = -kx * ks * odom * sin(ekf->x[2]);
	ekf->F[1][2] = ky * ks * odom * cos(ekf->x[2]);


    ekf->hx[0] = ekf->fx[0];
	ekf->hx[1] = ekf->fx[1];
	ekf->hx[2] = ekf->fx[2];

	for (j = 0; j < 3; ++j)
	{
		ekf->H[j][j] = 1.0;
	}
}

__declspec(dllexport) void print()
{
	//printf("hello wangmin\n");

}
