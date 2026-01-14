#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

int width = 600;
int height = 500;

struct Point{
    int x;
    int y;

    Point(int x, int y) : x(x), y(y){
        //nothing
    }

};

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

float clamp(float value, float min, float max){
    return std::max(min,std::min(max,value));
}

void setPixel(int x, int y, float red, float green, float blue, std::vector<uint8_t> & framebuffer) {
    int bufferIdx = (y*width + x)*3;
    framebuffer[bufferIdx]=clamp(red*255,0,255);
    framebuffer[bufferIdx+1]=clamp(green*255,0,255);
    framebuffer[bufferIdx+2]=clamp(blue*255,0,255);
}

void drawLine(Point a, Point b, std::vector<uint8_t> & framebuffer){
    int y_diff = b.y-a.y;
    int x_diff = b.x-a.x;
    int y_dir = 1;
    int x_dir = 1;

    if(y_diff<0){
        y_diff=-y_diff;
        y_dir=-1;
    }

    if(x_diff<0){
        x_diff=-x_diff;
        x_dir=-1;
    }

    int curPixel_x = a.x;
    int curPixel_y = a.y;
    bool alt = false;

    while(x_diff!=0&&y_diff!=0){
        if(x_diff==y_diff){
            curPixel_x = curPixel_x+x_dir;
            curPixel_y = curPixel_y+y_dir;
            x_diff--; y_diff--;
        } else if(x_diff>y_diff) {
            if(alt){
                curPixel_x = curPixel_x+x_dir;
                x_diff--;
            } else {
                curPixel_x = curPixel_x+x_dir;
                curPixel_y = curPixel_y+y_dir;
                x_diff--; y_diff--;
            }
            alt = !alt;
        } else {
            if(alt){
                curPixel_y = curPixel_y+y_dir;
                y_diff--;
            } else {
                curPixel_x = curPixel_x+x_dir;
                curPixel_y = curPixel_y+y_dir;
                x_diff--; y_diff--;
            }
            alt = !alt;
        }

        //color the pixel
        setPixel(curPixel_x,curPixel_y,1,0,0,framebuffer);
    }

    while(x_diff>y_diff){
        curPixel_x = curPixel_x+x_dir;
        x_diff--;
        setPixel(curPixel_x,curPixel_y,1,0,0,framebuffer);
    }

    while(y_diff>x_diff){
        curPixel_y = curPixel_y+y_dir;
        y_diff--;
        setPixel(curPixel_x,curPixel_y,1,0,0,framebuffer);
    }

    //set a,b to a diff color for visualization
    setPixel(a.x,a.y,0,1,0,framebuffer);
    setPixel(b.x,b.y,0,1,0,framebuffer);

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
            setPixel(w,h,1,1,1,framebuffer);
        }
    }
    drawLine(Point(234,120),Point(400,401),framebuffer);
    writePPMfile(width, height, framebuffer, "test.ppm");
    return 0;
}