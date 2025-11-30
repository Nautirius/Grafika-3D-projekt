#include "SkyBox.h"


GLuint loadTexture(const std::string& filename) {
    sf::Image image;
    if (!image.loadFromFile(filename)) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;
    }
    image.flipVertically();

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y,
        0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return texID;
}


void drawSkySphere(GLuint texture, const Vec3& camPos, float radius) {
    glPushMatrix();
    glTranslatef(camPos.x, camPos.y, camPos.z);

    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1, 1, 1);
    glDisable(GL_LIGHTING);

    int stacks = 20, sectors = 40;
    for (int i = 0; i < stacks; ++i) {
        float lat0 = std::numbers::pi * (-0.5 + float(i) / stacks);
        float lat1 = std::numbers::pi * (-0.5 + float(i + 1) / stacks);
        float y0 = sin(lat0) * radius;
        float y1 = sin(lat1) * radius;
        float r0 = cos(lat0) * radius;
        float r1 = cos(lat1) * radius;

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= sectors; ++j) {
            float lng = 2 * std::numbers::pi * float(j) / sectors;
            float x = cos(lng);
            float z = sin(lng);

            float s = float(j) / sectors;
            float t0 = float(i) / stacks;
            float t1 = float(i + 1) / stacks;

            glTexCoord2f(s, t0); glVertex3f(x * r0, y0, z * r0);
            glTexCoord2f(s, t1); glVertex3f(x * r1, y1, z * r1);
        }
        glEnd();
    }

    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}