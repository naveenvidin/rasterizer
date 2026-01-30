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

struct Vector2d{
    float x;
    float y;

    Vector2d(float x, float y) : x(x), y(y){
        // nothing
    }

    Vector2d operator+(const Vector2d & other){
        return Vector2d(x + other.x,y + other.y);
    }

    Vector2d operator-(const Vector2d & other){
        return Vector2d(x - other.x,y - other.y);
    }
};

float cross(Vector2d vec1, Vector2d vec2) {
    return vec1.x*vec2.y - vec1.y*vec2.x;
}



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

bool checkRightOfLine(Point a, Point b, Point pixel){
    Vector2d vec_a = Vector2d(a.x,a.y);
    Vector2d vec_b = Vector2d(b.x,b.y);
    Vector2d vec_p = Vector2d(pixel.x,pixel.y);
    return cross(vec_b-vec_a,vec_p-vec_a)<0;
}

void setPixel(int x, int y, float red, float green, float blue, std::vector<uint8_t> & framebuffer) {
    int bufferIdx = (y*width + x)*3;
    framebuffer[bufferIdx]=clamp(red*255,0,255);
    framebuffer[bufferIdx+1]=clamp(green*255,0,255);
    framebuffer[bufferIdx+2]=clamp(blue*255,0,255);
}

// Bresenham's algorithm
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

        // how far are we from the actual slope of the line
        // the best way to intuitively imagine is by taking the case where dx=dy and then it'll make sense
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

void drawTriangle(Point a, Point b, Point c, std::vector<uint8_t> & framebuffer){
    //go through all the pixels in the screen and check if that point is on the right of all the lines.
    for(int i=0; i<width; i++){
        for(int j=0; j<height; j++){
            Point pixel(i,j);
            if ((checkRightOfLine(a,b,pixel) == checkRightOfLine(b,c,pixel)) && (checkRightOfLine(b,c,pixel) == checkRightOfLine(c,a,pixel))) {
                setPixel(i,j,0,1,0,framebuffer);
            }
        }
    }
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
    // drawLine(Point(234,120),Point(400,401),framebuffer);
    drawTriangle(Point(0,0),Point(300,401),Point(400,401),framebuffer);
    writePPMfile(width, height, framebuffer, "test.ppm");
    return 0;
}