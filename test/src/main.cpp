#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    bool circle_exists = true;
    float circle_r = 200.0f;

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock deltaClock;
    ImGui::SetNextWindowSize(ImVec2(900, 900));
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        if (ImGui::Begin("Window title"))
        {
            ImGui::SetWindowFontScale(1.9);
            ImGui::Text("some text");
            ImGui::Checkbox("Circe", &circle_exists);
            ImGui::SliderFloat("Size of circle", &circle_r, 10, 500);


            if (ImGui::TreeNode("Selection State: Single Selection"))
            {
                static int selected = -1;
                for (int n = 0; n < 5; n++)
                {
                    char buf[32];
                    sprintf(buf, "Object %d", n);
                    if (ImGui::Selectable(buf, selected == n))
                        selected = n;
                }
                ImGui::TreePop();
            }
        }ImGui::End();



        if (ImGui::Begin("chat"))
        {
            if (ImGui::TreeNode("Password Input"))
            {
                static char password[64] = "password123";
                ImGui::InputText("password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
                ImGui::SameLine(); 
                ImGui::InputTextWithHint("password (w/ hint)", "<password>", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
                ImGui::InputText("password (clear)", password, IM_ARRAYSIZE(password));
                ImGui::TreePop();
            }


            if (ImGui::TreeNode("Selection State: Single Selection"))
            {
                static int selected = -1;
                for (int n = 0; n < 5; n++)
                {
                    char buf[32];
                    sprintf(buf, "Object %d", n);
                    if (ImGui::Selectable(buf, selected == n))
                        selected = n;
                }
                ImGui::TreePop();
            }
        }ImGui::End();


     

        //ImGui::ShowDemoWindow();

        window.clear();
        if (circle_exists)
            window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}