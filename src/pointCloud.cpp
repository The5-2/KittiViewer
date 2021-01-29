#include "../include/pointCloud.h"
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <algorithm>

PointCloud::PointCloud(){
    pointCloud.reserve(MAX_SIZE_POINTCLOUD);
}

PointCloud::~PointCloud(){
    pointCloud.clear();
}

void PointCloud::pcFileNames(std::string path){
    m_path = path;

    m_vFilenames.clear();

    DIR* dp = opendir(path.c_str());

    if (!dp){
        std::printf("ERROR: failed to open directory: %s\n", path.c_str());
    }

    while (true){
        errno = 0;
        dirent* de = readdir(dp);
        if (!de){
            break;
        }
        if (de->d_type == DT_REG){
            m_vFilenames.push_back(std::string(de->d_name));
        }
    }

    closedir(dp);

    std::sort(m_vFilenames.begin(), m_vFilenames.end());
}


void PointCloud::readPointCloud(unsigned int index){
    pointCloud.clear();

    // std::ifstream fin(m_path + "/" + m_vFilenames[index], std::ios::binary);
    // float f;
    // glm::vec4 v;
    // unsigned int count = 0;

    // while (fin.read(reinterpret_cast<char*>(&f), sizeof(float))){
    //     v[count] = f;
    //     count++;

    //     if(count == 3){
    //         count = 0;
    //         pointCloud.push_back(v);
    //     }
    // }

    // if(count != 0){
    //     printf("Error: Number of point cloud data not dividable by four (Leftover: %u).\n", count);
    // }

    float *data = (float*)malloc(MAX_SIZE_POINTCLOUD * sizeof(float));
    float *px = data+0;
    float *py = data+1;
    float *pz = data+2;
    float *pr = data+3;

    // load point cloud
    FILE *stream;
    std::string p = m_path + "/" + m_vFilenames[index];
    stream = fopen (p.c_str(),"rb");
    int32_t num = fread(data, sizeof(float), MAX_SIZE_POINTCLOUD, stream)/4;
    for (int32_t i=0; i < num; i++) {
        pointCloud.push_back(glm::vec4(*px,*py,*pz,*pr));
        px+=4; py+=4; pz+=4; pr+=4;
    }

    fclose(stream);

    delete data;
}

void PointCloud::uploadPc(){
    /*Bind position*/
    glGenBuffers(2, m_vbo_pc);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pc[0]);
    glBufferData(GL_ARRAY_BUFFER,         
        pointCloud.size() * sizeof(float) * 4, 
        &pointCloud[0].x, 
        GL_STATIC_DRAW);

    /*Bind color*/
    // glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pc[1]);
    // glBufferData(GL_ARRAY_BUFFER,         
    //     color.size() * sizeof(float) * 3, 
    //     &color[0].x, 
    //     GL_STATIC_DRAW);
}

void PointCloud::drawPc(){
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pc[0]);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // glEnableVertexAttribArray(1);
    // glBindBuffer(GL_ARRAY_BUFFER, m_vbo_pc[1]);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_POINTS, 0, pointCloud.size());

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // glDisableVertexAttribArray(1);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PointCloud::deletePc(){
    glDeleteBuffers(2, m_vbo_pc);
}