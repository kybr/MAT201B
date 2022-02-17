// Karl Yerkes
// MAT201B
// 2022-02-03
//
#include "al/app/al_App.hpp"
#include "al/app/al_GUIDomain.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/math/al_Functions.hpp"
#include "al/math/al_Random.hpp"

using namespace al;

double r() { return rnd::uniformS(); }

const double neighborhood = 0.57;
const double near = 0.1;
const double far = 0.23;
const double boundary = 3.0;
const double turn = 0.009;
const double speed = 0.13;

struct MyApp : App {
  std::vector<Nav> agent;
  double time{0};
  double angle{0};
  Mesh mesh;


  void onCreate() override {
    nav().pos(0.5, 0.7, 5);
    nav().faceToward(Vec3d(0), Vec3d(0, 1, 0));

    agent.resize(3000);
    for (auto &a : agent) {
      a.pos(r(), r(), r());
      a.quat().set(r(), r(), r(), r()).normalize();
    }

    mesh.primitive(Mesh::TRIANGLE_FAN);
    mesh.vertex(0, 0, -2);
    mesh.color(0, 0, 0, 1);
    mesh.vertex(0, 1, 0);
    mesh.color(1, 0, 0, 0.5);
    mesh.vertex(-1, 0, 0);
    mesh.color(0, 1, 0, 0.5);
    mesh.vertex(1, 0, 0);
    mesh.color(0, 0, 1, 0.5);
    mesh.vertex(0, 1, 0);
    mesh.color(1, 0, 0, 0.5);
  }

  void onAnimate(double dt) override {

    for (int i = 0; i < agent.size(); i++) {

      if (i == agent.size() / 3) continue; // does its own thing

      // investigate the neighborhood of each agent...
      //
      Vec3d sum(0);
      Vec3d heading(0);
      int count = 0;
      for (int j = 0; j < agent.size(); j++) {
        if (i == j) continue; // ignore self

        float distance = (agent[j].pos() - agent[i].pos()).mag();
        if (distance > neighborhood) continue; // ignore far away agents

        // compute "average" center and heading of the local nieghborhood
        sum += agent[j].pos();
        heading += agent[j].quat().rotate(Vec3d(0, 0, -1));
        ++count;
      }

      // if there is a nieghborhood....
      if (count > 0) {
        //std::cout << "hood is " << count << " of " << agent.size() << std::endl;

        // face (steer) a little bit toward that average heading
        Vec3d target = agent[i].pos() + heading / count;
        agent[i].faceToward(target, Vec3d(0, 1, 0), turn);

        // face a little bit toward or away from the center
        Vec3d center = sum / count;
        Vec3d towardCenter = center - agent[i].pos();
        float distance = towardCenter.mag();
        if (distance < near) {
          agent[i].faceToward(center, Vec3d(0, 1, 0), -turn);
        } else if (distance > far) {
          agent[i].faceToward(center, Vec3d(0, 1, 0), turn);
        }
      }
    }

    for (auto &a : agent) a.moveF(speed);

    for (auto &a : agent) {
      a.pos().x = wrap(a.pos().x, boundary, -boundary);
      a.pos().y = wrap(a.pos().y, boundary, -boundary);
      a.pos().z = wrap(a.pos().z, boundary, -boundary);
    }

    for (auto &a : agent) a.step(dt);
  }

  void onDraw(Graphics &g) override {
    g.clear(1);
    g.meshColor();
    g.depthTesting(true);
    g.blendTrans();
    g.rotate(angle, Vec3d(0, 1, 0));
    for (auto &a : agent) {
      g.pushMatrix();
      g.translate(a.pos());
      g.rotate(a.quat());
      g.scale(0.01);
      g.draw(mesh);
      g.popMatrix();
    }
  }

  void onInit() override {
    auto GUIdomain = GUIDomain::enableGUI(defaultWindowDomain());
    auto &gui = GUIdomain->newGUI();
    // gui.add(t);
  }
};

int main() {
  MyApp app;
  app.configureAudio(48000, 512, 2, 0);
  app.start();
}
