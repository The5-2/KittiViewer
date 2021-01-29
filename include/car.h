#ifndef CAR_H_
#define CAR_H_

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <errno.h>
#include <stdio.h>

#include "../contrib/imgui/include/imgui.h" 
#include "../contrib/imgui/include/imgui_internal.h" 
#include "../contrib/imgui/include/imgui_impl_glfw.h"
#include "../contrib/imgui/include/imgui_impl_opengl3.h"

struct CarLocation{
public:
    float lat;
    float lon;    
    float alt;    
    float roll;   
    float pitch;  
    float yaw;  
    float vn;    
    float ve;     
    float vf;     
    float vl;     
    float vu;     
    float ax;     
    float ay;     
    float az;     
    float af;     
    float al;     
    float au;     
    float wx;     
    float wy;     
    float wz;     
    float wf;     
    float wl;     
    float wu;     
    float posacc; 
    float velacc;
    float navstat;
    float numsats;
    float posmode;
    float velmode;
    float orimode;
};

inline std::ostream& operator<< (std::ostream& o, const CarLocation& c){
    o << "lat: " << c.lat << "\n";
    o << "lon: " << c.lon << "\n";
    o << "alt: " << c.alt << "\n";
    o << "roll: " << c.roll << "\n";
    o << "pitch: " << c.pitch << "\n";
    o << "yaw: " << c.yaw << "\n";
    o << "vn: " << c.vn << "\n";
    o << "ve: " << c.ve << "\n";
    o << "vf: " << c.vf << "\n";
    o << "vl: " << c.vl << "\n";
    o << "vu: " << c.vu << "\n";
    o << "ax: " << c.ax << "\n";
    o << "ay: " << c.ay << "\n";
    o << "az: " << c.az << "\n";
    o << "af: " << c.af << "\n";
    o << "al: " << c.al << "\n";
    o << "au: " << c.au << "\n";
    o << "wx: " << c.wx << "\n";
    o << "wy: " << c.wy << "\n";
    o << "wz: " << c.wz << "\n";
    o << "wf: " << c.wf << "\n";
    o << "wl: " << c.wl << "\n";
    o << "wu: " << c.wu << "\n";
    o << "posacc: " << c.posacc << "\n";
    o << "velacc: " << c.velacc << "\n";
    o << "navstat: " << c.navstat << "\n";
    o << "numsats: " << c.numsats << "\n";
    o << "posmode: " << c.posmode << "\n";
    o << "velmode: " << c.velmode << "\n";
    o << "orimode: " << c.orimode << "\n";
    
    return o;
}

#define MAX_SIZE_DIR_STRING 512

class Car{
public:
    Car();
    ~Car();

    void read3dCarLocation(std::string path);
    void drawCarOptions();

    std::vector<CarLocation> m_carLocations;
private:
    std::vector<std::string> m_vFilenames;
    char m_vDirArray[MAX_SIZE_DIR_STRING]; //Imgui cant handle std::string

};
#endif



// - lat:     latitude of the oxts-unit (deg)
// - lon:     longitude of the oxts-unit (deg)
// - alt:     altitude of the oxts-unit (m)
// - roll:    roll angle (rad),  0 = level, positive = left side up (-pi..pi)
// - pitch:   pitch angle (rad), 0 = level, positive = front down (-pi/2..pi/2)
// - yaw:     heading (rad),     0 = east,  positive = counter clockwise (-pi..pi)
// - vn:      velocity towards north (m/s)
// - ve:      velocity towards east (m/s)
// - vf:      forward velocity, i.e. parallel to earth-surface (m/s)
// - vl:      leftward velocity, i.e. parallel to earth-surface (m/s)
// - vu:      upward velocity, i.e. perpendicular to earth-surface (m/s)
// - ax:      acceleration in x, i.e. in direction of vehicle front (m/s^2)
// - ay:      acceleration in y, i.e. in direction of vehicle left (m/s^2)
// - az:      acceleration in z, i.e. in direction of vehicle top (m/s^2)
// - af:      forward acceleration (m/s^2)
// - al:      leftward acceleration (m/s^2)
// - au:      upward acceleration (m/s^2)
// - wx:      angular rate around x (rad/s)
// - wy:      angular rate around y (rad/s)
// - wz:      angular rate around z (rad/s)
// - wf:      angular rate around forward axis (rad/s)
// - wl:      angular rate around leftward axis (rad/s)
// - wu:      angular rate around upward axis (rad/s)
// - posacc:  velocity accuracy (north/east in m)
// - velacc:  velocity accuracy (north/east in m/s)
// - navstat: navigation status
// - numsats: number of satellites tracked by primary GPS receiver
// - posmode: position mode of primary GPS receiver
// - velmode: velocity mode of primary GPS receiver
// - orimode: orientation mode of primary GPS receiver