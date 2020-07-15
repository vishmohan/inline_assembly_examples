#include <iostream>
#include <math.h>
using namespace std;

double calc_area_cplusplus(double radius);
double calc_area_asm(double radius);


int main(void)
{
	double radius;
	cout << "Enter the radius: " << endl;
	cin >> radius;
	cout << "Area of the circle using calc_area_cplusplus: " << endl;
	cout << calc_area_cplusplus(radius) << endl;
	cout << "Area of the circle using calc_area_asm: " << endl;
	cout << calc_area_asm(radius) << endl;
	return 0;
}


double calc_area_cplusplus(double radius)
{
	double area = 0.0;
	area = M_PI * radius * radius;
	return area;
}

double calc_area_asm(double radius)
{
	double area=0.0;
	//1. Load the radius on the fpu stack
	//2. compute the square of the radius r^2 
	//3. load the fpu stack with PI
	//4. multiply r^2 with PI to get the area
	asm volatile(
				 "fldl %1\n" //tos = st0 = r
				 "fmull %1\n"//tos = tos * r => st0 = r^2 
				 //"fmul %%st(0), %%st(0)\n"  
				 "fldpi\n"   //tos = st0 = PI, st1 = r^2
				 "fmul %%st(1), %%st(0)\n" //st0 = st0*st1 = PI*r^2
				 :"=t"(area)
				 :"m"(radius)

				); 
	return area;
}

