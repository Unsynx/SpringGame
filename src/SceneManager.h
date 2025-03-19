#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

class Scene {
private:
    std::string name;
public:
    Scene(std::string name) : name(std::move(name)) {}
    virtual ~Scene() = default; // Ensure proper cleanup for derived classes
    std::string getName() const { return name; }
    virtual void update() {}
    virtual void draw() {}
};

class SceneManager {
private:
    std::string currentScene;
    std::string targetScene;
    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;

private:
    bool sceneNotExists(const std::string& sceneName) const {
        return scenes.find(sceneName) == scenes.end();
    }

public:
    SceneManager() = default;

    void addScene(std::unique_ptr<Scene> scene) {
        std::string name = scene->getName();
        scenes[name] = std::move(scene);
    }
    
    void setActiveScene(const std::string& sceneName) {
        if (sceneNotExists(sceneName)) return;
        targetScene = sceneName;
    }

    void removeScene(const std::string& sceneName) {
        if (sceneNotExists(sceneName)) return;
        if (currentScene == sceneName) {
            currentScene.clear();  // Alternatively, switch to a default scene
        }
        scenes.erase(sceneName);
    }

    void update() {
        if (sceneNotExists(currentScene)) return;
        scenes[currentScene]->update();
    }

    void draw() {
        if (sceneNotExists(currentScene)) return;
        scenes[currentScene]->draw();
    }

    Scene* getCurrentScene() {
        if (sceneNotExists(currentScene)) return nullptr;
        return scenes[currentScene].get();
    }

    void handleSceneChanges() { currentScene = targetScene; }
};

extern SceneManager SCENE_MANAGER;