#include <iostream> 
#include <sstream>
#include <string> 

using namespace std;

typedef unsigned long long U64;

const double pi = 3.14159;
const char nl = '\n';

enum Board{
	a = 2,
   	b = 3,
	c = 4,
	d = 5,	
};

int main(int argc, char **argv){

	Board pt = a;

	string str = "abcdefg";
	cout << pt;
	return 0;

}
