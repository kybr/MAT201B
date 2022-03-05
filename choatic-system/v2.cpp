// Karl Yerkes
// MAT201B
//
// Some interesting chaotic systems...
//
// * https://en.wikipedia.org/wiki/List_of_chaotic_maps
// * https://en.wikipedia.org/wiki/Logistic_map
// * https://en.wikipedia.org/wiki/Lorenz_system
// * https://en.wikipedia.org/wiki/Chua%27s_circuit
// * https://www.youtube.com/watch?v=m_Z-SIxqYcI
//   + See: "Dripping Faucet As a Model Chaotic System"
// * https://en.wikipedia.org/wiki/Duffing_equation
//
// Read _Chaos: Making a New Science_ (James Gleick)
//

#include "al/app/al_App.hpp"
#include "al/app/al_GUIDomain.hpp"

using namespace al;

// This is the Lorenz system

struct MyApp : App {
  static const int P = 9;
  static const int D = 2;
  Parameter p[P]{
      {"N", "p", 10000, 0, 100000},  // p[0] = N (simulation steps)
      {"h", "p", 0.01, 0, 0.018},    // p[1] = h (simulation time step)
      {"x0", "p", -0.72, -D, D},     // p[2] = x0 | initial
      {"y0", "p", -0.64, -D, D},     // p[3] = y0 | conditions
      {"z0", "p", 0, -D, D},         // p[4] = z0 |
      {"a", "p", 0.9, -3, 3},        //
      {"b", "p", -0.6, -3, 3},       //
      {"c", "p", 2.0, -3, 3},        //
      {"d", "p", 0.9, -3, 3},        //
  };

  Parameter width{"Width", 0.07, 0, 0.2};

  Mesh lorenz;
  bool light{false};

  void recompute() {
    lorenz.reset();
    lorenz.primitive(Mesh::LINE_STRIP);

    lorenz.vertex(p[2], p[3], p[4]);

    for (int i = 0; i < (int)p[0]; i++) {
      Vec3f _(lorenz.vertices().back());
      float a(p[5]), b(p[6]), c(p[7]), d(p[8]);
      lorenz.vertex(_.x * _.x - _.y * _.y + _.x * a + _.y * b,  // x
                    2 * _.x * _.y + _.x * c + _.y * d,          // y
                    _.z + 0.0                                   // z
      );
    }

    lorenz.ribbonize(width, true);
    lorenz.primitive(Mesh::TRIANGLE_STRIP);
    lorenz.generateNormals();
  }

  void onCreate() override {
    recompute();
    nav().pos(0, 0, 10);
  }

  void onDraw(Graphics& g) override {
    g.clear(0);

    // draw lorenz
    g.depthTesting(light);
    g.lighting(light);
    g.blendTrans();
    g.color(1);
    // g.scale(0.1);
    g.draw(lorenz);

    // draw red ends
    g.pointSize(8);
    Mesh m{Mesh::POINTS};
    m.vertex(lorenz.vertices()[0]);
    m.vertex(lorenz.vertices().back());
    g.color(1, 0, 0);
    g.draw(m);
  }

  bool onMouseDrag(Mouse const&) override {
    recompute();
    return true;
  }

  void onInit() override {
    auto GUIdomain = GUIDomain::enableGUI(defaultWindowDomain());
    auto& gui = GUIdomain->newGUI();
    for (auto& e : p) {
      gui.add(e);
      parameterServer() << e;
    }
    gui.add(width);
  }

  bool onKeyDown(const Keyboard& k) override {
    if (k.key() == ' ') {
      light = !light;
    }
    return false;
  }
  void onSound(AudioIOData& io) override {}
  void onAnimate(double) override {}
};

int main() {
  MyApp app;
  app.configureAudio(48000, 512, 2, 0);
  app.start();
}
