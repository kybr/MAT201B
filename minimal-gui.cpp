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

  void onInit() override {
    // set up GUI
    auto GUIdomain = GUIDomain::enableGUI(defaultWindowDomain());
    auto& gui = GUIdomain->newGUI();
    gui.add(mode);  // add parameter to GUI
    gui.add(color);  // add parameter to GUI
  }

  void onCreate() override {
    //
  }

  double t = 1.0;
  void onAnimate(double dt) override {
    //
    if (t < 1.0) {
      t += dt;  // time (seconds) accumulate until the process stops
    } else {
      t = 1.0;  // when the process stopped, t was bigger than 1.0
    }
  }

  void onDraw(Graphics& g) override {
    g.clear(color.get() * t);  // use parameter value
    //
  }

  bool onKeyDown(const Keyboard& k) override {
    t = 0.0;

    switch(k.key()) {
      case '1': 
      mode.set(1);
      break;

      case '2': 
      mode.set(2);
      break;

      case '3': 
      mode.set(3);
      break;

      case '4': 
      mode.set(4);
      break;

      default:
      break;
    }
    //
    return false;
  }

  void onSound(AudioIOData& io) override {
    //
  }
};

int main() {
  MyApp app;
  app.configureAudio(48000, 512, 2, 0);
  app.start();
}
