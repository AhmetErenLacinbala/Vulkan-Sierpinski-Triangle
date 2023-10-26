#include "first_app.hpp"
#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <algorithm>
#include <iostream>
#include <chrono>
#include <stdexcept>

namespace lve {

FirstApp::FirstApp() {
    loadGameObjects();
}

FirstApp::~FirstApp() {
}

void FirstApp::run() {
    SimpleRenderSystem simpleRendereSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};
    int currentDepth = -1;
    bool delayFlag = false;
    std::cout << "max push conts size = " << lveDevice.properties.limits.maxPushConstantsSize << "\n";
    float eraseTreshold = 0.01f;
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point lastTime = currentTime;
    while (!lveWindow.shouldClose()) {

        currentTime = std::chrono::high_resolution_clock::now();
        timeDifference = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();

        if (timeDifference >= 1.f) {
            std::cout<< "FPS: " << 1.f/timeDifference << std::endl;
            std::cout<< "memory: " << gameObjects.size() << std::endl;
            float offset = static_cast<float>(gameObjects.size());
            lastTime = currentTime;
            if(currentDepth < maxDepth - 2){
                gameObjects.erase(gameObjects.begin());
            }
            if (currentDepth < maxDepth-1) {
                currentDepth++;
            std::cout << currentDepth << std::endl;
            }
        }
        if (currentDepth < maxDepth-1) {

            gameObjects[0].alpha = 1 - fmod(timeDifference, 1.0f);
    
        }

       

        // poll events checks if any events are triggered (like keyboard or mouse input)
        // or dismissed the window etc.
        glfwPollEvents();
        if (auto commandBuffer = lveRenderer.beginFrame()) {
            lveRenderer.beginSwapChainRenderPass(commandBuffer);
            simpleRendereSystem.renderGameObjects(commandBuffer, gameObjects);
            lveRenderer.endSwapChainRenderPass(commandBuffer);
            lveRenderer.endFrame();
        }

        vkDeviceWaitIdle(lveDevice.device());
    }
}

void FirstApp::loadGameObjects() {
    std::vector<std::vector<LveModel::Vertex>> vertices;
    for (int i = 0; i < maxDepth; i++) {
        vertices.push_back(std::vector<LveModel::Vertex>());
    }
    int i = 0;
    sierpinski(vertices, maxDepth, {-1.f, 1.f}, {1.f, 1.f}, {0.0f, -1.f});
    for (auto &vertex : vertices) {
        auto lveModel = std::make_shared<LveModel>(lveDevice, vertex);
        auto triangle = LveGameObject::createGameObject();
        triangle.model = lveModel;
        triangle.color = {0.1f, 0.8f, 0.1f};
        triangle.depth = i++;
        gameObjects.push_back(std::move(triangle));
    }
}

void FirstApp::sierpinski(
    std::vector<std::vector<LveModel::Vertex>> &vertices,
    int depth,
    glm::vec2 left,
    glm::vec2 right,
    glm::vec2 top) {
    std::vector<LveModel::Vertex> triangleVertices = {
        {top},
        {right},
        {left}};
    vertices[maxDepth - depth].push_back({top});
    vertices[maxDepth - depth].push_back({right});
    vertices[maxDepth - depth].push_back({left});
    if (depth >= 0) {
        auto topleft = 0.5f * (top + left);
        auto topright = 0.5f * (top + right);
        auto rightleft = 0.5f * (right + left);
        sierpinski(vertices, depth - 1, topleft, left, rightleft);
        sierpinski(vertices, depth - 1, topright, rightleft, right);
        sierpinski(vertices, depth - 1, top, topleft, topright);
    }
}
} // namespace lve
