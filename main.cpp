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

//bresenham's algorithm
void drawLine(Point a, Point b, std::vector<uint8_t>& framebuffer) {
    int x0 = a.x;
    int y0 = a.y;
    int x1 = b.x;
    int y1 = b.y;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        setPixel(x0, y0, 1, 0, 0, framebuffer);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }

    setPixel(a.x, a.y, 0, 1, 0, framebuffer);
    setPixel(b.x, b.y, 0, 1, 0, framebuffer);
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