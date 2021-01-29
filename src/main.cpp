#define STB_IMAGE_IMPLEMENTATION

#include "imgui.h" 
#include "imgui_internal.h" 
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cstdint>

#include "camera.h"
#include "simpleGeometryCollection.h"
#include "shader.h"

#include "kittiTextParser.h"
#include "videoPlayer.h"
#include "tracklets.h"
#include "car.h"
#include "pointCloud.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#define WIDTH 1920
#define HEIGHT 1080

static void error_callback(int error, const char* description){
    fprintf(stderr, "Error %d: %s\n", error, description);
}


/*************************
 * CHANGE THIS
*************************/
std::string pathCalib = "D:/Dev/KittiViewer/dataset/";

/*************************
 * Globals
*************************/
const ImU32 u32_one = 1;

double g_lat = 35.889764;
double g_lon = 14.423190;
float g_zoom = 16.5f;

Shader* g_videoShader = nullptr;
Shader* g_trackletShader = nullptr;
Shader* g_tracklet3dShader = nullptr;
Shader* g_pointCloudShader = nullptr;
Shader* g_vertexColorShader = nullptr;

Camera* g_cam = nullptr;
SimpleQuad* g_videoQuadMesh = nullptr;
LineBox* g_trackletBoxMesh = nullptr;
CoordinateSystem* g_coordinateSystemMesh = nullptr;

VideoPlayer* g_videoPlayer = nullptr;
Tracklets* g_tracklets = nullptr;
Car* g_car = nullptr;
PointCloud* g_pointCloud = nullptr;

ProjectionTracklets2d* g_projectionTracklet = nullptr;

float g_cam_x = -50.0f;
float g_cam_y = 0.0f;
float g_cam_z = 50.0f;
int g_pointcloudIndex = 0;

std::string vertexShaderPath(std::string prefix, std::string name){
    return (prefix + "/resources/shaders/" + name + ".vert.glsl");
}

std::string fragmentShaderPath(std::string prefix, std::string name){
    return (prefix + "/resources/shaders/" + name + ".frag.glsl");
}

void initShader(){
    if(g_videoShader != nullptr){
        delete g_videoShader;
        g_videoShader = nullptr;
    }

    if(g_trackletShader != nullptr){
        delete g_trackletShader;
        g_trackletShader = nullptr;
    }

    if(g_pointCloudShader != nullptr){
        delete g_pointCloudShader;
        g_pointCloudShader = nullptr;
    }

    if(g_tracklet3dShader != nullptr){
        delete g_tracklet3dShader;
        g_tracklet3dShader = nullptr;
    }

    if(g_vertexColorShader != nullptr){
        delete g_vertexColorShader;
        g_vertexColorShader = nullptr;
    }
    
    #ifdef _SHADER_PATH
        std::string shaderPath = _SHADER_PATH;
        std::cout << "Default shader path: " << shaderPath << std::endl;
    #else
        std::string shaderPath = "";
    #endif

    g_videoShader =         new Shader(vertexShaderPath(shaderPath, "texturedQuad").c_str(),    fragmentShaderPath(shaderPath, "texturedQuad").c_str());
    g_trackletShader =      new Shader(vertexShaderPath(shaderPath, "tracklet").c_str(),        fragmentShaderPath(shaderPath, "tracklet").c_str());
    g_pointCloudShader =    new Shader(vertexShaderPath(shaderPath, "pointcloud").c_str(),      fragmentShaderPath(shaderPath, "pointcloud").c_str());
    g_tracklet3dShader =    new Shader(vertexShaderPath(shaderPath, "tracklet3d").c_str(),      fragmentShaderPath(shaderPath, "tracklet3d").c_str());
    g_vertexColorShader =   new Shader(vertexShaderPath(shaderPath, "vertexColor").c_str(),     fragmentShaderPath(shaderPath, "vertexColor").c_str());
}

