#ifndef KITTI_TEXT_PARSER_H_
#define KITTI_TEXT_PARSER_H_

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../contrib/imgui/include/imgui.h" 
#include "../contrib/imgui/include/imgui_internal.h" 
#include "../contrib/imgui/include/imgui_impl_glfw.h"
#include "../contrib/imgui/include/imgui_impl_opengl3.h"

/**************************
 * Only read information for projection into video
**************************/
//https://github.com/iralabdisco/kitti_player/blob/public/src/kitti_player.cpp

/**
 * @brief getCalibration
 * @param dir_root
 * @param camera_name
 * @param K double K[9]  - Calibration Matrix
 * @param D double D[5]  - Distortion Coefficients
 * @param R double R[9]  - Rectification Matrix
 * @param P double P[12] - Projection Matrix Rectified (u,v,w) = P * R * (x,y,z,q)
 * @return 1: file found, 0: file not found
 *
 *  from: http://kitti.is.tue.mpg.de/kitti/devkit_raw_data.zip
 *  calib_cam_to_cam.txt: Camera-to-camera calibration
 *
 *    - S_xx: 1x2 size of image xx before rectification
 *    - K_xx: 3x3 calibration matrix of camera xx before rectification
 *    - D_xx: 1x5 distortion vector of camera xx before rectification
 *    - R_xx: 3x3 rotation matrix of camera xx (extrinsic)
 *    - T_xx: 3x1 translation vector of camera xx (extrinsic)
 *    - S_rect_xx: 1x2 size of image xx after rectification
 *    - R_rect_xx: 3x3 rectifying rotation to make image planes co-planar
 *    - P_rect_xx: 3x4 projection matrix after rectification
 */
 
struct ProjectionTracklets2d{
    float d = 0.0f;
    glm::mat4 P_rect_02 = glm::mat4(d);
    glm::mat4 R_rect_02 = glm::mat4(d);
    glm::mat4 R_rect_00 = glm::mat4(d);
    glm::mat4 transform_cam00_to_cam02 = glm::mat4(d);
    glm::mat4 transform_velo_to_cam00 = glm::mat4(d);
    glm::mat4 projectionVideo = glm::mat4(d);

    void readMat_4x4(std::istringstream& ss, glm::mat4& m){
        ss  >> m[0][0] >> m[1][0] >> m[2][0] >> m[3][0]
            >> m[0][1] >> m[1][1] >> m[2][1] >> m[3][1]
            >> m[0][2] >> m[1][2] >> m[2][2] >> m[3][2];
            m[3][3] = 1.0f;
    }

    void readMat_3x4(std::istringstream& ss, glm::mat4& m){
        ss  >> m[0][0] >> m[1][0] >> m[2][0]
            >> m[0][1] >> m[1][1] >> m[2][1]
            >> m[0][2] >> m[1][2] >> m[2][2];
    }

    void read_calib_cam_to_cam(std::string path){
        std::ifstream infile(path);
        std::string line;

        while (std::getline(infile, line)){
            std::istringstream ss(line);
            string temp;

            if(!ss.eof()){
                ss >> temp;
            }

            if(temp == "P_rect_02:"){
                readMat_4x4(ss, P_rect_02);
            }else if(temp == "R_rect_00:"){
                readMat_3x4(ss, R_rect_00);
            }else if(temp == "R_rect_02:"){
                readMat_3x4(ss, R_rect_02);
            }else if(temp == "R_02:"){
                ss  >> transform_cam00_to_cam02[0][0] >> transform_cam00_to_cam02[1][0] >> transform_cam00_to_cam02[2][0]
                    >> transform_cam00_to_cam02[0][1] >> transform_cam00_to_cam02[1][1] >> transform_cam00_to_cam02[2][1]
                    >> transform_cam00_to_cam02[0][2] >> transform_cam00_to_cam02[1][2] >> transform_cam00_to_cam02[2][2];
            }else if(temp == "T_02:"){
                ss  >> transform_cam00_to_cam02[3][0] >> transform_cam00_to_cam02[3][1] >> transform_cam00_to_cam02[3][2];
                transform_cam00_to_cam02[3][3] = 1.0f;
            }else{
                continue;
            }
        }

        infile.close();
    }

    void read_calib_velo_to_cam(std::string path){
        std::ifstream infile(path);
        std::string line;

        while (std::getline(infile, line)){
            std::istringstream ss(line);
            string temp;

            if(!ss.eof()){
                ss >> temp;
            }

            if(temp == "R:"){
                ss  >> transform_velo_to_cam00[0][0] >> transform_velo_to_cam00[1][0] >> transform_velo_to_cam00[2][0]
                    >> transform_velo_to_cam00[0][1] >> transform_velo_to_cam00[1][1] >> transform_velo_to_cam00[2][1]
                    >> transform_velo_to_cam00[0][2] >> transform_velo_to_cam00[1][2] >> transform_velo_to_cam00[2][2];
            }else if(temp == "T:"){
                ss  >> transform_velo_to_cam00[3][0] >> transform_velo_to_cam00[3][1] >> transform_velo_to_cam00[3][2];
            }else{
                continue;
            }
        }

        infile.close();
    }

    void calculateProjection(){
        // projectionVideo = P_rect_02 * R_rect_02 * transform_cam00_to_cam02 * transform_velo_to_cam00;
        projectionVideo = P_rect_02 * R_rect_00 * transform_velo_to_cam00;
    }

    void drawMatrix(const glm::mat4& mat){
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                if(j != 0){
                    ImGui::SameLine();
                }
                ImGui::Button(std::to_string(mat[j][i]).c_str(), ImVec2(60,20));
            }
        }
    }

    void drawTextParserOptions(){
        if (ImGui::CollapsingHeader("Calib-Parser options")){
            if (ImGui::TreeNode("P_rect_02")){       
                drawMatrix(P_rect_02);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("R_rect_02")){       
                drawMatrix(R_rect_02);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("R_rect_00")){       
                drawMatrix(R_rect_00);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("transform_cam00_to_cam02")){       
                drawMatrix(transform_cam00_to_cam02);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("transform_velo_to_cam00")){       
                drawMatrix(transform_velo_to_cam00);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("projectionVideo")){       
                drawMatrix(projectionVideo);
                ImGui::TreePop();
            }
        }
    }
};


/**************************
 * Read all calibration info
**************************/
struct CalibInfo{
    std::string name;
    std::vector<float> values;
};

std::ostream& operator << (std::ostream& o, const CalibInfo& a){
    o << "Name: " << a.name << std::endl;

    for(unsigned int i = 0; i < a.values.size(); i++){
        o << "  " << a.values[i] << std::endl;
    }

    return o;
};

bool isFloat(string myString, float& result) {
    std::istringstream iss(myString);
    iss >> noskipws >> result;
    return iss.eof() && !iss.fail(); 
}

std::vector<CalibInfo> readCalibrationFromFile(std::string path){
    std::vector<CalibInfo> calib;
    
    std::ifstream infile(path);
    std::string line;
    bool first = true;

    while (std::getline(infile, line)){
        std::istringstream ss(line);
        if(first){
            first = false;
            continue;
        }

        std::string temp;
        bool name = true;
        CalibInfo c;

        while (!ss.eof()){ 
            ss >> temp;
            if(name){
                temp.pop_back();
                c.name = temp;
                name = false;
            }else{
                float f;
                if(isFloat(temp, f)){
                    c.values.push_back(f);
                }
            }  
        } 
        std::cout << c << std::endl;
    }

    return calib;
};

#endif