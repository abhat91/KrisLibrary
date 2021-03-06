#include "GLDisplayList.h"
#include "GL.h"
#include <errors.h>
#include <stdio.h>

using namespace GLDraw;

static int gNumDisplayLists = 0;

GLDisplayList::GLDisplayList(int _count)
  :count(_count)
{}

GLDisplayList::~GLDisplayList()
{
  erase();
}


bool GLDisplayList::isCompiled() const 
{
  return id != NULL;
}

void GLDisplayList::beginCompile(int index)
{
  if(id == NULL) {
    id = new int;
    *id = glGenLists(count);
    gNumDisplayLists += count;
    if(gNumDisplayLists > 3000)
      printf("Warning, compiling new OpenGL display list id %d, total number %d\n",*id,gNumDisplayLists);
  }
  glNewList(*id+index,GL_COMPILE);
}

void GLDisplayList::endCompile()
{
  if(id == NULL) return;
  //printf("End compile,  list %d\n",*id);
  glEndList();
}

void GLDisplayList::call(int index) const
{
  if(id == NULL) return;
  //printf("Calling list %d\n",*id+index);
  glCallList(*id+index);
}

void GLDisplayList::callAll() const
{
  if(id == NULL) return;
  for(int i=0;i<count;i++)
    glCallList(*id+i);
}

void GLDisplayList::erase()
{
  if(id && id.getRefCount()==1) {
    //printf("Erasing OpenGL display list %d\n",*id);
    glDeleteLists(*id,count);
    gNumDisplayLists -= count;
  }
  //else if(id)
    //printf("Not yet erasing OpenGL display list %d has ref count %d\n",*id,id.getRefCount());

  id=NULL;
}
