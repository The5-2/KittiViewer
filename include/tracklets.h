#ifndef TRACKLETS_H_
#define TRACKLETS_H_

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "../contrib/tinyxml2/tinyxml2.h"
#include "../contrib/imgui/include/imgui.h" 
#include "../contrib/imgui/include/imgui_internal.h" 
#include "../contrib/imgui/include/imgui_impl_glfw.h"
#include "../contrib/imgui/include/imgui_impl_opengl3.h"

struct Tracklet{
    public:
        float width, height, length;
        glm::vec4 color;
        std::string type;
        int firstFrame;
        int poses;

        std::vector<glm::vec3> rotations;
        std::vector<glm::vec3> translations;
};

class Tracklets{
    public:
        enum class Type {
        Car,
        Van,
        Truck,
        Pedestrian,
        Sitter,
        Cyclist,
        Tram,
        Misc
        }type;

    public:
    std::string tDir;
    tinyxml2::XMLDocument xml_doc;
    std::vector<Tracklet> trackletVector;

    Tracklets();
    void drawTrackletOptions(unsigned int frame, glm::mat4 proj);
    bool readXml(std::string directory);
    glm::vec3 colorForType(std::string type);
};

#endif