#include "../include/videoPlayer.h"

#include <algorithm>

VideoPlayer::VideoPlayer(std::string videoDir){
    strcpy(vDirArray, videoDir.c_str());
    loadFramesFromVideoDir();

    if(vFilenames.size() >= 1){
        initTexture();
    }
}

void VideoPlayer::loadFramesFromVideoDir(){
    DIR* dp = opendir(vDirArray);
    if (!dp){
        printf("ERROR: failed to open directory: %s\n", vDirArray);
    }

    while (true){
        errno = 0;
        dirent* de = readdir(dp);
        if (!de){
            break;
        }
        if (de->d_type == DT_REG){
            vFilenames.push_back(std::string(de->d_name));
        }
    }
    closedir(dp);

    std::sort(vFilenames.begin(), vFilenames.end());
}

void VideoPlayer::drawVideoOptions(){
    if (ImGui::CollapsingHeader("Video options")){
        if(ImGui::InputText("Dir imagedata", vDirArray, IM_ARRAYSIZE(vDirArray))){
            playVideo = false;
        }

        if (ImGui::Button("Load new directory")){
            playVideo = false;
            vFilenames.clear();
            loadFramesFromVideoDir();
            currentFrame = 0;
            previousFrame = 0;
            if(vFilenames.size() >= 1){
                std::string l = std::string(vDirArray) + "/" + vFilenames[currentFrame];
                vTexture->reloadFromImage(l);
            }
        }


        static bool inputs_step = true;
        if(ImGui::InputScalar("Frame", ImGuiDataType_U32, &currentFrame, inputs_step ? &u32_one : NULL, NULL, "%u")){
            playVideo = false;
            changedFrame = true;
            if(currentFrame != previousFrame){
                if(currentFrame >= vFilenames.size()){
                    currentFrame = vFilenames.size() - 1;
                }

                loadToTexture(currentFrame);
                previousFrame = currentFrame;
            }
        }else{
            changedFrame = false;
        }

        ImGui::CheckboxFlags("Play", &playVideo, 1);
    }
}

void VideoPlayer::loadToTexture(unsigned int index){
    std::string l = std::string(vDirArray) + "/" + vFilenames[index];
    vTexture->reloadFromImage(l);
}

void VideoPlayer::initTexture(){
    std::string l = std::string(vDirArray) + "/" + vFilenames[currentFrame];

    if(vTexture != nullptr){
        delete vTexture;
    }

    vTexture = new Texture(l,
                            Texture::WrapMode::Repeat,
                            Texture::FilterMode::Bilinear);
}

void VideoPlayer::update(){
    if(playVideo){
        changedFrame = true;
        currentFrame++;
        currentFrame = currentFrame % vFilenames.size();
        previousFrame = currentFrame;
        loadToTexture(currentFrame);
    }
}