// Karl Yerkes
// MAT201B
// 2022-01-04
// minimal app, ready for adapting..
//

#include "al/app/al_App.hpp"
#include "al/app/al_GUIDomain.hpp"

using namespace al;

struct MyApp : App {
  ParameterColor color{"Color"};
  ParameterInt mode{"Mode", "", 1, 1, 4};
  Parameter value{"Value", 0, 0, 1};

  void onInit() override {
    // set up GUI
    auto GUIdomain = GUIDomain::enableGUI(defaultWindowDomain());
    auto& gui = GUIdomain->newGUI();
    gui.add(mode);   // add parameter to GUI
    gui.add(color);  // add parameter to GUI
    gui.add(value);  // add parameter to GUI

    parameterServer() << value;
  }

  void onMessage(osc::Message& m) override { m.print(); }

  void onCreate() override {}

  void onAnimate(double dt) override {}

  void onDraw(Graphics& g) override {
    g.clear(color.get() * t);

    // properties of things we can draw
    // position
    // scale
    // rotation
    // complexity
    // material
    // lighting
    // many?
    // shape
    //
    // curvature~
    // texture
  }

  bool onKeyDown(const Keyboard& k) override { return false; }

  void onSound(AudioIOData& io) override {
    while (io()) {
      float f = io.in(0) + io.in(1);
      // most recent sample

      // qualities of sound?
      // pitch / frequency / f0 (fundamnetal frequency)
      // pan / spatialization
      // amplitude / volume / loudness / power
      // envelope (amplitude changes over time)
      // timbre (spectral changes over time)
      // rhythm
      //
      // https://github.com/adamstark/Gist
    }
  }
};

int main() {
  MyApp app;
  app.configureAudio(48000, 512, 2, 0);
  app.start();
}
