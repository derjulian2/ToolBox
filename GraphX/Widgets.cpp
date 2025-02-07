//////////////////////////////////////////////////
#include "Widgets.hpp"
//////////////////////////////////////////////////
grx::AlphaMask::AlphaMask(const sf::Texture& tex)
    : mask(tex)
{

}

grx::AlphaMask grx::AlphaMask::RoundedCorners(float curvature, float size)
{
    if (curvature < 0 || curvature > 1)
        throw std::invalid_argument("curvature can only vary from 0 to 1");

    sf::RenderTexture tex({
        static_cast<uint32_t>(size),
        static_cast<uint32_t>(size) });
    float radius = curvature * (0.5f * size);

    // put rounded corners to texture
    sf::CircleShape circle(radius, 1024);
    circle.setFillColor(sf::Color::Magenta);
    tex.draw(circle);
    circle.setPosition({ 0.0f, size - 2 * radius });
    tex.draw(circle);
    circle.setPosition({ size - 2 * radius, 0.0f });
    tex.draw(circle);
    circle.setPosition({ size - 2 * radius, size - 2 * radius });
    tex.draw(circle);

    // put rectangle body to texture
    sf::RectangleShape rect;
    rect.setFillColor(sf::Color::Magenta);
    rect.setPosition({ radius, 0.0f });
    rect.setSize({ size - 2 * radius, size });
    tex.draw(rect);
    rect.setPosition({ 0.0f, radius });
    rect.setSize({ size, size - 2 * radius });
    tex.draw(rect);

    tex.display();

    return AlphaMask({ tex.getTexture() });
}

grx::Maskable::Maskable(const Maskable&)
{
    this->drawable_masked = nullptr;
    this->mask = nullptr;
}
grx::Maskable& grx::Maskable::operator=(const Maskable&)
{
    this->drawable_masked = nullptr;
    this->mask = nullptr;
    return *this;
}

void grx::Maskable::applyAlphaMask(const AlphaMask& mask)
{

}

grx::Clickable::Clickable(Position2D pos, Size2D size)
    : Position2D(pos), Size2D(size), MouseButtonEvent([&](Position2D mpos)
        {
            if (mpos.x > x && mpos.x < x + width &&
                mpos.y > y && mpos.y < y + height)
                is_clicked = true;
            else
                is_clicked = false;
        })
{

}

bool grx::Clickable::is()
{
    if (is_clicked)
    {
        is_clicked = false;
        return true;
    }
    else
        return false;
}

grx::BoxWidget::BoxWidget(float x,
    float y,
    float width,
    float height,
    sf::Color color)
{
    fill_color = color;
    outline_color = color;
    setPosition({ x, y });
    setSize({ width, height });
}

void grx::BoxWidget::setOutline(float width, sf::Color color)
{
    has_outline = true;
    outline_width = width;
    outline_color = color;
    updateVertices();
}

void grx::BoxWidget::applyAlphaMask(const AlphaMask& mask)
{
    this->mask = std::make_shared<AlphaMask>(mask);

    sf::Vector2u size =
    {
        static_cast<uint32_t>(rectangle_array[0].getSize().width),
        static_cast<uint32_t>(rectangle_array[0].getSize().height)
    };

    sf::Sprite mask_sprite(this->mask->mask);

    this->is_masked = false;
    Position2D temp = getPosition();
    setPosition({ outline_width, outline_width });

    sf::RenderTexture rendertex_drawable(size);
    drawtotarget(rendertex_drawable);
    rendertex_drawable.display();

    sf::Sprite drawable_sprite(rendertex_drawable.getTexture());
    drawable_sprite.setScale(calcSize(size, this->mask->mask.getSize()));


    sf::RenderTexture result(this->mask->mask.getSize());
    result.draw(mask_sprite);
    result.draw(drawable_sprite, sf::BlendMode(sf::BlendMode::Factor::DstAlpha, sf::BlendMode::Factor::Zero));
    result.display();

    setPosition(temp);

    this->drawable_masked = std::make_shared<sf::Texture>(result.getTexture());
    this->is_masked = true;
}

void grx::BoxWidget::drawtotarget(sf::RenderTarget& target, sf::BlendMode blendmode) const
{
	if (is_masked)
	{
        if (drawable_masked != nullptr)
        {
            sf::Sprite sprite(*drawable_masked);
            sprite.setScale(calcSize(sprite.getTexture().getSize(), { static_cast<uint32_t>(size.width), static_cast<uint32_t>(size.height) }));
            sprite.setPosition({ rectangle_array[0].getPosition().x, rectangle_array[0].getPosition().y });
            target.draw(sprite, blendmode);
        }
	}
	else
	{
        if (has_outline)
		    rectangle_array[0].drawtotarget(target, blendmode);
		rectangle_array[1].drawtotarget(target, blendmode);
	}
}

void grx::BoxWidget::updateVertices()
{
    rectangle_array[0].setPosition({ pos.x - outline_width, pos.y - outline_width });
    rectangle_array[0].setSize({ size.width + 2 * outline_width, size.height + 2 * outline_width });
    rectangle_array[0].setColor(outline_color);

    rectangle_array[1].setPosition(pos);
    rectangle_array[1].setSize(size);
    rectangle_array[1].setColor(fill_color);
}

grx::TextWidget::TextWidget(const sf::Font& font,
    const std::string& text,
    float x,
    float y,
    float width,
    float height,
    sf::Color textcolor)
    : sfml_text(font, text), sfml_font(font)
{
    setPosition({ x, y });
    setSize({ width, height });
}

void grx::TextWidget::drawtotarget(sf::RenderTarget& target, sf::BlendMode blendmode) const
{
    sf::RenderTexture tex({ static_cast<uint32_t>(size.width), static_cast<uint32_t>(size.height) });
    tex.draw(sfml_text);
    tex.display();

    sf::Sprite text_sprite(tex.getTexture());
    text_sprite.setPosition({ pos.x, pos.y });

    target.draw(text_sprite);
}

void grx::TextWidget::updateVertices()
{

}

grx::Button::Button(float x,
    float y,
    float width,
    float height,
    sf::Color color)
    : BoxWidget(x,y,width, height, color), Clickable({x, y}, {width, height})
{

}