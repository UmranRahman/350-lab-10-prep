#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <chrono>

int main() {
    // OpenGL settings we request for:
    sf::ContextSettings settings;
    settings.depthBits = 24;   // 24-bit depth buffer
    settings.stencilBits = 8;  // 8-bit stencil buffer
    // Request OpenGL 2.1 for compatibility with both SFML/Graphics AND macOS:
    //  - SFML's Graphics module implementation relies on legacy fixed-function pipeline features
    //     of OpenGL, which means only up to OpenGL 2.1. Any newer version requires Compatibility
    //     profile (i.e., maintaining legacy features).
    //  - macOS has deprecated OpenGL support, meaning for anything newer than 2.1 (i.e., 3.0 and
    //      newer), only Core profile is supported, i.e., REMOVING legacy fixed-function features.
    // As a result, if you want to both support macOS AND use SFML's Graphics features (e.g.,
    // sf::RenderWindow), you can use at latest OpenGL 2.1, which is a nightmare.
    settings.majorVersion = 2;
    settings.minorVersion = 1;
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Shaders", sf::State::Windowed, settings);
    // Query the settings we actually received (may be different):
    const auto& actualSettings = window.getSettings();
    std::cout << "OpenGL version " << actualSettings.majorVersion << "."
              << actualSettings.minorVersion << " flags: " << actualSettings.attributeFlags << '\n';
    // Fullscreen triangle, as described in the prep.
    sf::VertexArray fullscreenTriangle(sf::PrimitiveType::Triangles, 3);
    fullscreenTriangle[0].position = {-1, -1};  // bottom-left
    fullscreenTriangle[1].position = {3, -1};   // bottom-right
    fullscreenTriangle[2].position = {-1, 3};   // top-left
    fullscreenTriangle[0].texCoords = {0, 0};
    fullscreenTriangle[1].texCoords = {2, 0};
    fullscreenTriangle[2].texCoords = {0, 2};
    // load vertex and fragment shaders:
    sf::Shader shader(std::filesystem::path{"main.vert"}, std::filesystem::path{"main.frag"});
    // TODO: eventually add another Shader here which uses the blur fragment shader,
    //3
    sf::Shader blurShader(std::filesystem::path{"main.vert"}, std::filesystem::path{"blur.frag"});
    sf::RenderTexture preBlur(window.getSize());

    // also create a sf::RenderTexture...
    auto startTime = std::chrono::steady_clock::now();

    while (window.isOpen()) {
        while (auto e = window.pollEvent()) {
            if (e->is<sf::Event::Closed>()) {
                return 0;
            }
        }
        
        
        // TODO: set shader uniforms and eventually draw into
        //1a
        shader.setUniform("windowSize", sf::Vector2f(window.getSize()));

        //1b
        sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
        float mouseX = static_cast<float>(mousePixel.x);
        float mouseY = static_cast<float>(window.getSize().y) - static_cast<float>(mousePixel.y);
        shader.setUniform("mousePos", sf::Vector2f(mouseX, mouseY));

        //2
        auto now = std::chrono::steady_clock::now();
        float timeMs = std::chrono::duration<float, std::milli>(now - startTime).count();
        shader.setUniform("time", timeMs);


        // a RenderTexture first before postprocessing into the window target.
        /// Pass 1: draw the gradient and circle into the offscreen texture
preBlur.clear(sf::Color::Black);
preBlur.draw(fullscreenTriangle, &shader);
preBlur.display();

// Pass 2: draw that texture through the blur into the window
blurShader.setUniform("texture", preBlur.getTexture());
blurShader.setUniform("windowSize", sf::Vector2f(window.getSize()));
window.clear(sf::Color::Black);
window.draw(fullscreenTriangle, &blurShader);
window.display();
    }
    return 0;
}
