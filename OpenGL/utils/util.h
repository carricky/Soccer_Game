#ifndef UTIL_H
#define UTIL_H
#undef PI

#define MAX_PARTICLES 2000

#include <GL/gl.h>
#include <cmath>
#include <string>

namespace zagflws {
namespace global {
namespace util {

bool LoadData();
bool SaveData();
void GetCurrentImg(int w, int h, const std::string& fileprefix);

extern float slowDown;
extern float xSpeed;
extern float ySpeed;

extern GLuint loop;
extern GLuint col;
extern GLuint delay;

struct Particle {
  bool active;
  float life;
  float fade;
  
  float colorR;
  float colorG;
  float colorB;
  
  float posX;
  float posY;
  float posZ;
  
  float dirX;
  float dirY;
  float dirZ;
  
  float graX;
  float graY;
  float graZ;
};
extern Particle particle[MAX_PARTICLES];

}  // namespace util

namespace math {

extern const double PI;

template<typename T>
T cos(T radian) {
  return std::cos(radian / 180 * math::PI);
}

template<typename T>
T sin(T radian) {
  return std::sin(radian / 180 * math::PI);
}

void to_3d_vector(float v[3]);

}  // namespace math

struct DisplayMode {
  DisplayMode() {
    animation = false;
    wire = false;
    ambient = false;
    fullscreen = false;
    perspective = true;
    firstperson = true;
    rain = false;
    fog = false;
  }

  bool animation;
  bool wire;
  bool ambient;
  bool fullscreen;
  bool perspective;
  bool firstperson;
  bool rain;
  bool fog;
};

}  // namespace global
}  // namespace zagflws

#endif  // UTIL_H
