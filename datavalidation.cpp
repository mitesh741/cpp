#include<iostream>
using namespace std;
void initMenu();
void menuDecision(int);
double areaCircle(double);
double areaSquare(double);
double areaRectangle(double,double);
double areaTriangle(double,double);
int main()
{
	int choice;
	char cont;
	do
	{
		initMenu();
		cin >> choice;
		menuDecision(choice);

		do
		{
			cout << "Do u want to continue the program ? (Y/N)" << endl;
			cin >> cont;

		}while (cont != 'y' && cont != 'Y' && cont != 'n' && cont != 'N');

	}while(cont == 'Y' || cont == 'y');
	cin >> choice ; 

	return 0;
}
void initMenu()
{
	cout <<"Enter option:" << endl;
	cout <<"1. Circle" << endl;
	cout <<"2. Square" << endl;
	cout <<"3. Rectangle" << endl;
	cout <<"4. Triangle" << endl;
}
void menuDecision(int choice)
{
	double r, a, b,h;
	switch(choice)
	{
		case 1:
			cout << "Enter the radius" <<endl;
			cin >> r;
			areaCircle(r);
			break;
		case 2:
			cout << "Enter the sides of a square" <<endl;
			cin >> a;
			areaSquare(a);
			break;
		case 3:
			cout << "Enter the width and length of rectangle" <<endl;
			cin >> a >> b;
			areaRectangle(a,b);
			break;
		case 4:
			cout << "Enter the height" <<endl;
			cin >> h >> a;
			areaTriangle(h,a);
			break;
		default:
			cout <<"You didn't choose any of the option above" <<endl;
	}
}
double areaCircle(double r)
{
	double result = 3.14 * r *r;
	cout << "The area of cicle is "<< result << endl;
	return result;
}
double areaSquare(double r)
{
	double result =  r *r;
	cout << "The area of square is "<< result << endl;
	return result;
}

double areaRectangle(double a,double b)
{
	double result = a *b;
	cout << "The area of Rectangle is "<< result << endl;
	return result;
}
double areaTriangle(double r,double h)
{
	double result = (1/2.0) * r *h;
	cout << "The area of Triangle is "<< result << endl;
	return result;
}
