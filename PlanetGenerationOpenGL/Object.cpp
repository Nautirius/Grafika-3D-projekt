#include "Object.h"


Object::Object(Vec3 p, Vec3 v, float m, float _density, Vec4 col, float _rotSpeed, bool Glow, const std::string& tex, std::string _name) {
    position = p; velocity = v; mass = m; density = _density; color = col; rotSpeed = _rotSpeed; glow = Glow;
    texturePath = tex; name = _name;
    radius = std::pow((3.0f * mass / density) / (4.0f * std::numbers::pi), 1.0f / 3.0f) / sizeRatio;
    if (!texturePath.empty()) loadTexture(texturePath);
}

void Object::loadTexture(const std::string& path) {
    sf::Image img;
    if (!img.loadFromFile(path)) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return;
    }
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    hasTexture = true;
}

void Object::UpdatePos(float simSpeed) {
    float dt = (1.0f / 94.0f) * simSpeed;

    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
    position.z += velocity.z * dt;

    radius = std::pow((3.0f * mass / density) / (4.0f * std::numbers::pi), 1.0f / 3.0f) / sizeRatio;
    //std::cout << "Updated position: x = " << position.x << " y = " << position.y << " z = " << position.z << " mass: " << mass << std::endl;
}

void Object::accelerate(float x, float y, float z, float simSpeed) {

    float dt = (1.0f / 96.0f) * simSpeed;

    velocity.x += x * dt;
    velocity.y += y * dt;
    velocity.z += z * dt;
}

bool Object::CheckCollision(const Object& other) const {
    float dx = other.position.x - position.x;
    float dy = other.position.y - position.y;
    float dz = other.position.z - position.z;
    float dist = std::sqrt(dx * dx + dy * dy + dz * dz);

    return dist < (radius + other.radius);
}

void Object::Draw(GLUquadric* quad, bool texturedAllowed) const {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);

    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

    glRotatef(rotation, 0.0f, 0.0f, 1.0f);


    // Texture handling
    if (hasTexture && texturedAllowed) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        gluQuadricTexture(quad, GL_TRUE);

        // White material for proper texture display
        GLfloat white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
        GLfloat spec[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0f);
    }
    else {
        glDisable(GL_TEXTURE_2D);
        gluQuadricTexture(quad, GL_FALSE);

        // Use object color for non-textured spheres
        GLfloat amb[4] = { color.r * 0.2f, color.g * 0.2f, color.b * 0.2f, color.a };
        GLfloat diff[4] = { color.r, color.g, color.b, color.a };
        GLfloat spec[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0f);
    }

    // Glow effect for stars
    if (glow) {
        // Enable a light source
        GLfloat lightColor[4] = { color.r, color.g, color.b, 1.0f };
        GLfloat lightPos[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // relative to object position
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

        // glow sphere for visual effect
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glDisable(GL_LIGHTING);
        glColor4f(color.r, color.g, color.b, 1.0f);
        float glowRadius = radius * 1.05f;
        gluSphere(quad, glowRadius, 24, 16);
        glEnable(GL_LIGHTING);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_BLEND);

        glDisable(GL_TEXTURE_2D);
        gluQuadricTexture(quad, GL_FALSE);
    }

    // Draw the main sphere
    if (!glow) gluSphere(quad, radius, 32, 24);

    // Cleanup
    if (hasTexture && texturedAllowed) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();
}