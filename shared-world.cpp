// Karl Yerkes
// MAT201B
// 2022-01-04
// minimal app, ready for adapting..
//

#include "al/app/al_App.hpp"
#include "al/graphics/al_Shapes.hpp"
#include "al/app/al_GUIDomain.hpp"

using namespace al;

#include <regex>
#include <unordered_map>

struct MyApp : App {
  osc::Send client;
  std::unordered_map<std::string, al::Pose> agent;
    Mesh cone{Mesh::TRIANGLES};

  void onCreate() override {
    client.open(9010, "255.255.255.255");
  
    addCone(cone);
    cone.generateNormals();
    cone.decompress();
  }

  void onMessage(osc::Message& m) override {
    // m.print();

    // receive someones pose
    if (m.addressPattern().find("/pose/") != std::string::npos) {
      std::string who;
      m >> who;

      Pose p;
      m >> p.pos().x;
      m >> p.pos().y;
      m >> p.pos().z;
      m >> p.quat().w;
      m >> p.quat().x;
      m >> p.quat().y;
      m >> p.quat().z;
      agent[who] = p;
    }
  }

  void onAnimate(double dt) override {
    // broadcast our pose
    client.send(std::string("/pose/"), "yourname",            //
                nav().pos().x, nav().pos().y, nav().pos().z,  //
                nav().quat().w, nav().quat().x, nav().quat().y, nav().quat().z);
  }

  void onDraw(Graphics& g) override { g.clear(0.2);

    for (auto a : agent) {
      g.pushMatrix();
      g.translate(a.second.pos());
      g.rotate(a.second.quat());
      g.scale(0.1);
      g.draw(cone);
      g.popMatrix();
    }
  }

  bool onKeyDown(const Keyboard& k) override { return false; }

  void onSound(AudioIOData& io) override {}

  void onInit() override {
    // set up GUI
    auto GUIdomain = GUIDomain::enableGUI(defaultWindowDomain());
    auto& gui = GUIdomain->newGUI();
    // gui.add(color);
  }
};

int main() {
  MyApp app;
  app.configureAudio(48000, 512, 2, 0);
  app.start();
}
