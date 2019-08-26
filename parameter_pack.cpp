#include <iostream>

// Notes I learned:
// 1. ... before args is pack, afterwards is unpack
// 2. std::forward used if throwing templated arguments to another function
//   It neds both template typename and ...unpack
// 3. For other parameter pack, you always need recursive + base case

/*
Example with parameter pack + template add
*/
// Base case : Single value to add
template<typename T>
T MyAdd(T value) {
	return value;
}

// Recursive case
template<typename T , typename ...Args> 
T MyAdd(T value, Args&&... args) {
	return value + MyAdd(args...);
}

/*
Example with parameter pack + template printf
*/
// Base case: No more parametesr in MyPrintf()
void MyPrintf(const char* fmt) {
	std::cout << fmt ;
}

// Recursive case
template<typename T, typename ...Args>
void MyPrintf(const char* fmt, T t, Args&& ...args) {
	auto ptr = fmt;
	while (ptr) {
		if (*ptr == '%') {
			std::cout << t;
			ptr++;
			// Recurse
			MyPrintf(ptr, args...);
			return;
		}
		// print out the current character in fmt string
		std::cout << *ptr;
		ptr++;
	}
}


/*
Example with parameter pack + std::forward
*/
// Forward using unique_ptr
template<typename T, typename ...Args>
std::unique_ptr<T> MyMakeUnique(Args&&... args) {
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
 
 // Sample class definitions to test above classes
struct A {
    A(int&& n) { std::cout << "rvalue overload, n=" << n << "\n"; }
    A(int& n)  { std::cout << "lvalue overload, n=" << n << "\n"; }
};
 
class B {
public:
    template<class T1, class T2, class T3>
    B(T1&& t1, T2&& t2, T3&& t3) :
        a1_{std::forward<T1>(t1)},
        a2_{std::forward<T2>(t2)},
        a3_{std::forward<T3>(t3)}
    {
    }
 
private:
    A a1_, a2_, a3_;
};


int main() {
	// Simple add function
	std::cout << MyAdd(2) << std::endl;
	std::cout << MyAdd(3, 4, 5) << std::endl;
	std::cout << MyAdd(2, 4.2, 5) << std::endl;
	std::cout << MyAdd<double, double, int>(2, 4.2, 5) << std::endl;

	auto ptr = MyMakeUnique<int>(2);
	std::cout << *ptr << std::endl;

	int i = 4;
	auto t = MyMakeUnique<B>(2, i, 3);
	MyPrintf("Hello world %,% \n", 2, 3.3f);
	MyPrintf("Hello world % % blah blah % finally\n", 2, LONG_MAX, "done");


}