#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H

#include "vertexBuffer.h"
#include "renderer.h"
#include <vector>
#include <GL/gl.h>

struct VertexBufferElement{
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  static unsigned int GetSizeOfType(unsigned int type){
    switch (type){
      case GL_FLOAT: return sizeof(float);
        break;
      case GL_UNSIGNED_INT: return sizeof(unsigned int);
        break;
      case GL_UNSIGNED_BYTE: return sizeof(char);
        break;
      default:
        break;
    }

    ASSERT(false);
    return 0;
  }
};


class VertexBufferLayout{
private:
  std::vector<VertexBufferElement> m_Elements;
  unsigned int m_Stride;

public:
  VertexBufferLayout() : m_Stride{0} {};

  template<typename T>
  void Push(unsigned int count){
  }

  inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
  inline unsigned int GetStride() const { return m_Stride; }
};

template<> inline void VertexBufferLayout::Push<float>(unsigned int count){
  m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
  m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template<> inline void VertexBufferLayout::Push<unsigned int>(unsigned int count){
  m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
  m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<> inline void VertexBufferLayout::Push<unsigned char>(unsigned int count){
  m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
  m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}

#endif /*VERTEX_BUFFER_LAYOUT_H*/