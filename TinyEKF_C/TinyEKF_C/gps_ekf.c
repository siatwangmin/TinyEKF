/* gps_ekf: TinyEKF test case using You Chong's GPS example:
 * 
 *   http://www.mathworks.com/matlabcentral/fileexchange/31487-extended-kalman-filter-ekf--for-gps
 * 
 * Reads file gps.csv of satellite data and writes file ekf.csv of mean-subtracted estimated positions.
 *
 *
 * References:
 *
 * 1. R G Brown, P Y C Hwang, "Introduction to random signals and applied 
 * Kalman filtering : with MATLAB exercises and solutions",1996
 *
 * 2. Pratap Misra, Per Enge, "Global Positioning System Signals, 
 * Measurements, and Performance(Second Edition)",2006
 * 
 * Copyright (C) 2015 Simon D. Levy
 *
 * This code is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this code.  If not, see <http:#www.gnu.org/licenses/>.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "tinyekf_config.h"
#include "tiny_ekf.h"
#include "mat.h"



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


static void init(ekf_t * ekf, double z[][3])
{
	ekf->Q[0][0] = pow(q_x, 2.0);
	ekf->Q[1][1] = pow(q_y, 2.0);
	ekf->Q[2][2] = pow(q_theta, 2.0);

    for (int i=0; i<Nsta; ++i)
        ekf->P[i][i] = 1;

   /* for (int i=0; i<4; ++i)
        ekf->R[i][i] = R0;*/

	ekf->R[0][0] = pow(r_x, 2.0);
	ekf->R[1][1] = pow(r_y, 2.0);
	ekf->R[2][2] = pow(r_theta, 2.0);

	ekf->x[0] = z[0][0];
	ekf->x[1] = z[0][1];
	ekf->x[2] = z[0][2];


}


static void model(ekf_t * ekf, double odom)
{ 
    int j;

    ekf->fx[0] = ekf->x[0] + kx * ks * odom * cos(ekf->x[2]);
	ekf->fx[1] = ekf->x[1] + ky * ks * odom * sin(ekf->x[2]);
    ekf->fx[2] = ekf->x[2];



    for (j=0; j<3; ++j)
        ekf->F[j][j] = 1;

	ekf->F[0][2] = -kx * ks * odom * sin(ekf->x[2]);
	ekf->F[1][2] = ky * ks * odom * cos(ekf->x[2]);


    ekf->hx[0] = ekf->fx[0];
	ekf->hx[1] = ekf->fx[1];
	ekf->hx[2] = ekf->fx[2];

	for (j = 0; j < 3; ++j)
	{
		ekf->H[j][j] = 1;
	}
}


void read_data(const char *file, double z[][3], double delta_odom[])
{
	// open MAT-file
	MATFile *pmat = matOpen(file, "r");
	if (pmat == NULL) return;
	int count = 0;

	// extract the specified variable
	mxArray *arr = matGetVariable(pmat, "x");
	if (arr != NULL && mxIsDouble(arr) && !mxIsEmpty(arr)) {
		// copy data
		mwSize num = mxGetNumberOfElements(arr);
		double *pr = mxGetPr(arr);
		if (pr != NULL) {

			for (int i = 0; i < num; i++)
			{
				z[i][0] = *(pr + i);
			}
			//v.reserve(num); //is faster than resize :-)
			//v.assign(pr, pr + num);
		}
	}

	arr = matGetVariable(pmat, "y");
	if (arr != NULL && mxIsDouble(arr) && !mxIsEmpty(arr)) {
		// copy data
		mwSize num = mxGetNumberOfElements(arr);
		double *pr = mxGetPr(arr);
		if (pr != NULL) {

			for (int i = 0; i < num; i++)
			{
				z[i][1] = *(pr + i);
			}
			//v.reserve(num); //is faster than resize :-)
			//v.assign(pr, pr + num);
		}
	}

	arr = matGetVariable(pmat, "orth_theta");
	if (arr != NULL && mxIsDouble(arr) && !mxIsEmpty(arr)) {
		// copy data
		mwSize num = mxGetNumberOfElements(arr);
		double *pr = mxGetPr(arr);
		if (pr != NULL) {

			for (int i = 0; i < num; i++)
			{
				z[i][2] = *(pr + i);
			}
			//v.reserve(num); //is faster than resize :-)
			//v.assign(pr, pr + num);
		}
	}

	arr = matGetVariable(pmat, "delta_odom");
	if (arr != NULL && mxIsDouble(arr) && !mxIsEmpty(arr)) {
		// copy data
		mwSize num = mxGetNumberOfElements(arr);
		double *pr = mxGetPr(arr);
		if (pr != NULL) {

			for (int i = 0; i < num; i++)
			{
				delta_odom[i] = *(pr + i);
			}
			//v.reserve(num); //is faster than resize :-)
			//v.assign(pr, pr + num);
		}
	}
	mxDestroyArray(arr);
	matClose(pmat);
}


#define Ndata 7005

int main(int argc, char ** argv)
{    
	
	double z[Ndata][3];
	double x[Ndata][3];
	double delta_odom[Ndata];
	char* file = "middle-circle-ekf.mat";
	//char* file = "big-circle-ekf.mat";

	read_data(file, z, delta_odom);


	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < Ndata; j++)
		{
			x[j][i] = z[j][i];
		}
	}

    // Do generic EKF initialization
    ekf_t ekf;
    ekf_init(&ekf, Nsta, Mobs);

    // Do local initialization
    init(&ekf, z);

    int j, k;


    // Loop till no more data
	for (j = 1; j < Ndata; ++j) {

        //readdata(ifp, SV_Pos, SV_Rho);

        model(&ekf, delta_odom[j]);

        ekf_step(&ekf, z[j]);

        // grab positions, ignoring velocities
        for (k=0; k<3; ++k)
            x[j][k] = ekf.x[k];
    }


	FILE* fp = fopen("E:\\Document\\EyooProject\\2.Vanke\\odom\\big-circle-fusion.dat","w");

	for (int i = 0; i < Ndata; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			fprintf(fp, "%.12f ", x[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);

	//// output MAT-file
	//MATFile *pmat = matOpen("fusion_data", "w");

	//mxArray* arr = matGetVariable(pmat, "fusion");
	//if (arr != NULL && mxIsDouble(arr) && !mxIsEmpty(arr)) {
	//	// copy data
	//	mwSize num = mxGetNumberOfElements(arr);

	//	double *pr = mxGetPr(arr);
	//	if (pr != NULL) {

	//		for (int i = 0; i < num; i++)
	//		{
	//			delta_odom[i] = *(pr + i);
	//		}
	//		//v.reserve(num); //is faster than resize :-)
	//		//v.assign(pr, pr + num);
	//	}
	//}

	//matClose(pmat);

	printf("[%.12f, %.12f, %.12f]\n", x[Ndata - 1][0], x[Ndata - 1][1], x[Ndata - 1][2]);

	printf("EKF Process Ending....\n");
	getchar();
    return 0;
}