void initGlobals(){
    initShader();

    g_cam = new Camera();
	g_cam->setProjectionMatrix(70.0f, WIDTH / HEIGHT, 1.0f, 500.0f);
    g_cam->setViewMatrix(glm::vec3(g_cam_x, g_cam_y, g_cam_z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    g_videoQuadMesh = new SimpleQuad();
    g_videoQuadMesh->upload();

    g_trackletBoxMesh = new LineBox();
    g_trackletBoxMesh->upload();

    g_coordinateSystemMesh = new CoordinateSystem();
    g_coordinateSystemMesh->upload();

    //CALIB
    g_projectionTracklet = new ProjectionTracklets2d();
    g_projectionTracklet->read_calib_cam_to_cam(pathCalib + "calib_cam_to_cam.txt");

    g_projectionTracklet->read_calib_velo_to_cam(pathCalib + "calib_velo_to_cam.txt");
    g_projectionTracklet->calculateProjection();

    //SYNC
    g_videoPlayer = new VideoPlayer(pathCalib + "image/data");

    g_car = new Car();
    g_car->read3dCarLocation(pathCalib + "oxts/data");

    g_pointCloud = new PointCloud();
    g_pointCloud->pcFileNames(pathCalib + "velodyne_points/data");


    //TRACKLETS
    g_tracklets = new Tracklets();
    g_tracklets->readXml(pathCalib + "tracklet_labels.xml");
}

void loadPointcloud(unsigned int index){
    g_pointCloud->deletePc();
    g_pointCloud->readPointCloud(index);
    g_pointCloud->uploadPc();
}

void drawVideoFrame(){
    glDepthMask(GL_FALSE);
    g_videoShader->enable();
    g_videoShader->uniform("texture", g_videoPlayer->getTextureIndex());
    g_videoShader->uniform("uChannels", g_videoPlayer->getTextureChannels());
    g_videoQuadMesh->draw();
    g_videoShader->disable();
    glDepthMask(GL_TRUE);
}

void drawPointCloud(unsigned int frame){
    g_pointCloudShader->enable();
    g_pointCloudShader->uniform("uViewMatrix", g_cam->viewMatrix);
    g_pointCloudShader->uniform("uProjMatrix", g_cam->projMatrix);

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    g_pointCloudShader->uniform("uModelMatrix", modelMatrix);

    g_pointCloudShader->uniform("uGlPointSize", 5.0f);

    g_pointCloud->drawPc();

    g_pointCloudShader->disable();
}

void drawTracklet3dFrame(unsigned int frame){
    for(unsigned int i = 0; i < g_tracklets->trackletVector.size(); i++){
        unsigned int first = g_tracklets->trackletVector[i].firstFrame;
        unsigned int poses = g_tracklets->trackletVector[i].poses;
        unsigned int last = first + poses;
        if(frame >= first && frame < last){
            g_tracklet3dShader->enable();

            float pitch = 0.0f;
            float yaw = 0.5f * glm::pi<float>();
            float roll = g_tracklets->trackletVector[i].rotations[frame - first].z;
            glm::mat4 modelMatrix = glm::mat4(glm::quat(glm::vec3(yaw, pitch, roll)));
            modelMatrix[0] *= g_tracklets->trackletVector[i].length;
            modelMatrix[1] *= g_tracklets->trackletVector[i].height;
            modelMatrix[2] *= g_tracklets->trackletVector[i].width;

            modelMatrix[3][0] = g_tracklets->trackletVector[i].translations[frame - first].x;
            modelMatrix[3][1] = g_tracklets->trackletVector[i].translations[frame - first].y;
            modelMatrix[3][2] = g_tracklets->trackletVector[i].translations[frame - first].z;

            g_tracklet3dShader->uniform("uModelMatrix", modelMatrix);

            g_tracklet3dShader->uniform("uColor", glm::vec4(g_tracklets->trackletVector[i].color));

            g_tracklet3dShader->uniform("uViewMatrix", g_cam->viewMatrix);
            g_tracklet3dShader->uniform("uProjectionMatrix", g_cam->projMatrix);

            g_trackletBoxMesh->draw();
            g_tracklet3dShader->disable();
        }
    }
}

void drawTrackletFrame(unsigned int frame){
    for(unsigned int i = 0; i < g_tracklets->trackletVector.size(); i++){
        unsigned int first = g_tracklets->trackletVector[i].firstFrame;
        unsigned int poses = g_tracklets->trackletVector[i].poses;
        unsigned int last = first + poses;
        if(frame >= first && frame < last){
            g_trackletShader->enable();

            g_trackletShader->uniform("uProjectionVideo", g_projectionTracklet->projectionVideo);

            g_trackletShader->uniform("P_rect_02", g_projectionTracklet->P_rect_02);
            g_trackletShader->uniform("R_rect_02", g_projectionTracklet->R_rect_02);
            g_trackletShader->uniform("R_rect_00", g_projectionTracklet->R_rect_00);
            g_trackletShader->uniform("transform_cam00_to_cam02", g_projectionTracklet->transform_cam00_to_cam02);
            g_trackletShader->uniform("transform_velo_to_cam00", g_projectionTracklet->transform_velo_to_cam00);


            g_trackletShader->uniform("uWidth", 1242.0f);
            g_trackletShader->uniform("uHeight", 375.0f);

            float pitch = 0.0f;
            float yaw = 0.5f * glm::pi<float>();
            float roll = g_tracklets->trackletVector[i].rotations[frame - first].z;
            glm::mat4 modelMatrix = glm::mat4(glm::quat(glm::vec3(yaw, pitch, roll)));
            modelMatrix[0] *= g_tracklets->trackletVector[i].length;
            modelMatrix[1] *= g_tracklets->trackletVector[i].height;
            modelMatrix[2] *= g_tracklets->trackletVector[i].width;

            modelMatrix[3][0] = g_tracklets->trackletVector[i].translations[frame - first].x;
            modelMatrix[3][1] = g_tracklets->trackletVector[i].translations[frame - first].y;
            modelMatrix[3][2] = g_tracklets->trackletVector[i].translations[frame - first].z;

            g_trackletShader->uniform("uModelMatrix", modelMatrix);

            g_trackletShader->uniform("uColor", glm::vec4(g_tracklets->trackletVector[i].color));

            g_trackletShader->uniform("uViewMatrix", g_cam->viewMatrix);
            g_trackletShader->uniform("uProjectionMatrix", g_cam->projMatrix);

            g_trackletBoxMesh->draw();
            g_trackletShader->disable();
        }
    }
}

void drawCar(unsigned int frame){
    g_vertexColorShader->enable();
    
    float pitch = g_car->m_carLocations[frame].pitch;
    float yaw = g_car->m_carLocations[frame].yaw;
    float roll = g_car->m_carLocations[frame].roll;
    glm::mat4 modelMatrix = glm::mat4(glm::quat(glm::vec3(yaw, pitch, roll)));

    modelMatrix[0] *= 1.0f;
    modelMatrix[1] *= 1.0f;
    modelMatrix[2] *= 1.0f;

    g_vertexColorShader->uniform("uModelMatrix", modelMatrix);
    g_vertexColorShader->uniform("uViewMatrix", g_cam->viewMatrix);
    g_vertexColorShader->uniform("uProjectionMatrix", g_cam->projMatrix);

    g_coordinateSystemMesh->draw();

    g_vertexColorShader->disable();
}

int main(void){
    /*************************
	 * Helper
	*************************/
    double lastTime = glfwGetTime();

    /*************************
	 * Setup opengl / buffers
	*************************/
    GLFWwindow* window;
    GLuint vertex_buffer;
    GLint vpos_location, vcol_location;
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit()){
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_SAMPLES, 2);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Simple example", NULL, NULL);
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
        exit(EXIT_FAILURE);
	};

    glfwSwapInterval(1);

	/*************************
	 * Imgui
	*************************/
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);
    
	/*************************
	 * Init OpenGl-Stuff
	*************************/
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    initGlobals();

    /*************************
	 * Init pointcloud
	*************************/
    g_pointCloud->readPointCloud(0);
    g_pointCloud->uploadPc();

	/*************************
	 * Renderloop
	*************************/
	while (!glfwWindowShouldClose(window)){   
        /*************************
	 	* Timer
		*************************/
        double currentTime = glfwGetTime();
        double delta = currentTime - lastTime;
        lastTime = currentTime;

        /*************************
	 	* Clear
		*************************/
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*************************
	 	* Draw OpenGl
		*************************/
        glLineWidth(1.0f);
        g_cam->setViewMatrix(glm::vec3(g_cam_x, g_cam_y, g_cam_z), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        g_videoPlayer->update();
        glViewport(0, 0, 1242, 375);
        drawVideoFrame();
        drawTrackletFrame(g_videoPlayer->currentFrame);

        glLineWidth(3.0f);
        glViewport(1242, 0, 678, 375);
        drawTracklet3dFrame(g_videoPlayer->currentFrame);
        drawPointCloud(g_videoPlayer->currentFrame);
        glLineWidth(1.0f);
        drawCar(g_videoPlayer->currentFrame);


        glViewport(0, 0, WIDTH, HEIGHT);

        /*************************
	 	* Imgui overlay
		*************************/
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

        /*Imgui-Kitti*/
        ImGui::Begin("Settings");
        float max = 50.0f;
        ImGui::SliderFloat("Cam x", &g_cam_x, -max, max);
        ImGui::SliderFloat("Cam y", &g_cam_y, -max, max);
        ImGui::SliderFloat("Cam z", &g_cam_z, -max, max);

        if(g_videoPlayer->changedFrame){
            loadPointcloud(g_videoPlayer->currentFrame);
        }
        
        if (ImGui::CollapsingHeader("Debug")){
            if (ImGui::Button("Reload shader")){
                initShader();
            }
        }
        
        g_videoPlayer->drawVideoOptions();
        g_tracklets->drawTrackletOptions(g_videoPlayer->currentFrame, g_projectionTracklet->projectionVideo);
        g_projectionTracklet->drawTextParserOptions();
        
        ImGui::End();
        
        ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	/*************************
	 * Exit, cleanup
	*************************/
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}