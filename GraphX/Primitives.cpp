//////////////////////////////////////////////////
#include "Primitives.hpp"
//////////////////////////////////////////////////

grx::Transformable2D::Transformable2D(float x,
    float y,
    float width,
    float height,
    float angle)
    : pos{x, y}, size{width, height}, rotation{angle}
{
    updateVertices();
}

grx::Position2D grx::Transformable2D::getPosition() const
{
    return pos;
}
grx::Size2D grx::Transformable2D::getSize() const
{
    return size;
}
grx::Rotation2D grx::Transformable2D::getRotation() const
{
    return rotation;
}

void grx::Transformable2D::setPosition(Position2D pos)
{
    this->pos = pos;
    updateVertices();
}
void grx::Transformable2D::setSize(Size2D size)
{
    this->size = size;
    updateVertices();
}
void grx::Transformable2D::setRotation(Rotation2D rotation)
{
    this->rotation = rotation;
    updateVertices();
}

void grx::Transformable2D::transformPosition(Position2D pos)
{
    this->pos.x += pos.x;
    this->pos.y += pos.y;
    updateVertices();
}
void grx::Transformable2D::transformSize(Size2D size)
{
    this->size.width += size.width;
    this->size.height += size.height;
    updateVertices();
}
void grx::Transformable2D::transformRotation(Rotation2D rotation)
{
    this->rotation.angle += rotation.angle;
    updateVertices();
}
void grx::Transformable2D::updateVertices()
{

}

sf::Color grx::Colorable::getColor()
{
    return color;
}

void grx::Colorable::setColor(sf::Color color)
{
    this->color = color;
    updateVertices();
}
void grx::Colorable::updateVertices()
{

}

void grx::Drawable::drawtotarget(sf::RenderTarget& target, sf::BlendMode blendmode) const
{

}

void grx::Triangle::drawtotarget(sf::RenderTarget& target, sf::BlendMode blendmode) const
{
    target.draw(vertices, blendmode);
}

grx::Rectangle::Rectangle(float x,
    float y,
    float width,
    float height,
    sf::Color color)
{
    setPosition({ x,y });
    setSize({ width, height });
    setColor(color);
}

void grx::Rectangle::drawtotarget(sf::RenderTarget& target, sf::BlendMode blendmode) const
{
    triangle_array[0].drawtotarget(target, blendmode);
    triangle_array[1].drawtotarget(target, blendmode);
}

void grx::Rectangle::updateVertices()
{
    triangle_array[0].vertices[0] = { .position = {pos.x,pos.y}, .color = color };
    triangle_array[0].vertices[1] = { .position = {pos.x + size.width,pos.y}, .color = color };
    triangle_array[0].vertices[2] = { .position = {pos.x + size.width,pos.y + size.height}, .color = color };
   
    triangle_array[1].vertices[0] = { .position = {pos.x,pos.y}, .color = color };
    triangle_array[1].vertices[1] = { .position = {pos.x,pos.y + size.height}, .color = color };
    triangle_array[1].vertices[2] = { .position = {pos.x + size.width,pos.y + size.height}, .color = color };
}

