
// Karl Yerkes
// MAT201B
// 2022-02-03
//
#include "al/app/al_App.hpp"
#include "al/app/al_GUIDomain.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/math/al_Random.hpp"

using namespace al;

double r() { return rnd::uniformS(); }

struct MyApp : App {
  std::vector<Nav> agent;
  double time{0};
  Mesh mesh;

  void onCreate() override {
    // camera
    nav().pos(0.1, 0.3, 1);
    nav().faceToward(Vec3d(0), Vec3d(0, 1, 0));

    agent.resize(1);
    agent[0].pos(0, 0, 0);

    // agent body
    mesh.primitive(Mesh::TRIANGLE_FAN);
    mesh.vertex(0, 0, -2);
    mesh.color(0, 0, 0);
    mesh.vertex(0, 1, 0);
    mesh.color(1, 0, 0);
    mesh.vertex(-1, 0, 0);
    mesh.color(0, 1, 0);
    mesh.vertex(1, 0, 0);
    mesh.color(0, 0, 1);
    mesh.vertex(0, 1, 0);
    mesh.color(1, 0, 0);
  }

  void onAnimate(double dt) override { time += dt; }

  void onDraw(Graphics &g) override {
    g.clear(1);

    // settings
    g.meshColor();
    g.depthTesting(true);

    // draw axes
    Mesh axes(Mesh::LINES);
    axes.vertex(-10, 0, 0);
    axes.vertex(10, 0, 0);
    axes.vertex(0, -10, 0);
    axes.vertex(0, 10, 0);
    axes.vertex(0, 0, -10);
    axes.vertex(0, 0, 10);
    g.draw(axes);

    // turn the work
    g.rotate(time * 13, Vec3d(0, 1, 0));
    g.rotate(time * 7, Vec3d(1, 0, 0));

    for (auto &a : agent) {
      g.pushMatrix();
      g.translate(a.pos());
      g.rotate(a.quat());
      g.scale(0.1);
      g.draw(mesh);
      g.popMatrix();
    }

  }
};

int main() {
  MyApp app;
  app.configureAudio(48000, 512, 2, 0);
  app.start();
}