#include "CImg.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace cimg_library;
char asciiChar[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[] -_+~<>i!lI;:,\"^`'. ";

char getChar(int r, int g, int b, int a);

int main(int argc, char *argv[]) {
  cimg_usage("Turn pic to ascii art");
  const char* output = cimg_option("-o","ascii.txt","OUTPUT");
  int width = cimg_option("--width",0,"WIDTH");
  int height = cimg_option("--height",0,"HEIGHT");
  const char* input = cimg_option("-i","ascii.png","INPUT");
  cimg_option("-h"," ","help");
  if (!output)
    output = "ascii.txt";
  cimg_help("usage: ascii.py [-h] [-o OUTPUT] [--width WIDTH] [--height HEIGHT] [-i INPUT]");
  CImg<unsigned char> image(input);
  if (width == 0 || height == 0) {
    width = image.width();
    height = image.height();
  } else {
    image.resize(width, height);
  }
  
  
  ofstream fout (output);
  cimg_forY(image, y) {
    cimg_forX(image, x) {
      int r = image(x, y , 0);
      int g = image(x, y , 1);
      int b = image(x, y , 2);
      int a = image(x, y , 3);
      fout << getChar(r, g, b, a);
    }
    fout << "\n";
  }
  fout.close();
  return 0;
}

char getChar(int r, int g, int b, int a = 256) {
  if (a == 0)
    return ' ';
  int length = strlen(asciiChar);
  // RGB to gray
  double gray = 0.2126 * r + 0.7152 * g + 0.0722 * b;

  double unit = 256.0 / length;
  return asciiChar[static_cast<int>(gray / unit)];
}
