#include <gtest/gtest.h>
#include <Entity.h>

using namespace core;

TEST(EntityTest, get_set_mesh){
    uint32_t id = 999;
    Entity ent = Entity(id);
    EXPECT_EQ(ent.getId(),id);

    Mesh mesh = Mesh();
    math::PointArray pnts = math::PointArray();
    pnts.push_back(math::Point(1.0,0.0,0.0));
    pnts.push_back(math::Point(0.0,1.0,0.0));
    pnts.push_back(math::Point(0.0,0.0,1.0));
    mesh.setPoints(pnts, false);
    ent.setMesh(mesh);
    
    Mesh* newMesh = ent.getMesh();
    math::PointArray newPnts = newMesh->getPoints();

    //make sure the points are the same
    std::vector oldVec = pnts.toFloatVector();
    std::vector newVec = newPnts.toFloatVector();

    ASSERT_EQ(oldVec.size(), newVec.size());

    for(int i = 0; i < oldVec.size(); i++){
        EXPECT_FLOAT_EQ(oldVec.at(i), newVec.at(i));
    }
    
    EXPECT_FALSE(newMesh->is2D());
}

TEST(EntityTest, get_set_dirty){
    uint32_t id = 0;
    Entity ent = Entity(id);
    
    EXPECT_EQ(ent.getId(),id);
    
    ent.setDirty(true);
    EXPECT_TRUE(ent.isDirty());

    ent.setDirty(false);
    EXPECT_FALSE(ent.isDirty());
}

TEST(Entity_test, get_set_layer){
    uint32_t id = 1234567890;
    Entity ent = Entity(id);
    
    EXPECT_EQ(ent.getId(),id);
    
    EXPECT_EQ(ent.getLayer(), hgLayer::eWorld);

    ent.setLayer(hgLayer::eText);
    EXPECT_EQ(ent.getLayer(), hgLayer::eText);

    
    ent.setLayer(hgLayer::eUI);
    EXPECT_EQ(ent.getLayer(), hgLayer::eUI);
    
    ent.setLayer(hgLayer::eWorld);
    EXPECT_EQ(ent.getLayer(), hgLayer::eWorld);

}

TEST(Entity_test, get_set_Texture_path){
    uint32_t id = 987654321;
    Entity ent = Entity(id);
    
    EXPECT_EQ(ent.getId(),id);
    
    std::string path = "This is a test path lol";
    ent.setTexturePath(path);
    EXPECT_STREQ(path.c_str(), ent.getTexturePath().c_str());

}

/**
 * get/set texture
 * get/set position
 * get/set rotation
 * get/set scale
 * intersects entity
 * intersects vec3
 * intersects x,y,z
 */