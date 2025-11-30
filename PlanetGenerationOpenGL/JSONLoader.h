#pragma once

#include "pch.h"
#include "Object.h"


using json = nlohmann::json;

static Vec3 vec3_from_json(const json& j, const std::string& key, Vec3 fallback = { 0,0,0 }) {
    if (!j.contains(key)) return fallback;
    try {
        auto arr = j.at(key);
        if (arr.is_array() && arr.size() >= 3) {
            return Vec3{ arr[0].get<float>(), arr[1].get<float>(), arr[2].get<float>() };
        }
    }
    catch (...) {}
    return fallback;
}

static Vec4 vec4_from_json(const json& j, const std::string& key, Vec4 fallback = { 1,1,1,1 }) {
    if (!j.contains(key)) return fallback;
    try {
        auto arr = j.at(key);
        if (arr.is_array() && arr.size() >= 4) {
            return Vec4{ arr[0].get<float>(), arr[1].get<float>(), arr[2].get<float>(), arr[3].get<float>() };
        }
    }
    catch (...) {}
    return fallback;
}


bool LoadObjectsFromJSON(const std::string& path, std::vector<Object>& objs, int& selectedIndex, bool& pauseSim) {
    std::ifstream in(path);
    if (!in.is_open()) {
        std::cerr << "LoadObjectsFromJSON: can't open " << path << std::endl;
        return false;
    }

    json j;
    try {
        in >> j;
    }
    catch (const std::exception& e) {
        std::cerr << "LoadObjectsFromJSON: parse error: " << e.what() << std::endl;
        return false;
    }

    if (!j.is_array()) {
        std::cerr << "LoadObjectsFromJSON: top-level JSON must be an array" << std::endl;
        return false;
    }


    // Clear existing objects
    for (auto& o : objs) if (o.hasTexture && o.textureID) glDeleteTextures(1, &o.textureID);
    objs.clear();

    for (const auto& item : j) {
        Vec3 pos = vec3_from_json(item, "pos", { 0.0f, 0.0f, 0.0f });
        Vec3 vel = vec3_from_json(item, "vel", { 0.0f, 0.0f, 0.0f });
        float mass = static_cast<float>(item.at("mass").get<double>());

        float density = item.value("density", 3344.0f);
        Vec4 color = vec4_from_json(item, "color", { 1.0f,1.0f,1.0f,1.0f });
        float rotSpeed = item.value("rotSpeed", 0.0f);
        bool glow = item.value("glow", false);
        std::string tex = item.value("texture", std::string(""));
        std::string name = item.value("name", std::string("Object"));

        Object obj(pos, vel, mass, density, color, rotSpeed, glow, tex, name);

        // load texture if provided
        if (!tex.empty()) {
            obj.loadTexture(tex);
        }

        objs.emplace_back(std::move(obj));
    }

    // set selection and pause
    selectedIndex = objs.empty() ? -1 : 0;
    pauseSim = true;

    std::cout << "Loaded " << objs.size() << " objects from " << path << std::endl;
    return true;
}