#ifndef VIDEOPLAYER_H_
#define VIDEOPLAYER_H_

#include <string>
#include <vector>
#include <string>
#include <vector>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #include "dirent.h"
#else
    #include <dirent.h>
#endif
#include <errno.h>

#include "texture.h"
#include "../contrib/imgui/include/imgui.h" 
#include "../contrib/imgui/include/imgui_internal.h" 
#include "../contrib/imgui/include/imgui_impl_glfw.h"
#include "../contrib/imgui/include/imgui_impl_opengl3.h"

#define MAX_SIZE_DIR_STRING 512

class VideoPlayer{
    public:
    unsigned int currentFrame = 0;
    bool changedFrame;
    
    private:
    std::vector<std::string> vFilenames;
    Texture* vTexture = nullptr;
    char vDirArray[MAX_SIZE_DIR_STRING]; //Imgui cant handle std::string
    
    /*Imgui helper*/
    const ImU32 u32_one = 1;
    unsigned int previousFrame = 0;

    unsigned int playVideo = 0; //Used as bool by imgui

    public:
    VideoPlayer(std::string videoDir);

    void loadFramesFromVideoDir();
    void drawVideoOptions();
    void update();

    void initTexture();
    void loadToTexture(unsigned int index);

    GLuint getTextureIndex() const{
        return vTexture->index;
    }

    int getTextureChannels() const{
        return vTexture->channels;
    }
};
#endif