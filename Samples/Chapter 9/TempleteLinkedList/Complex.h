#ifndef _COMPLEX_H_
#define _COMPLEX_H_
class Complex
{
public:
	Complex(double r=0.0, double i=0.0);
	void Display();
	Complex operator + (Complex c2);
	Complex operator - (Complex c2);
private:
	double real;
	double imag;
};

#endif