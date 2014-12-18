#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef void (*funcRef)(int);
typedef void (func)(int);

void FunTest(int i)
{
	cout << i << endl;
}

class A
{
public:
	int &Re()
	{
		return a;
	}
	int a;
};

void Test(int &a)
{
	a = 1;
	return;
}

int &Ref(int &a)
{
	return a;
}

int main()
{
	/*int a = 5;
	int &b = Ref(a);
	b =6;
	cout << a << endl;
	A test;
	test.a = 3;
	int &c = test.Re();
	c = 5;
	cout << test.a << endl;
	string s("fef");
	string x(s + "fef");
	cout << x << endl;*/
	vector<double> *x = new vector<double>(4);
	(*x)[0] = 5;
	cout << (*x)[0] << endl;
	return 0;
}
