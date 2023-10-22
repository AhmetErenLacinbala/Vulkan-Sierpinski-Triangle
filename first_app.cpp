#include "first_app.hpp"
#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <iostream>
#include <stdexcept>

namespace lve {

FirstApp::FirstApp() {
    loadGameObjects();
}

FirstApp::~FirstApp() {
}

void FirstApp::run() {
    SimpleRenderSystem simpleRendereSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};

    std::cout << "max push conts size = " << lveDevice.properties.limits.maxPushConstantsSize << "\n";
    float move = 0.2f;
    while (!lveWindow.shouldClose()) {

        auto currentTime = std::chrono::high_resolution_clock::now();
        timeDifference = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();

        if (timeDifference >= 1.f) { 

            float offset = static_cast<float>(gameObjects.size()); 
            // addNewTriangle(offset);
            lastTime = currentTime;
            std::cout << timeDifference << std::endl;
            std::cout << sizeof(gameObjects) << std::endl;
            std::cout << "slmm" << gameObjects.size() << "slmm\n";

            currentDepth--;
        }
        /*for (auto &gameObject : gameObjects) {
            // Update gameObject.alpha based on time or some other logic
            // For example, a simple fade-in effect:
            
        }*/
        for (auto &gameObject : gameObjects) {
            if (gameObject.depth == currentDepth + 1 && currentDepth > 0) {
                gameObject.alpha = 1 - fmod(timeDifference, 1.0f);
            }
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
    std::vector<LveModel::Vertex> vertices{};
    sierpinski(vertices, currentDepth, {-1.f, 1.f}, {1.f, 1.f}, {0.0f, -1.f});
}

void FirstApp::sierpinski(
    std::vector<LveModel::Vertex> &vertices,
    int depth,
    glm::vec2 left,
    glm::vec2 right,
    glm::vec2 top) {
    std::vector<LveModel::Vertex> triangleVertices = {
        {top},
        {right},
        {left}};

    
    auto lveModel = std::make_shared<LveModel>(lveDevice, triangleVertices);

    
    auto triangle = LveGameObject::createGameObject();
    triangle.model = lveModel;           
    triangle.depth = depth;              
    triangle.color = {0.1f, 0.8f, 0.1f}; 

   
    gameObjects.push_back(std::move(triangle));
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
