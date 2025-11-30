#pragma once

#include "pch.h"
#include "Object.h"


void drawGridImmediate(const std::vector<float>& verts);

std::vector<float> CreateGridVertices(float size, int divisions, const std::vector<Object>& objs);

std::vector<float> UpdateGridVertices(std::vector<float> vertices, const std::vector<Object>& objs);