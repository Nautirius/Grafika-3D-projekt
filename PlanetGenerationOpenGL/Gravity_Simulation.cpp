#include "pch.h"
#include "Object.h"
#include "Grid.h"
#include "Camera.h"
#include "SkyBox.h"
#include "JSONLoader.h"


// Simulation variables
bool running = true;
bool pauseSim = true;
bool showGrid = false;

float simSpeed = 1.0f;   // 1.0 = normal speed
float minSimSpeed = 0.01f;
float maxSimSpeed = 4.0f;


// GLU quadric
GLUquadric* gQuad = nullptr;

// Camera
Camera camera;

// Object list
std::vector<Object> objs;


void initGL(int width, int height) {
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);

    // projection (perspective)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 0.1, 750000.0);

    // create GLU quadric
    gQuad = gluNewQuadric();
    gluQuadricNormals(gQuad, GLU_SMOOTH);
    gluQuadricTexture(gQuad, GL_FALSE); // toggled per-object
}


int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 2;
    settings.minorVersion = 0;


    sf::Window window(sf::VideoMode(1280, 720), "GravityLab - Marcin Knapczyk", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW init failed\n";
        return -1;
    }

    initGL(1280, 720);

    // initial camera
    //camera.position = { 0.0f, 1000.0f, 0.0f };
    camera.yaw = -90.0f;
    camera.pitch = -10.0f;
    camera.distance = 8000.0f;

    // create initial objects
    objs.clear();

    /*objs.emplace_back(Vec3{ -5000,650,-350 }, Vec3{ 0,100,300 }, 5.97219e23f, 5515.0f, Vec4{ 1,1,1,1 }, -0.5);
    objs.emplace_back(Vec3{ 5000,650,-350 }, Vec3{ 0,0,-300 }, 5.97219e22f, 5515.0f, Vec4{ 1,1,1,1 }, -2.0);
    objs.emplace_back(Vec3{ 4850,650,-200 }, Vec3{ -100,0,-400 }, 5.97219e21f, 5515.0f, Vec4{ 1,1,1,1 }, 1.0);
    objs.emplace_back(Vec3{ 0,0,-350 }, Vec3{ 0,0,0 }, 1.989e25f, 5515.0f, Vec4{ 1.0f,1.0f,1.0f,1.0f }, -0.1, true);

    objs[0].loadTexture("textures/jupiter_texture_1.jpg");
    objs[1].loadTexture("textures/earth_texture_1.jpg");
    objs[2].loadTexture("textures/moon_texture_1.jpg");
    objs[3].loadTexture("textures/sun_texture_1.jpg");*/

    // selection
    int selectedIndex = (objs.empty() ? -1 : 0);

    LoadObjectsFromJSON("example_sims/scene.json", objs, selectedIndex, pauseSim);
    

    std::vector<float> gridVerts = CreateGridVertices(20000.0f, 25, objs);

    sf::Clock clock;

    sf::Vector2i lastMousePos = sf::Mouse::getPosition(window);

    GLuint skyTexture = loadTexture("textures/milky_way_skybox_1.png");


    while (window.isOpen() && running) {
        float dt = clock.restart().asSeconds();

        // Events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { window.close(); running = false; }
            if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(45.0, (double)event.size.width / (double)event.size.height, 0.1, 750000.0);
            }

            if (event.type == sf::Event::MouseWheelScrolled) {
                camera.zoom(event.mouseWheelScroll.delta);
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Tab) {
                    if (!objs.empty()) {
                        selectedIndex = (selectedIndex + 1) % (int)objs.size();
                        std::cout << "Selected: " << selectedIndex << "\n";
                    }
                }

                if (event.key.code == sf::Keyboard::Space) {
                    pauseSim = !pauseSim;
                    std::cout << "pause: " << pauseSim << std::endl;
                }

                if (event.key.code == sf::Keyboard::G) {
                    showGrid = !showGrid;
                    std::cout << "show grid: " << showGrid << std::endl;
                }

                if (event.key.code == sf::Keyboard::Q) {
                    running = false;
                    window.close();
                }

                if (event.key.code == sf::Keyboard::M) {
                    simSpeed *= 2.0f;
                    if (simSpeed > maxSimSpeed) simSpeed = maxSimSpeed;
                    std::cout << "Simulation Speed: " << simSpeed << "x\n";
                }

                if (event.key.code == sf::Keyboard::N) {
                    simSpeed *= 0.5f;
                    if (simSpeed < minSimSpeed) simSpeed = minSimSpeed;
                    std::cout << "Simulation Speed: " << simSpeed << "x\n";
                }

                if (event.key.code == sf::Keyboard::B) {
                    simSpeed = 1.0f;
                    std::cout << "Simulation Speed: 1x\n";
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i pos(event.mouseMove.x, event.mouseMove.y);

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    int dx = pos.x - lastMousePos.x;
                    int dy = pos.y - lastMousePos.y;
                    camera.orbit((float)dx, (float)dy);
                }

                lastMousePos = pos;
            }
        }

        // Objects update
        for (auto& obj : objs) {
            for (auto& obj2 : objs) {
                if (&obj2 != &obj && !obj.Initializing && !obj2.Initializing) {
                    float dx = obj2.position.x - obj.position.x;
                    float dy = obj2.position.y - obj.position.y;
                    float dz = obj2.position.z - obj.position.z;
                    float distance = sqrt(dx * dx + dy * dy + dz * dz);

                    if (distance > 0) {
                        std::vector<float> direction = { dx / distance, dy / distance, dz / distance };
                        distance *= 1000;
                        double Gforce = (G * obj.mass * obj2.mass) / (distance * distance);


                        float acc1 = Gforce / obj.mass;
                        std::vector<float> acc = { direction[0] * acc1, direction[1] * acc1, direction[2] * acc1 };
                        if (!pauseSim) {
                            obj.accelerate(acc[0], acc[1], acc[2], simSpeed);
                        }


                        if (obj.CheckCollision(obj2)) {
                            // Determine which object is larger
                            Object* bigger = (obj.mass >= obj2.mass) ? &obj : &obj2;
                            Object* smaller = (obj.mass >= obj2.mass) ? &obj2 : &obj;

                            // Merge masses
                            bigger->mass += smaller->mass;

                            // Momentum conservation
                            float newMass = bigger->mass + smaller->mass;

                            // momentum-weighted velocity
                            bigger->velocity.x = (bigger->velocity.x * bigger->mass + smaller->velocity.x * smaller->mass) / newMass;

                            bigger->velocity.y = (bigger->velocity.y * bigger->mass + smaller->velocity.y * smaller->mass) / newMass;

                            bigger->velocity.z = (bigger->velocity.z * bigger->mass + smaller->velocity.z * smaller->mass)/ newMass;


                            // Recompute radius from volume formula
                            bigger->radius = pow(
                                (3 * bigger->mass / bigger->density) / (4 * std::numbers::pi),
                                1.0f / 3.0f
                            ) / 1000000.0f;

                            // Mark smaller for deletion
                            smaller->toDelete = true;
                        }

                        //std::cout << "radius: " << obj.radius << std::endl;
                    }
                }
            }
            if (obj.Initializing) {
                obj.radius = pow(((3 * obj.mass / obj.density) / (4 * std::numbers::pi)), (1.0f / 3.0f)) / 1000000;
            }

            // update positions
            if (!pauseSim) {
                obj.UpdatePos(simSpeed);
                obj.rotation += obj.rotSpeed * simSpeed;
                if (std::abs(obj.rotation) > 360.0f) obj.rotation = 0.0f;
            }
        }

        objs.erase(
            std::remove_if(objs.begin(), objs.end(),
                [](const Object& o) { return o.toDelete; }),
            objs.end()
        );


        // Grid update
        gridVerts = UpdateGridVertices(gridVerts, objs);

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // camera/view
        if (selectedIndex >= 0) {
            if (selectedIndex >= objs.size()) selectedIndex = 0;
            camera.setTarget(objs[selectedIndex].position);
            camera.apply();
        }

        // skybox
        drawSkySphere(skyTexture);


        // draw grid
        if (showGrid) {
            drawGridImmediate(gridVerts);
        }

        // draw objects
        for (size_t i = 0; i < objs.size(); ++i) {
            if (objs[i].hasTexture) gluQuadricTexture(gQuad, GL_TRUE);
            else gluQuadricTexture(gQuad, GL_FALSE);

            objs[i].Draw(gQuad, true);

        }

        window.display();
    }

    // cleanup
    if (gQuad) { gluDeleteQuadric(gQuad); gQuad = nullptr; }
    // delete textures
    for (auto& o : objs) if (o.hasTexture && o.textureID) glDeleteTextures(1, &o.textureID);

    return 0;
}