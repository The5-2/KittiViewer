#include "../include/car.h"
#include <algorithm>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #include "dirent.h"
#else
    #include <dirent.h>
#endif


Car::Car(){

}

Car::~Car(){

}

void Car::read3dCarLocation(std::string path){    
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


    for(unsigned int i = 0; i < m_vFilenames.size(); i++){
        std::ifstream infile(path + "/" + m_vFilenames[i]);
        std::string line;

        while (std::getline(infile, line)){
            std::istringstream ss(line);
            std::string temp;

            if(!ss.eof()){
                CarLocation carLoc;
                ss >> carLoc.lat >> carLoc.lon >> carLoc.alt >> carLoc.roll >> carLoc.pitch;
                ss >> carLoc.yaw >> carLoc.vn >> carLoc.ve >> carLoc.vf >> carLoc.vl;
                ss >> carLoc.vu >> carLoc.ax >> carLoc.ay >> carLoc.az >> carLoc.af;
                ss >> carLoc.al >> carLoc.au >> carLoc.wx >> carLoc.wy >> carLoc.wz;
                ss >> carLoc.wf >> carLoc.wl >> carLoc.wu >> carLoc.posacc >> carLoc.velacc;
                ss >> carLoc.navstat >> carLoc.numsats >> carLoc.posmode >> carLoc.velmode >> carLoc.orimode;

                m_carLocations.push_back(carLoc);
            }

        }

        infile.close();
    }
}