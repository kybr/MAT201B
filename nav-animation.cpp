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
  Nav agent;
  Nav target;
  double time{0};
  double angle{0};

  void onCreate() override {

    // place the camera so that we can see the axes
    nav().pos(0.5, 0.7, 5);
    nav().faceToward(Vec3d(0, 0, 0));

    // Don't do this:
    // nav().faceToward(0, 0, 0);
    // because it will be interpreted as this:
    // nav().faceToward(Vec3d(0), Vec3d(0), 0);
    // which has no effect because of the final 0!
  }

  void randomize() {
    target.pos(r(), r(), r());
    target.quat().set(r(), r(), r(), r()).normalize();
  }

  void onAnimate(double dt) override {
    if (time > 5) {
      time -= 5;
      randomize();
    }
    time += dt;
    angle += 0.1;

    // The target (blue) aims for the origin (0,0,0) while the agent (red) aims
    // for the target. moveF(t) means "move forward" by amount t. faceToward
    // also accepts an amount t that controls the rate of steering.
    target.faceToward(Vec3d(0, 0, 0), 0.01);
    target.moveF(0.2);
    agent.faceToward(target, 0.01);
    agent.moveF(0.2);

    // XXX we have to step each Nav to get them to move and turn!!!
    // skip this and nothing will move.
    target.step(dt);
    agent.step(dt);

    // XXX there's an exception. The App's nav() automatically steps. Don't call
    // nav().step() because that is done automatically.

    // Might we also animate the App's nav()? This would be like making a
    // virtual robotic camera. We might decide the position and direction of the
    // camera based on what is happening in the virtual world. We could make
    // cinematic maneuvers such as a "dolly shot" or mimic styles such as Dutch
    // angle or "handheld". Cuts and fades are also possible.
  }

  void onDraw(Graphics &g) override {

    // graphics / drawing settings
    g.clear(1);
    g.meshColor();
    g.pointSize(10);

    g.rotate(angle, Vec3d(0, 1, 0));

    // draw the two nav positions
    {
      Mesh mesh(Mesh::POINTS);
      mesh.vertex(agent.pos());
      mesh.color(1, 0, 0);
      mesh.vertex(target.pos());
      mesh.color(0, 0, 1);
      g.draw(mesh);
    }

    // draw the two nav orientations
    {
      Mesh mesh(Mesh::LINES);

      mesh.vertex(agent.pos());
      mesh.color(1, 0, 0);
      mesh.vertex(agent.pos() + agent.quat().rotate(Vec3d(0, 0, -1)) * 0.1);
      mesh.color(1, 0, 0);

      mesh.vertex(target.pos());
      mesh.color(0, 0, 1);
      mesh.vertex(target.pos() + target.quat().rotate(Vec3d(0, 0, -1)) * 0.1);
      mesh.color(0, 0, 1);

      // draw the axes
      mesh.vertex(-10, 0, 0);
      mesh.vertex(10, 0, 0);
      mesh.vertex(0, -10, 0);
      mesh.vertex(0, 10, 0);
      mesh.vertex(0, 0, -10);
      mesh.vertex(0, 0, 10);
      for (int i = 0; i < 6; i++)
        mesh.color(0, 0, 0);

      g.draw(mesh);
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
