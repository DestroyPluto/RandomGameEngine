#include "Scene.h"
#include "Mouse.h"
#include "Config.h"
#include "PNGLoader.h"

using namespace core;
using namespace io;

static const std::string CONFIG_DEFAULT_TEXTURE = "ConfigDefaultTexture";

Scene::Scene(std::vector<Entity*> entities, RenderingPlugin* engine){
    m_entities = entities;
    loadTextures(engine);

}

/*Scene::Scene(){
    //do nothing lol
    HgLogger::logError("something messed up has happened with the scene lol");
}
*/
void Scene::loadTextures(RenderingPlugin* engine){
    //TODO handle textures better
    PNGLoader loader = PNGLoader();

    //load the initial textures, or just the default one if it isn't set
    for(Entity* e : m_entities){
       if(e->getLayer() != eWorld){
           continue;
       }
       if (e->getTexturePath().empty()) {
           continue;
       }

       std::string path = e->getTexturePath().empty() ? engine->getConfig()->getOption(CONFIG_DEFAULT_TEXTURE, "redChecker.png") : e->getTexturePath();
       HgTexture* tex =  loader.loadFromFile(path.c_str());
       e->setTexture(tex);
       engine->addTexture(tex);
   }
}

void Scene::updateEntity(Entity* ent, double mouseX, double mouseY){
    ent->onUpdate();

    //probably a way to do this with recursion...
    for (Entity* children : ent->getChildren()) {
        updateEntity(children, mouseX, mouseY);
    }
    
    if (ent->shouldDestroy()) {
        m_destroyedEntities.push_back(ent->getId());
        ent->getParent()->removeChild(ent);
        delete ent;
        return;
    }
    
    //don't want to check for collisions, or send it to the engine if there isn't a mesh.
    if (!ent->getMesh()) {
        return;
    }

    if (ent->isDirty()) {
        m_dirtyEnts.push_back(ent);
    }

    if (ent->intersects(mouseX, mouseY, 0)) {
        ent->onCollision();
    }

}

void Scene::update(RenderingPlugin* engine){
    m_dirtyEnts.clear();
    m_destroyedEntities.clear();

    double mX, mY;
    Mouse::getInstance()->getScreenPos(mX, mY);

    for(Entity* e : m_entities){
        updateEntity(e, mX, mY);
    }

    if(m_dirtyEnts.size() > 0){
        engine->setDirtyEntities(m_dirtyEnts);
    }

    if (m_destroyedEntities.size() > 0) {
        engine->destroyEntities(m_destroyedEntities);
    }
}