#pragma once

#include "lve_device.hpp"
#include "lve_game_object.hpp"
#include "lve_renderer.hpp"
#include "lve_window.hpp"

#include <chrono>
#include <memory>
#include <vector>

namespace lve {

class FirstApp {
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;
    FirstApp();
    ~FirstApp();
    FirstApp(const FirstApp &) = delete;
    FirstApp &operator=(const FirstApp &) = delete;
    void run();

private:
    void loadGameObjects();
    bool isTime();
    void createSier();
    void sierpinski(
        std::vector<LveModel::Vertex> &vertices,
        int depth,
        glm::vec2 left,
        glm::vec2 right,
        glm::vec2 top);
    float cycle = 0;
    float defaultSize = 1.f;
    float timeDifference = .0f;
    int currentDepth = 9;

    std::vector<LveModel::Vertex> basicTriangleVertices = {
    {glm::vec2(0.0f, -1.0f)},
    {glm::vec2(1.0f, 1.0f)},
    {glm::vec2(-1.0f, 1.0f)}
};
    LveWindow lveWindow{WIDTH, HEIGHT, "sierpinski"};
    LveDevice lveDevice{lveWindow};
    LveRenderer lveRenderer{lveWindow, lveDevice};
    std::vector<LveGameObject> gameObjects;

    std::chrono::high_resolution_clock::time_point lastTime;
};
} // namespace lve