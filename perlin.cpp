#include "lodepng.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>


class Perlin {
    private:
        const int permutation[256] = { 151,160,137,91,90,15,                 // Hash lookup table as defined by Ken Perlin.  This is a randomly
    131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,    // arranged array of all numbers from 0-255 inclusive.
    190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
    88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
    77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
    102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
    135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
    5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
    223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
    129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
    251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
    49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
    };

    int* p;
    int repeat;


    public:
    Perlin(int repeat = -1) {
        p = new int[512];

        for(int i=0;i<512;i++){
            p[i] = permutation[i%256];
        }
        this->repeat = repeat;
    }

    double perline(double x, double y) {
  
        int xi = (int)floor(x) % 256;
        int yi = (int)floor(y) % 256;

        double xf = x-(int)x;
        double yf = y-(int)y;

        double u = fade(xf);
        double v = fade(yf);

        int topLeft,topRight,bottomLeft,bottomRight;
        
        bottomLeft = p[p[xi]+yi];
        bottomRight =p[p[inc(xi)]+yi];
        topLeft = p[p[xi]+inc(yi)];
        topRight = p[p[inc(xi)] + inc(yi)];

        double x1, x2;

    // Bottom edge of the unit square
    x1 = lerp(
        grad(bottomLeft, xf,     yf),      // bottom-left
        grad(bottomRight, xf - 1, yf),      // bottom-right
        u
    );

    // Top edge of the unit square
    x2 = lerp(
        grad(topLeft, xf,     yf - 1),  // top-left
        grad(topRight, xf - 1, yf - 1),  // top-right
        u
    );

    // Final vertical interpolation
    double value = lerp(x1, x2, v);

    // Normalize from theoretical range ≈ [-1, 1] to [0, 1]
    return (value + 1) / 2;
    }

    double lerp(double a,double b, double x) {
        return a + x*(b-a);
    }
    double grad(int hash, double x, double y) {
        int h = hash & 15;
        double u = h < 8 ? x : y;
        double v = h < 4 ? y : (h == 12 || h == 14 ? x : 0);  
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}   

    double fade(double t) {
        return t*t*t*(t*(6*t-15)+10);
    }

    int inc(int num) {
        return (num+1) % 256;
    }
};


void encodeOneStep(const char* filename, const std::vector<unsigned char>& image, unsigned width, unsigned height) {
    //encode and save
    unsigned error = lodepng::encode(filename, image, width, height,LCT_RGB);

    if (error) {
        std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    } else {
        std::cout << "Success! Wrote PNG to " << filename << std::endl;
    }
}

class Img {
    private:
        int width;
        int height;
        int bytePerPixel;
        int count;
        std::vector<unsigned char> pixels;
    public:
    Img(int w, int h, int bpp = 1) {
        width = w;
        height = h;
        count = 0;
        pixels = std::vector<unsigned char>(width*height*bpp);
        bytePerPixel = bpp;
    }

    void setPixel(int x, int y, unsigned char W) {
        pixels.at(y*width + x) = W;
        count ++;
    }

    void setPixel(int x, int y, unsigned char R, unsigned G, unsigned B) {
        pixels.at((y*width + x)*3) = R;
        pixels.at((y*width + x)*3 + 1) = G;
        pixels.at((y*width + x)*3 + 2) = B;
        count ++;
    }

    std::vector<unsigned char> getImage() {
        return pixels;
    }
};

double getNoise(Perlin p,double x, double y, int octaves, double persistence, double frequency, double amplitude, double lacunarity){			

        double total = 0;
        double maxValue = 0;

		for(int i=0;i<octaves;i++) {
			total += p.perline(x * frequency, y * frequency) * amplitude;
			
			maxValue += amplitude;
			
			amplitude *= persistence;
			frequency *= lacunarity;
		}
		
		return total/maxValue;
}

int generate(int width,int height, double octave, double persistence, double frequency, double amplitude,double lacunarity) {
    std::vector<unsigned char> image;
    image.resize(width * height); 
    Perlin p = Perlin();
    Img img = Img(width,height,3);
    for (int i=0;i< height;i++){
        for (int j=0;j< width; j++) {
            //double scale = 20;
            double x = i*20;
            double y = j*20;
            double noise = getNoise(p,x,y,(int)octave,persistence,frequency,amplitude,lacunarity);
            img.setPixel(i,j,noise*255,noise*255,noise*255);
        }
    }
    std::cout << img.getImage().size() << std::endl;
    encodeOneStep("output.png", img.getImage(), width, height);
    return 0;
}




