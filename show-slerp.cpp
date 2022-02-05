// Karl Yerkes
// MAT201B
// 2022-02-03
//
#include "al/app/al_App.hpp"
#include "al/app/al_GUIDomain.hpp"
#include "al/math/al_Random.hpp"

using namespace al;

double r() { return rnd::uniformS(); }

struct MyApp : App {
  Pose p, q;
  Parameter t{"t", 0.37, 0, 1};
  float angle{0};
  double time{0};

  void randomize() {
    p.pos(r(), r(), r());
    q.pos(r(), r(), r());
    p.quat().set(r(), r(), r(), r()).normalize();
    q.quat().set(r(), r(), r(), r()).normalize();
  }

  void onCreate() override {
    randomize();
    nav().pos(0, 0, 10);
  }

  void onAnimate(double dt) override {
    angle += 0.87;
    time += dt;
    if (time > 4) {
      time -= 4;
      randomize();
    }
  }

  void onDraw(Graphics &g) override {

    // graphics / drawing settings
    g.clear(1);
    g.meshColor();
    g.pointSize(10);
    g.rotate(angle, Vec3f(0, 1, 0));

    // interpolate the position and orientation of the two poses
    Vec3d w = Vec3d(q.pos()).lerp(p.pos(), t);
    Quatd u = Quatd(q.quat()).slerp(p.quat(), t);

    // building and drawing meshes

    {
      Mesh mesh(Mesh::LINES);

      // p
      mesh.vertex(p.pos());
      mesh.color(1, 0, 0);
      mesh.vertex(p.pos() + p.quat().rotate(Vec3d(0, 0, -1)));
      mesh.color(1, 0, 0);

      // q
      mesh.vertex(q.pos());
      mesh.color(0, 0, 1);
      mesh.vertex(q.pos() + q.quat().rotate(Vec3d(0, 0, -1)));
      mesh.color(0, 0, 1);

      // w
      mesh.vertex(w);
      mesh.color(0, 1, 0);
      mesh.vertex(w + u.rotate(Vec3d(0, 0, -1)));
      mesh.color(0, 1, 0);

      g.draw(mesh);
    }

    {
      Mesh mesh(Mesh::POINTS);

      // p
      mesh.vertex(p.pos());
      mesh.color(1, 0, 0);

      // q
      mesh.vertex(q.pos());
      mesh.color(0, 0, 1);

      // w
      mesh.vertex(w);
      mesh.color(0, 1, 0);

      g.draw(mesh);
    }
  }

  void onInit() override {
    auto GUIdomain = GUIDomain::enableGUI(defaultWindowDomain());
    auto &gui = GUIdomain->newGUI();
    gui.add(t);
  }
};

int main() {
  MyApp app;
  app.configureAudio(48000, 512, 2, 0);
  app.start();
}
