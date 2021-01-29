#ifndef TANGRAM_MAP_H_
#define TANGRAM_MAP_H_

#include <memory>
#include <glm/glm.hpp>

#include "tangram.h"
#include "map.h"
#include "platform.h"

#include "../include/platform_impl.h"

//https://www.raspberrypi.org/blog/tangram-an-open-source-map-rendering-library/
//https://github.com/tangrams/PI-GPS

//http://discuss.cocos2d-x.org/t/combine-with-tangram-es/39637/4
//http://theoutpost.io/modern-map-renderers/tangram-es/mbtiles.html
//https://docs.mapbox.com/api/maps/#vector-tiles

//Tangram platforms:
//Implemt self (e.g. platform_impl.cpp)
// Linux platform:  https://github.com/tangrams/tangram-es/blob/master/platforms/linux/src/linuxPlatform.cpp
//                  https://github.com/tangrams/tangram-es/blob/master/platforms/linux/src/linuxPlatform.h

class TangramMap{
public:
    TangramMap();
    ~TangramMap();

    void setup();
    void renderMap(float dt);
    void drawTangramMapOptions();

private:
    std::unique_ptr<Tangram::Map> m_map;

    float m_zoom;
    double m_lon;
    double m_lat;
    uint8_t m_cameraType;
};

#endif