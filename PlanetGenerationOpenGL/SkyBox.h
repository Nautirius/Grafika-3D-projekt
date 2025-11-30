#pragma once

#include "pch.h"


GLuint loadTexture(const std::string& filename);

void drawSkySphere(GLuint texture, const Vec3& camPos = {0, 0, 0}, float radius = 100000.0f);