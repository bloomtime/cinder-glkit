#include "Vbo.h"

namespace cinder { namespace gl {


Vbo::Vbo(GLuint type)
: m_type(type)
{
}
Vbo::~Vbo()
{
}

void Vbo::set(const Attribute &attr)
{
    m_attributes[attr.getName()] = AttributeRef(new Attribute(attr));
}

void Vbo::assignLocations(GlslProg shader)
{
    for(auto pair : m_attributes){
        pair.second->setLocation(shader.getAttribLocation(pair.first));
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


Vbo::Attribute::Attribute(const string &name, int size, int location, GLenum usage)
: m_name(name)
, m_size(size)
, m_location(location)
, m_usage(usage)
{
    m_target = m_name == "index" ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
}
Vbo::Attribute::Attribute(const char *name, int size, int location, GLenum usage)
: Attribute(string(name), size, location, usage)
{
}
Vbo::Attribute::Attribute(const Attribute &attr)
: Attribute(attr.m_name, attr.m_size, attr.m_location)
{
    if(attr.m_data)
        setData(attr.m_data);
}
Vbo::Attribute::~Attribute()
{
    if(m_buffer){
        glDeleteBuffers(1, &m_buffer);
        m_buffer = 0;
    }
}

Vbo::Attribute& Vbo::Attribute::setData(const void* data, int data_length)
{
    Buffer buf(data_length);
    buf.copyFrom(data, data_length);
    return setData(buf);
}
Vbo::Attribute& Vbo::Attribute::setData(const vector<int> &data)
{
    return setData(&data[0], sizeof(int) * data.size());
}
Vbo::Attribute& Vbo::Attribute::setData(const vector<float> &data)
{
    return setData(&data[0], sizeof(float) * data.size());
}
Vbo::Attribute& Vbo::Attribute::setData(const vector<Vec2f> &data)
{
    return setData(&data[0], sizeof(float) * 2 * data.size());
}
Vbo::Attribute& Vbo::Attribute::setData(const vector<Vec3f> &data)
{
    return setData(&data[0], sizeof(float) * 3 * data.size());
}

void Vbo::Attribute::bufferData()
{
    if(!m_buffer)
        glGenBuffers(1, &m_buffer);
    glBindBuffer(m_target, m_buffer);
    glBufferData(m_target, m_data.getDataSize(), m_data.getData(), m_usage);
    m_data_dirty = false;
}

void Vbo::Attribute::bind()
{
    glBindBuffer(m_target, m_buffer);
}

void Vbo::Attribute::bindAndEnable()
{
    if(m_data_dirty)
        bufferData();
    glBindBuffer(m_target, m_buffer);
    glVertexAttribPointer(m_location, m_size, GL_FLOAT, false, 0, 0);
    glEnableVertexAttribArray(m_location);
}


Vbo Vbo::createPlane(const Vec2f &p1, const Vec2f &p2)
{
    float positions[] = { p1.x, p1.y, 0, p1.x, p2.y, 0, p2.x, p1.y, 0, p2.x, p2.y, 0 };
    float texcoords[] = { 0, 0, 0, 1, 1, 0, 1, 1 };
    
    Vbo vbo(GL_TRIANGLE_STRIP);
    vbo.set(Attribute("position", 3).setData(positions, sizeof(float) * 12));
    vbo.set(Attribute("texcoord", 2).setData(texcoords, sizeof(float) * 8));
    
    return vbo;
}


}} // namespace cinder::gl