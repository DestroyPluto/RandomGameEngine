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

void Scene::update(RenderingPlugin* engine){
    m_dirtyEnts.clear();
    double mX, mY;
    Mouse::getInstance()->getScreenPos(mX, mY);

    for(Entity* e : m_entities){
        e->onUpdate();
        
        if(e->isDirty()){
            m_dirtyEnts.push_back(e);
        }
        
        if(e->intersects(mX,mY,0)){
            e->onCollision();
        }

        //probably a way to do this with recursion...
        for (Entity* children : e->getChildren()) {
            children->onUpdate();
            if (children->isDirty()) {
                m_dirtyEnts.push_back(children);
            }
            if (children->intersects(mX, mY, 0)){
                children->onCollision();
            }
        }
    }
    
    

    if(m_dirtyEnts.size() > 0){
        engine->setDirtyEntities(m_dirtyEnts);
    }
}