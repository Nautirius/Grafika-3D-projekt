#pragma once

#include "pch.h"


struct Object {
    std::string name = "Object";

    Vec3 position{};
    Vec3 velocity{};

    float mass;
    float density;
    float radius;

    Vec4 color;

    float rotation = 0.0f;
    float rotSpeed = 0.0f;


    bool Initializing = false;
    bool Launched = false;
    bool target = false;
    bool glow = false;

    bool toDelete = false;

    // texture
    GLuint textureID = 0;
    bool hasTexture = false;
    std::string texturePath;

    Object(Vec3 p, Vec3 v, float m, float density_ = 3344.0f, Vec4 col = { 0,0,0,1 }, float _rotSpeed = 0.0f, bool Glow = false, const std::string& tex = "", std::string name = "Object");

    void loadTexture(const std::string& path);

    void UpdatePos(float simSpeed);

    void accelerate(float x, float y, float z, float simSpeed);

    bool CheckCollision(const Object& other) const;

    void Draw(GLUquadric* quad, bool texturedAllowed = true) const;
};