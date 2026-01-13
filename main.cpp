#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

int width = 600;
int height = 500;

void writePPMfile(int width, int height, std::vector<uint8_t>& framebuffer, const std::string & filename){
    std::ofstream out(filename,std::ios::binary);
    if(!out){
        throw std::runtime_error("couldn't create file stream");
    }

    out << "P6\n";
    out << width << " " << height << "\n";
    out << "255\n";
    
    out.write(reinterpret_cast<const char*>(framebuffer.data()), framebuffer.size());
    out.close();
}

void setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue, std::vector<uint8_t> & framebuffer) {
    int bufferIdx = (y*width + x)*3;
    framebuffer[bufferIdx]=red;
    framebuffer[bufferIdx+1]=green;
    framebuffer[bufferIdx+2]=blue;
}
int main() {

    /*
    This is the format of a ppm file
    P3
    3 1
    255
    0 0 255  # Blue pixel
    255 0 0  # Red pixel
    0 255 0  # Green pixel
    */
    
    std::vector<uint8_t> framebuffer(width * height * 3);
    for(int h = 0; h < height; h++){
        for(int w = 0; w < width; w++){
            setPixel(w,h,255,255,255,framebuffer);
        }
    }
    writePPMfile(width, height, framebuffer, "test.ppm");
    return 0;
}