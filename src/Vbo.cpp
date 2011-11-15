#include "Vbo.h"

namespace cinder { namespace gl {


Vbo::Vbo(GLuint type, GLuint usage)
: m_type(type)
, m_usage(usage)
{
}

Vbo::~Vbo()
{
}

void Vbo::set(const Attribute &attr)
{
    m_attributes[attr.getName()] = AttributeRef(new Attribute(attr));
}

void Vbo::update()
{
    for(auto pair : m_attributes){
        AttributeRef attr = pair.second;
        attr->setTarget(pair.first == "index" ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER);
        attr->bufferData(m_usage);
    }
}

void Vbo::draw()
{
    int length = numeric_limits<int>::max();
    for(auto pair : m_attributes){
        AttributeRef attr = pair.second;
        if(attr->getTarget() == GL_ARRAY_BUFFER && attr->getLocation() >= 0){
            attr->bindAndEnable();
            length = min(length, attr->getLength());
        }
    }
    if(m_attributes.count("index")){
        AttributeRef index = m_attributes["index"];
        index->bind();
        glDrawElements(m_type, index->getData().getDataSize() / 2, GL_UNSIGNED_SHORT, 0); // TODO(ryan): This assumes the indices are 16bit ints
    }
    else{
        glDrawArrays(m_type, 0, length);
    }
}


Vbo::Attribute::Attribute(const string &name, int size, Buffer data, int location)
: m_name(name)
, m_size(size)
, m_data(data)
, m_location(location)
{
}
Vbo::Attribute::Attribute(const char *name, int size, Buffer data, int location)
: Attribute(string(name), size, data, location)
{
}
Vbo::Attribute::Attribute(const Attribute &attr)
: Attribute(attr.m_name, attr.m_size, attr.m_data, attr.m_location)
{
}
Vbo::Attribute::~Attribute()
{
    if(m_buffer){
        glDeleteBuffers(1, &m_buffer);
        m_buffer = 0;
    }
}

void Vbo::Attribute::bufferData(GLenum usage)
{
    if(!m_buffer)
        glGenBuffers(1, &m_buffer);
    glBindBuffer(m_target, m_buffer);
    glBufferData(m_target, m_data.getDataSize(), m_data.getData(), usage);
}

void Vbo::Attribute::bind()
{
    glBindBuffer(m_target, m_buffer);
}

void Vbo::Attribute::bindAndEnable()
{
    glBindBuffer(m_target, m_buffer);
    glVertexAttribPointer(m_location, m_size, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(m_location);
}


Vbo Vbo::createPlane(const Vec2f &p1, const Vec2f &p2)
{
    float positions[] = { p1.x, p1.y, 0, p1.x, p1.y, 0, p2.x, p1.y, 0, p2.x, p2.y, 0 };
    float texcoords[] = { 0, 0, 0, 1, 1, 0, 1, 1 };
    
    Vbo vbo;
    vbo.set(Attribute("position", 3, Buffer(positions, sizeof(float) * 12, true)));
    vbo.set(Attribute("texcoord", 2, Buffer(texcoords, sizeof(float) * 8, true)));
    
    return vbo;
}


}} // namespace cinder::gl