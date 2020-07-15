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
	asm volatile("fldl %1\n" //step1
				 //"fmul %%st(0), %%st(0)\n" //step2
				 "fmull %1\n" //step2 alternate way
				 "fldpi\n" //step3 
				 "fmul %%st(1), %%st(0)" //step4
				 :"=t"(area)
				 :"m"(radius)
				 ); 
	return area;
}

