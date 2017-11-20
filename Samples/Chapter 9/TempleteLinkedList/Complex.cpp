#include "Complex.h"
#include <iostream>
using namespace std;

Complex::Complex(double r, double i)
{
	real = r; 
	imag = i;
}
void Complex::Display()
{
	cout << real << "," << imag << "i" << endl;
}
Complex Complex::operator+(Complex c2)
{
	Complex c;
	c.real = real + c2.real;
	c.imag = imag + c2.imag;
	return c;
}
Complex Complex::operator-(Complex c2)
{
	Complex c;
	c.real = real - c2.real;
	c.imag = imag - c2.imag;
	return c;
}
