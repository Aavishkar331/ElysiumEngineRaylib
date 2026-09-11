#include <Elysium.hpp>
#include "raylib.h"
#include <memory>
#include <cstdlib>
#include <iostream>
#include "Scene.hpp"
#include "GameObject.hpp"
#include "SimpleRenderer.hpp"
#include "Component.hpp"
#include "Input.hpp"

class LifeSpan : public Component {
public:
    float remaining;
    LifeSpan(float duration) : remaining(duration) {}

    void Update(float dt) override {
        remaining -= dt;
        if (remaining <= 0) {
            gameObject->active = false;
        }
    }
};

class PlayerController : public Component {
public:
    float speed = 10.0f;

    void Update(float dt) override {
        if (!gameObject->rigidBody) return;

        Vec3 velocity(0, 0, 0);
        if (Input::IsKeyDown(KEY_A)) velocity.x -= 1.0f;
        if (Input::IsKeyDown(KEY_D)) velocity.x += 1.0f;
        if (Input::IsKeyDown(KEY_W)) velocity.y -= 1.0f;
        if (Input::IsKeyDown(KEY_S)) velocity.y += 1.0f;

        if (velocity.MagnitudeSquared() > 0) {
            velocity = velocity.Normalized() * speed;
            gameObject->rigidBody->linearVelocity.x = velocity.x;
            gameObject->rigidBody->linearVelocity.y = velocity.y;
        } else {
            // Apply some damping if no input
            gameObject->rigidBody->linearVelocity.x *= 0.9f;
            gameObject->rigidBody->linearVelocity.y *= 0.9f;
        }

        if (Input::IsKeyPressed(KEY_SPACE)) {
            gameObject->rigidBody->linearVelocity.y = -10.0f; // Jump!
        }
    }
};

class Sandbox : public Elysium::Application {
public:
    Sandbox() {
    }

    ~Sandbox() {
    }

    void Run() {
        // 1. Setup Scene
        Scene scene;
        scene.physicsWorld.gravity = Vec3(0.0f, 9.81f, 0.0f);

        // 2. Create Initial Dynamic Balls
        for (int i = 0; i < 3; ++i) {
            auto ball = std::make_shared<GameObject>("Ball " + std::to_string(i));
            ball->position = Vec3(3.0f + i * 2.0f, 2.0f, 0.0f);
            auto& rb = ball->CreateRigidBody();
            rb.linearVelocity = Vec3(0.5f * i, 2.0f, 0.0f);
            rb.AddColliders(Collider::CreateSphere(0.5f, 1.0f));
            scene.AddGameObject(ball);
        }

        // 3. Setup Window and Renderer
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        InitWindow(800, 600, "Elysium Engine");
        SetTargetFPS(60);
        SimpleRenderer renderer;

        // Restore default boundary (5×5m) — renderer auto-scales to fill screen.
        scene.physicsWorld.SetBoundaries(
            Vec3(8.0f, 6.0f, 0.0f),
            Vec3(5.0f, 5.0f, 0.0f),
            0.0f
        );

        while (!WindowShouldClose()) {
            Input::Update();

            float dt = GetFrameTime();
            if (dt > 0.1f) dt = 0.1f;

            // Toggle debug grid
            if (Input::IsKeyPressed(KEY_G)) {
                renderer.drawDebugGrid = !renderer.drawDebugGrid;
            }

            // Spawn ball on mouse click — convert screen → physics coords via dynamic ppm
            if (Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                auto ball = std::make_shared<GameObject>("New Ball");
                Vector2 mousePos = Input::GetMousePosition();
                float screenW = (float)GetScreenWidth();
                float screenH = (float)GetScreenHeight();
                float hx = scene.physicsWorld.boundaryHalfExtents.x;
                float hy = scene.physicsWorld.boundaryHalfExtents.y;
                float halfDiag = sqrtf(hx * hx + hy * hy);
                float ppm = std::min(screenW * 0.85f / (2.0f * halfDiag),
                                     screenH * 0.85f / (2.0f * halfDiag));
                float offX = screenW * 0.5f - scene.physicsWorld.boundaryCenter.x * ppm;
                float offY = screenH * 0.5f - scene.physicsWorld.boundaryCenter.y * ppm;
                ball->position = Vec3((mousePos.x - offX) / ppm, (mousePos.y - offY) / ppm, 0.0f);

                auto& rb = ball->CreateRigidBody();
                rb.linearVelocity = Vec3((rand() % 10 - 5), (rand() % 10 - 5), 0);
                rb.AddColliders(Collider::CreateSphere(0.4f, 1.0f));

                ball->AddComponent<LifeSpan>(5.0f);
                scene.AddGameObject(ball);
            }

            scene.physicsWorld.boundaryRotation += 1.5f * dt; // 1.5 rad/s ≈ one spin per 4s

            scene.Update(dt);
            renderer.Render(scene);
        }

        CloseWindow();
    }
};

Elysium::Application* Elysium::CreateApplication() {
    return new Sandbox();
}
