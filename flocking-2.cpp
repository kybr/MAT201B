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

const double neighborhood = 0.5;
const double near = 0.1;
const double far = 0.2;

struct MyApp : App {
  std::vector<Nav> agent;
  double time{0};
  double angle{0};
  Mesh mesh;

  void onCreate() override {
    nav().pos(0.5, 0.7, 5);
    nav().faceToward(Vec3d(0), Vec3d(0, 1, 0));

    agent.resize(200);
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
    time += dt;
    if (time > 7) {
      time -= 7;
      int n = rnd::uniform(agent.size());
      agent[n].pos(r(), r(), r());
      agent[n].quat().set(r(), r(), r(), r()).normalize();
    }

    for (int i = 0; i < agent.size(); i++) {
      Vec3d sum(0);
      Vec3d heading(0);
      int count = 0;
      for (int j = 0; j < agent.size(); j++) {
        if (i == j) continue;
        float distance = (agent[j].pos() - agent[i].pos()).mag();
        if (distance > neighborhood) continue;
        sum += agent[j].pos();
        heading += agent[j].quat().rotate(Vec3d(0, 0, -1));
        ++count;
      }
      if (count > 0) {
        //std::cout << "hood is " << count << " of " << agent.size() << std::endl;
        Vec3d target = agent[i].pos() + heading / count;
        agent[i].faceToward(target, Vec3d(0, 1, 0), 0.01);

        Vec3d center = sum / count;
        Vec3d towardCenter = center - agent[i].pos();
        float distance = towardCenter.mag();
        if (distance < near) {
          agent[i].faceToward(center, Vec3d(0, 1, 0), -0.01);
        } else if (distance > far) {
          agent[i].faceToward(center, Vec3d(0, 1, 0), 0.01);
        }
      }
    }

    // agent[0].faceToward(agent.back(), Vec3d(0, 1, 0));
    // for (int i = 1; i < agent.size(); i++) {
    //   agent[i].faceToward(agent[i - 1], 0.1);
    // }

    for (auto &a : agent) a.moveF(0.1);
    for (auto &a : agent) a.step(dt);

    for (auto &a : agent) {
      const double N = 3.0;
      a.pos().x = wrap(a.pos().x, N, -N);
      a.pos().y = wrap(a.pos().y, N, -N);
      a.pos().z = wrap(a.pos().z, N, -N);
    }
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
