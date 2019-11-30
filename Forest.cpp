#include "Forest.h"
#include "GLDrawable.h"
#include "Tree.h"

Forest::Forest(const int32_t x, const int32_t y, const int32_t tree_size)
    : GLDrawable{}
    , size_x{x}
    , size_y{y}
    , tree{tree_size}
{

}

bool Forest::Initialize(){
    initializeOpenGLFunctions();
    tree.Initialize();

    return true;
}

void Forest::Draw(){
    glPushMatrix();
    glTranslatef(_pos.x(), _pos.y(), _pos.z());
    for( int i = -size_x; i < size_x - 8 ; i+=8){
        glTranslatef(8,0,0);
        glPushMatrix();
        for( int j = -size_y; j < size_y; j +=8){
            glTranslatef(0,8,0);
            tree.Draw();
        }
        glPopMatrix();
    }
    glPopMatrix();
}
