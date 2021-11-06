#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "vertexBuffer.h"
#include "vertexBufferLayout.h"

class VertexArray{
private:
  unsigned int m_RendererID;
public:
  VertexArray();
  ~VertexArray();

  void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
  void Bind() const;
  void UnBind() const;
};

#endif /*VERTEX_ARRAY_H*/