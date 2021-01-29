
#include <iostream>
#include <curl/curl.h>
#include <fstream>

#include "../include/tangramMap.h"
#include "../contrib/imgui/include/imgui.h" 
#include "../contrib/imgui/include/imgui_internal.h" 
#include "../contrib/imgui/include/imgui_impl_glfw.h"
#include "../contrib/imgui/include/imgui_impl_opengl3.h"

#define WIDTH 1242
#define HEIGHT 375

std::string sceneFile = "file:///home/dakr6016/workspace/0TestApplications/OpenGl_Kitti/resources/tangram/scene2.yaml";

struct SceneUpdate {
    std::string path;
    std::string value;
    SceneUpdate(std::string p, std::string v) : path(p), value(v) {}
    SceneUpdate() {}
};

TangramMap::TangramMap(){
    // curl_global_init(CURL_GLOBAL_DEFAULT);
}

TangramMap::~TangramMap(){
    // curl_global_cleanup();
}

void TangramMap::setup(){
    //Read the yaml file to string
    std::ifstream yamlFile ("/home/dakr6016/workspace/0TestApplications/OpenGl_Kitti/resources/tangram/scene2.yaml", std::ifstream::in);
    std::string yamlData((std::istreambuf_iterator<char>(yamlFile)),std::istreambuf_iterator<char>());
    yamlFile.close();
    m_map = std::make_unique<Tangram::Map>( std::move(std::make_unique<Tangram::PlatformImpl>()) );

    //Load yaml
    // Tangram::SceneID id = m_map->loadSceneYaml(yamlData, "");

    //Load scene
    m_map->loadSceneAsync(sceneFile, false);

	m_map->setupGL();
	m_map->resize(WIDTH, HEIGHT);
    m_map->setPixelScale( (float) WIDTH / (float) HEIGHT );

    m_map->setDefaultBackgroundColor(1.0f, 0.0f, 1.0f);

    // m_lon = 49.547559;
    // m_lat = 11.016135;
    m_lon = 40.70531887544228;
    m_lat = -74.00976419448853;
    m_zoom = 16.0f;
    m_cameraType = 0;
}

void TangramMap::renderMap(float dt){
    m_map->setPosition(m_lon, m_lat);
	m_map->setZoom(m_zoom);
    m_map->setCameraType(m_cameraType);

    // m_map->update(dt);
	// m_map->render();

    Tangram::MapState state = m_map->update(0.1);
    if (state.isAnimating()) {
        m_map->getPlatform().requestRender();
    }

	m_map->render();
}

void TangramMap::drawTangramMapOptions(){
    if (ImGui::CollapsingHeader("Tangram options")){
        static bool inputs_step = true;

        double f64_one = 0.000001;
        float f32_one = 1.0f; 
        uint8_t u8_one = 1;

        ImGui::InputScalar("Longitude",  ImGuiDataType_Double, &m_lon, inputs_step ? &f64_one : NULL);
        ImGui::InputScalar("Latitude",  ImGuiDataType_Double, &m_lat, inputs_step ? &f64_one : NULL);
        ImGui::InputScalar("Zoom",   ImGuiDataType_Float,  &m_zoom, inputs_step ? &f32_one : NULL);
        
        if(ImGui::InputScalar("CameraType",   ImGuiDataType_U8,  &m_cameraType, inputs_step ? &u8_one : NULL)){
            if(m_cameraType > 2){
                m_cameraType = 2;
            }
        }
    }
}