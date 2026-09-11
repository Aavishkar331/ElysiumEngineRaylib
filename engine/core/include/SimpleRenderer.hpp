#ifndef SIMPLERENDERER_HPP
#define SIMPLERENDERER_HPP

#include "raylib.h"
#include "Scene.hpp"
#include "BroadPhase.hpp"
#include <cmath>
#include <algorithm>

class SimpleRenderer {
public:
    bool drawDebugGrid = false;

    void Render(Scene& scene) {
        BeginDrawing();
        ClearBackground({119, 221, 119, 255});

        float screenW = (float)GetScreenWidth();
        float screenH = (float)GetScreenHeight();

        // --- Dynamic viewport ---
        // The box corners are at distance sqrt(hx²+hy²) from center at worst-case rotation (45°).
        // Scale ppm so that this diagonal fits within 85% of the smaller screen half-dimension.
        float hx = scene.physicsWorld.boundaryHalfExtents.x;
        float hy = scene.physicsWorld.boundaryHalfExtents.y;
        float halfDiag = sqrtf(hx * hx + hy * hy); // worst-case corner distance from center
        float ppm = std::min(screenW * 0.85f / (2.0f * halfDiag),
                             screenH * 0.85f / (2.0f * halfDiag));

        // Compute offset so the physics boundary center maps to screen center.
        float physCXpx = scene.physicsWorld.boundaryCenter.x * ppm;
        float physCYpx = scene.physicsWorld.boundaryCenter.y * ppm;
        float offX = screenW * 0.5f - physCXpx;
        float offY = screenH * 0.5f - physCYpx;

        // Helper: physics coords → screen pixel
        auto px = [&](float x) { return x * ppm + offX; };
        auto py = [&](float y) { return y * ppm + offY; };

        // --- Debug Grid ---
        if (drawDebugGrid) {
            float cellSize = BroadPhase::CELL_SIZE * ppm;
            Color gridColor = {60, 60, 60, 150};
            for (float x = fmodf(offX, cellSize); x <= screenW; x += cellSize)
                DrawLineV({x, 0}, {x, screenH}, gridColor);
            for (float y = fmodf(offY, cellSize); y <= screenH; y += cellSize)
                DrawLineV({0, y}, {screenW, y}, gridColor);
        }

        // --- Boundary Box (rotated outline) ---
        {
            float cx  = px(scene.physicsWorld.boundaryCenter.x);
            float cy  = py(scene.physicsWorld.boundaryCenter.y);
            float halfW = hx * ppm;
            float halfH = hy * ppm;
            float cosR = cosf(scene.physicsWorld.boundaryRotation);
            float sinR = sinf(scene.physicsWorld.boundaryRotation);

            Vector2 corners[4] = {
                {cx + (-halfW * cosR - (-halfH) * sinR), cy + (-halfW * sinR + (-halfH) * cosR)},
                {cx + ( halfW * cosR - (-halfH) * sinR), cy + ( halfW * sinR + (-halfH) * cosR)},
                {cx + ( halfW * cosR - ( halfH) * sinR), cy + ( halfW * sinR + ( halfH) * cosR)},
                {cx + (-halfW * cosR - ( halfH) * sinR), cy + (-halfW * sinR + ( halfH) * cosR)},
            };
            for (int i = 0; i < 4; ++i)
                DrawLineEx(corners[i], corners[(i + 1) % 4], 3.0f, WHITE);
        }

        // --- Physics Bodies ---
        for (auto& obj : scene.objects) {
            if (!obj->rigidBody) continue;
            auto& body = obj->rigidBody;

            for (auto& col : body->colliders) {
                Vec3 globalPos = body->LocalToGlobal(col.localCentroid);
                float sx = px(globalPos.x);
                float sy = py(globalPos.y);

                if (col.type == ColliderType::Sphere) {
                    DrawCircleV({sx, sy}, col.radius * ppm, WHITE);
                }
                else if (col.type == ColliderType::Box) {
                    float w = col.halfExtents.x * 2.0f * ppm;
                    float h = col.halfExtents.y * 2.0f * ppm;
                    Quat q = body->orientation.ToQuat();
                    float angle = 2.0f * std::atan2(q.z, q.w) * 180.0f / PI;
                    DrawRectanglePro({sx, sy, w, h}, {w / 2.0f, h / 2.0f}, angle, {100, 100, 100, 255});
                }

                if (drawDebugGrid) {
                    DrawCircleV({px(body->globalCentroid.x), py(body->globalCentroid.y)}, 2.0f, RED);
                    AABB bounds = body->GetAABB();
                    float ax = px(bounds.min.x);
                    float ay = py(bounds.min.y);
                    float aw = (bounds.max.x - bounds.min.x) * ppm;
                    float ah = (bounds.max.y - bounds.min.y) * ppm;
                    DrawRectangleLinesEx({ax, ay, aw, ah}, 1.0f, {0, 255, 0, 100});
                }
            }
        }

        EndDrawing();
    }
};

#endif
