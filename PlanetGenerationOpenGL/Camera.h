#pragma once

#include "pch.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct Camera {
    float yaw = 0.0f;
    float pitch = 20.0f;
    float distance = 8000.0f;

    Vec3 target{ 0,0,0 };

    float yawSensitivity = 0.2f;
    float pitchSensitivity = 0.2f;
    float zoomSensitivity = 1.1f;

    Vec3 computePosition() const {
        float radYaw = yaw * std::numbers::pi / 180.0f;
        float radPitch = pitch * std::numbers::pi / 180.0f;

        float x = target.x + distance * std::cos(radPitch) * std::cos(radYaw);
        float y = target.y + distance * std::sin(radPitch);
        float z = target.z + distance * std::cos(radPitch) * std::sin(radYaw);

        return { x, y, z };
    }

    glm::vec3 getPosition() const {
        Vec3 p = computePosition();
        return glm::vec3(p.x, p.y, p.z);
    }

    glm::mat4 getViewMatrix() const {
        glm::vec3 eye = getPosition();
        glm::vec3 center = glm::vec3(target.x, target.y, target.z);
        glm::vec3 up = glm::vec3(0, 1, 0);

        return glm::lookAt(eye, center, up);
    }

    glm::mat4 getProjectionMatrix(float fovDeg, float aspect) const {
        return glm::perspective(glm::radians(fovDeg), aspect, 1.0f, 2'000'000.0f);
    }

    void apply() const {
        Vec3 pos = computePosition();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(
            pos.x, pos.y, pos.z,
            target.x, target.y, target.z,
            0.0f, 1.0f, 0.0f
        );
    }

    void orbit(float dx, float dy) {
        yaw += dx * yawSensitivity;
        pitch -= dy * pitchSensitivity;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    void zoom(float delta) {
        if (delta > 0) distance /= zoomSensitivity;
        else distance *= zoomSensitivity;

        if (distance < 50.0f) distance = 50.0f;
        if (distance > 500000.0f) distance = 500000.0f;
    }

    void setTarget(const Vec3& t) {
        target = t;
    }
};