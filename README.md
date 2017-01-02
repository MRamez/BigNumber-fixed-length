# BigNumber
Fixed-length BigNumber implementation in C++11

This is an implementation of BigNumber in C++11 using a fixed length array to hold the digits. It supports the same set of operations as the built-in `int` type. These operations behave similarly to their built-in counterparts.

The main class is named `BigNumber` and is defined in `Big_Number` namespace. To create a new `BigNumber`, you have to pass it a template parameter, which is a `std::size_t`. This template parameter indicates the maximum number of digits this number can have (i.e. the length of the array used to hold the digits). This class uses `std::array` instead of `std::vector` to store the digits. This is very useful (and results in faster computation) if you just want to calculate certain number of rightmost digits of the result. Furthermore, this gives you more control over how the class manages memory (you can determine how many digits you need). Also, operations on `std::array` are generally faster than `std::vector`, because `std::array` has a fixed lenght and resides in stack.

In designing this class, my first priority was to call existing functions whenever possible, and my second priority was performance. As a result, you may see some functions that delegate their work to other functions. This can decrease performace at some points, but makes the code easier to read and maintain. A notable function that its performace can be improved by copying and pasting is `operator%=`. This function can be implemented the same way `operator/=` is implemented (Their only difference is their return values).

Currently, `operator*=` is of O(n^2) (in multiplying two n-digit numbers). There is a better algorithm for multiplying big integers using Fast Fourier Transform (FFT). This algorithm is of O(n * log(n) * log(log(n))). I will try to rewrite `operator*=` using the latter.

Example:
```C++
//example.cpp
  
#include <iostream>
#include "BigNumber.h"

using namespace Big_Number;
using namespace std;
  
int main()
{
  ios_base::sync_with_stdio(false);
  BigNumber<20> bn1("4");
  bn1 ^= bn1;
  cout<<bn1<<'\n';
  BigNumber<20> bn2("-4");
  cout<<bn2<<'\n';
  bn2 *= bn2;
  cout<<bn2<<'\n';
  bn2 = BigNumber<20>("2") ^ 32;
  cout<<bn2<<'\n';
  BigNumber<10> bn3("38");
  BigNumber<10> bn4("-7");
  cout<<(bn3 / bn4)<<'\n';
  cout<<(bn3 % bn4)<<'\n';
  cout<<(bn3 * bn4)<<'\n';
  cout<<(bn3 - bn4)<<'\n';
  cout<<(-bn3 - bn4)<<'\n';
  cout<<(bn3 + bn4)<<'\n';
  cout<<(bn3 % -bn4)<<'\n';
  cout<<(bn4 ^ bn3)<<'\n';
  bn3[0] = 4;
  cout<<bn3<<'\n';
  cin>>bn4;
  cout<<bn4<<'\n';
  bn4.at(0) = 3;
  for (BigNumber<20> bi = 7; bi-- != 0; )
    cout<<bi<<'\n';
  cout<<(9 * bn4)<<'\n';
  cout<<(bn4 + "384958943")<<'\n';
  return 0;
}
```
