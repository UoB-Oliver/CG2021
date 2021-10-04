#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>


#define WIDTH 1920
#define HEIGHT 1080


std::vector<float> interpolateSingleFloats(float from, float to, int numberOfValues){
    float steps = (to-from)/(numberOfValues-1);
    std::vector<float> result;
    for (int i = 0; i < numberOfValues; i++)
    {
        result.push_back(from+i*steps);
    }
    return result;
}

std::vector<glm::vec3> interpolateThreeElementValues(glm::vec3 from, glm::vec3 to, int numberOfValues){
    std::vector<glm::vec3> result;
    
    std::vector<float> vecA;
    std::vector<float> vecC;
    std::vector<float> vecB;
    
    vecA = interpolateSingleFloats(from[0], to[0], numberOfValues);
    vecB = interpolateSingleFloats(from[1], to[1], numberOfValues);
    vecC = interpolateSingleFloats(from[2], to[2], numberOfValues);

    for (size_t i = 0; i < numberOfValues; i++)
    {
        result.push_back(glm::vec3(vecA[i], vecB[i], vecC[i]));
    }

    return result;
}

void drawA(DrawingWindow &window) {
    window.clearPixels();
    // std::cout << "reee" << std::endl;
    std::vector<float> result;
    result = interpolateSingleFloats(0, 255, WIDTH);
    for (size_t y = 0; y < window.height; y++) {
        for (size_t x = 0; x < window.width; x++) {
            float red = result[x];
            float green = result[x];
            float blue = result[x];
            uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
            window.setPixelColour(x, y, colour);
        }
    }
}

void drawB(DrawingWindow &window) {
    window.clearPixels();
    // std::cout << "reee" << std::endl;
    // result = interpolateSingleFloats(0, 255, WIDTH);
    glm::vec3 topLeft(255, 0, 0);        // red 
    glm::vec3 topRight(0, 0, 255);       // blue 
    glm::vec3 bottomRight(0, 255, 0);    // green 
    glm::vec3 bottomLeft(255, 255, 0);   // yellow

    std::vector<glm::vec3> left;
    std::vector<glm::vec3> right;

    left = interpolateThreeElementValues(topLeft, bottomLeft, HEIGHT);
    right = interpolateThreeElementValues(topRight, bottomRight, HEIGHT);

    std::vector<glm::vec3> result;
    for (size_t y = 0; y < window.height; y++) {
        result = interpolateThreeElementValues(left[y], right[y], WIDTH);
        for (size_t x = 0; x < window.width; x++) {
            float red = result[x][0];
            float green = result[x][1];
            float blue = result[x][2];
            uint32_t colour = (255 << 24) + (int(red) << 16) + (int(green) << 8) + int(blue);
            window.setPixelColour(x, y, colour);
        }
    }
}

void handleEvent(SDL_Event event, DrawingWindow &window) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
        else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
        else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
        else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        window.savePPM("output.ppm");
        window.saveBMP("output.bmp");
    }
}

int main(int argc, char *argv[]) {
    /*
    DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
    SDL_Event event;
    while (true) {
        // We MUST poll for events - otherwise the window will freeze !
        if (window.pollForInputEvents(event)) handleEvent(event, window);
        draw(window);
        // Need to render the frame at the end, or nothing actually gets shown on the screen !
        window.renderFrame();
    }
    */
    DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
    SDL_Event event;
    while (true) {
        Uint64 start = SDL_GetPerformanceCounter();

        drawA(window);
        // We MUST poll for events - otherwise the window will freeze !
        if (window.pollForInputEvents(event)) handleEvent(event, window);
        // Need to render the frame at the end, or nothing actually gets shown on the screen !
        window.renderFrame();

    	Uint64 end = SDL_GetPerformanceCounter();
	    float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
	    std::cout << "Current FPS: " << std::to_string(1.0f / elapsed) << std::endl;
    }
    

    // std::vector<float> result;
    // result = interpolateSingleFloats(8.5, 2.2, 7);
    // for(size_t i=0; i<result.size(); i++) std::cout << result[i] << " ";
    // std::cout << std::endl;

    // glm::vec3 from(1, 4, 9.2);
    // glm::vec3 to(4, 1, 9.8);
    // std::vector<glm::vec3> result;
    // result = interpolateThreeElementValues(from, to, 4);
    // for(size_t i=0; i<result.size(); i++) {
    //     std::cout << glm::to_string(result[i]) << std::endl;
    // }
    
}
