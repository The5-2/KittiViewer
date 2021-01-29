#ifndef POINT_CLOUD_H_
#define POINT_CLOUD_H_

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

#define MAX_SIZE_POINTCLOUD 1000000 //https://github.com/MarkMuth/QtKittiVisualizer/tree/master/kitti-devkit-raw

class PointCloud{
public:
    PointCloud();
    ~PointCloud();

    void pcFileNames(std::string path);
    void readPointCloud(unsigned int index);
    void uploadPc();
    void drawPc();
    void deletePc();

    unsigned int getSize(){
        return pointCloud.size();
    }

private:
    std::string m_path;
    std::vector<glm::vec4> pointCloud;
    std::vector<std::string> m_vFilenames;
    GLuint m_vbo_pc[2];
};

#endif