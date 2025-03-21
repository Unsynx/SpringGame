#pragma once

#include <raylib.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

enum SceneType {
    STANDARD,
    DELETE_ON_CLOSE,
    DATA_CONTAINER
};

class Scene {
private:
    std::string name;
    SceneType type;
public:
    Scene(std::string name, SceneType type = STANDARD) : name(std::move(name)), type(type) {}
    virtual ~Scene() = default; // Ensure proper cleanup for derived classes
    std::string getName() const { return name; }
    SceneType getType() const { return type; }
    virtual void update() {}
    virtual void draw() {}
    virtual void onClose() { 
        std::string logMessage = "Closed " + name;
        TraceLog(LOG_INFO, logMessage.c_str()); 
    }
    virtual void onOpen() {
        std::string logMessage = "Opened " + name;
        TraceLog(LOG_INFO, logMessage.c_str()); 
    }
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
        if (scenes[sceneName]->getType() == DATA_CONTAINER) return;
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

    Scene* getScene(std::string name) {
        if (sceneNotExists(name)) return nullptr;
        return scenes[name].get();
    }

    void handleSceneChanges() { 
        if (currentScene == targetScene) return;
        if (!sceneNotExists(currentScene)) {
            getCurrentScene()->onClose();
            if (getCurrentScene()->getType() == DELETE_ON_CLOSE) removeScene(currentScene);
        }
        currentScene = targetScene; 
        getCurrentScene()->onOpen();
    }
};

extern SceneManager SCENE_MANAGER;