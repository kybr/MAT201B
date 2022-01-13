/*
  This example shows how to use Image, Array and Texture to read a .jpg file,
display it as an OpenGL texture and print the pixel values on the command line.
Notice that while the intput image has only 4 pixels, the rendered texture is
smooth.  This is because interpolation is done on the GPU.

  Karl Yerkes and Matt Wright (2011/10/10)
*/

#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

#include "al/app/al_App.hpp"
#include "al/graphics/al_Image.hpp"

using namespace al;
using namespace std;

class MyApp : public App {
 public:
  Mesh pic, rgb, hsv, somthing_else;
  Mesh actual;

  void onCreate() {
    const char *filename = "hubble.jpg";
    auto imageData = Image(filename);
    if (imageData.array().size() == 0) {
      cout << "failed to load image" << endl;
      exit(1);
    }
    cout << "loaded image size: " << imageData.width() << ", "
         << imageData.height() << endl;

    int W = imageData.width();
    int H = imageData.height();
    pic.primitive(Mesh::POINTS);

    // iterate through all the pixel, scanning each row
    for (int row = 0; row < H; row++) {
      for (int column = 0; column < W; column++) {
        auto pixel = imageData.at(column, H - row - 1);
        pic.vertex(1.0 * column / W, 1.0 * row / H, 0.0);
        pic.color(pixel.r / 255.0, pixel.g / 255.0, pixel.b / 255.0);

        // XXX you put more code here!
      }
    }
  }

  void onAnimate(double dt) override {
    // XXX you put more code here!
    // hint modify `actual`
    // make an animation parameter (float) `t`
  }

  bool onKeyDown(const Keyboard &k) override {
    // Use a switch to do something when a particular key is pressed
    switch (k.key()) {
      // For printable keys, we just use its character symbol:
      case '1':
        // XXX you put more code here!
        break;

      case '?':
        // XXX you put more code here!
        break;

        // XXX you put more code here!
        // hint reset animation parameter
      default:
        break;
    }
    return true;
  }

  void onDraw(Graphics &g) {
    g.clear(0.2f);
    g.meshColor();
    g.draw(actual);
  }
};

int main() {
  MyApp app;
  app.configureAudio(48000, 512, 2, 0);
  app.start();
}
