#include <iostream>
#include <iomanip>
#include <cmath>


namespace LenaSpace {
  int weight = 0;
  enum MyMood {
    BAD,
    GOOD,
    PERFECT
  };
  int length = 150;
  MyMood mood = BAD;

  void eatPie(){
    std::cout << "Я покушала\n";
    mood = (MyMood) 1;
    mood = PERFECT;
  }
  void doSmth(){
    
  }
}
int main() {
  double a, b, c, s, p;
  std::cin >> a, b, c;
  p = (a+b+c)/2;
  std::cout << p << " ";
  s = sqrt(p * (p - a) * (p - b) * (p - c));
  std::cout << s;
  return 0; 
}
// fpu - для работы везщественных чисел что то там 
