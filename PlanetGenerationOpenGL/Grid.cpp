#include "Grid.h"

void drawGridImmediate(const std::vector<float>& verts) {
    glDisable(GL_LIGHTING);
    glLineWidth(1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 0.25f);

    glBegin(GL_LINES);
    for (size_t i = 0; i < verts.size(); i += 6) {
        glVertex3f(verts[i + 0], verts[i + 1], verts[i + 2]);
        glVertex3f(verts[i + 3], verts[i + 4], verts[i + 5]);
    }
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

std::vector<float> CreateGridVertices(float size, int divisions, const std::vector<Object>& objs) {
    std::vector<float> vertices;
    float step = size / divisions;
    float half = size / 2.0f;

    for (int yStep = 3; yStep <= 3; ++yStep) {
        float y = -half * 0.3f + yStep * step;
        for (int zStep = 0; zStep <= divisions; ++zStep) {
            float z = -half + zStep * step;
            for (int xStep = 0; xStep < divisions; ++xStep) {
                float xStart = -half + xStep * step;
                float xEnd = xStart + step;
                vertices.push_back(xStart); vertices.push_back(y); vertices.push_back(z);
                vertices.push_back(xEnd);   vertices.push_back(y); vertices.push_back(z);
            }
        }
    }
    for (int xStep = 0; xStep <= divisions; ++xStep) {
        float x = -half + xStep * step;
        for (int yStep = 3; yStep <= 3; ++yStep) {
            float y = -half * 0.3f + yStep * step;
            for (int zStep = 0; zStep < divisions; ++zStep) {
                float zStart = -half + zStep * step;
                float zEnd = zStart + step;
                vertices.push_back(x); vertices.push_back(y); vertices.push_back(zStart);
                vertices.push_back(x); vertices.push_back(y); vertices.push_back(zEnd);
            }
        }
    }
    return vertices;
}

std::vector<float> UpdateGridVertices(std::vector<float> vertices, const std::vector<Object>& objs) {
    // center of mass Y
    double totM = 0.0;
    double comY = 0.0;
    for (const auto& o : objs) {
        if (o.Initializing) continue;
        comY += o.mass * o.position.y;
        totM += o.mass;
    }
    if (totM > 0.0) comY /= totM;

    float originalMaxY = -std::numeric_limits<float>::infinity();
    for (size_t i = 0; i < vertices.size(); i += 3) originalMaxY = std::max(originalMaxY, vertices[i + 1]);

    float verticalShift = float(comY - originalMaxY);

    for (size_t i = 0; i < vertices.size(); i += 3) {
        float vx = vertices[i], vy = vertices[i + 1], vz = vertices[i + 2];
        float totalDY = 0.0f;
        for (const auto& o : objs) {
            float dx = o.position.x - vx;
            float dy = o.position.y - vy;
            float dz = o.position.z - vz;
            float dist = std::sqrt(dx * dx + dy * dy + dz * dz);
            float dist_m = dist * 1000.0f;
            float rs = (2.0f * float(G) * o.mass) / (c_light * c_light);
            float dzs = 2.0f * std::sqrt(rs * (dist_m - rs));
            totalDY += dzs * 2.0f;
        }
        vertices[i + 1] = totalDY - std::abs(verticalShift);
    }
    return vertices;
}