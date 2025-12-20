#include "problem_solutions.h"
#include <fstream>
#include <iostream>

using namespace std;

void d1p1(){
   ifstream input("input/d1.txt");
   string s;
   int pos = 50, ans = 0;
   while (input >> s){
      int movement = stoi(s.substr(1));
      pos += s[0] == 'R' ? movement : -movement;
      pos = (pos % 100 + 100) % 100;
      
      if (pos == 0){
         ans++;
      }
   }

   cout << "Part 1: " << ans << endl;
}

void d1p2(){
   ifstream input("input/d1.txt");
   string s;
   int pos = 50, ans = 0;
   int counter = 1;
   while (input >> s){
      int movement = stoi(s.substr(1));
      if (s[0] == 'R'){
         // Loop right
         pos += movement;
         ans += pos / 100;
         pos %= 100;
      } else {
         // Convert pos from "number of steps to right" to "number of steps to left"
         ans += ((100 - pos) % 100 + movement)/100;
         pos = ((pos - movement) % 100 + 100) % 100;
      }
      counter++;
   }

   cout << "Part 2: " << ans << endl;
}