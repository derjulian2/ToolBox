//////////////////////////////////////////////////
/// THULIUM LIBRARY GUI WIDGET SOURCE FILE     ///
/// 2024 Julian Benzel						   ///
//////////////////////////////////////////////////
#if __has_include("ThuliumWidgets.hpp")
#include "ThuliumWidgets.hpp"
#endif
#if __has_include("ThuliumWindow.hpp")
#include "ThuliumWindow.hpp"
#endif
#if __has_include("ThuliumEvents.hpp")
#include "ThuliumEvents.hpp"
#endif
//////////////////////////////////////////////////
#if defined(THL_WIDGETS_H) && defined(THL_WINDOW_H)
//////////////////////////////////////////////////
thl::Object::Object() {}
thl::Object::Object(vec2f pos, vec2f dim) 
	: position(pos), dimensions(dim) {}
void thl::Object::SetPosition(vec2f pos)
{
	position = pos;
}
void thl::Object::SetDimensions(vec2f dim)
{
	dimensions = dim;
}
void thl::Object::SetScreenPositions(mat3f projection_matrix)
{
	position_screen = thl::NDCtoScreenPosition(position, projection_matrix);
	dimensions_screen = 
		thl::NDCtoScreenPosition(position + dimensions, projection_matrix) - 
		thl::NDCtoScreenPosition(position - dimensions, projection_matrix);
}
//////////////////////////////////////////////////
thl::MaterialObject::MaterialObject() {}
thl::MaterialObject::MaterialObject(vec4f col, vec4f col_o, uint16_t w, Texture* tex) 
	: color_body(col), color_outline(col_o), outline_width(w), texture(tex) {}
void thl::MaterialObject::SetTexture(thl::Texture& texture)
{
	this->texture = &texture;
}
void thl::MaterialObject::SetColor(vec4f color)
{
	color_body = color;
}
void thl::MaterialObject::SetOutline(uint16_t width, vec4f color)
{
	color_outline = color;
	outline_width = width;
}
void thl::MaterialObject::SetMaterials(vec2f screen_pos, vec2f screen_dim)
{
	sfml_shape.setOrigin(sf::Vector2f(0.5f * screen_dim.x, 0.5f * screen_dim.y));
	sfml_shape.setSize(sf::Vector2f(screen_dim.x, screen_dim.y));
	sfml_shape.setPosition(sf::Vector2f(screen_pos.x, screen_pos.y));

	sfml_shape.setOutlineColor(assign(color_outline));
	sfml_shape.setOutlineThickness(outline_width);
	sfml_shape.setFillColor(assign(color_body));

	if (texture != nullptr)
	{
		sfml_sprite.setTexture(*texture);
		sfml_sprite.setOrigin(sf::Vector2f(0.5f * screen_dim.x, 0.5f * screen_dim.y));
		sfml_sprite.setPosition(sf::Vector2f(screen_pos.x, screen_pos.y));
	}
}
//////////////////////////////////////////////////
thl::TextObject::TextObject() {}
thl::TextObject::TextObject(std::string text, vec4f col, uint16_t size, Font* font)
	: text_str(text), text_color(col), text_size(size), font(font) {}
void thl::TextObject::SetTextColor(vec4f color)
{
	text_color = color;
}
void thl::TextObject::SetTextSize(uint16_t size)
{
	text_size = size;
}
void thl::TextObject::SetText(vec2f screen_pos, vec2f screen_dim)
{
	if (font == nullptr)
		return;

	sfml_text.setFont(*font);
	sfml_text.setString(text_str);
	sfml_text.setFillColor(assign(text_color));
	sfml_text.setScale(text_size, text_size);

	sf::FloatRect lbounds = sfml_text.getLocalBounds();

	sfml_text.setOrigin(sf::Vector2f(0.5f * lbounds.width + lbounds.left, 0.5f * lbounds.height + lbounds.top));
	sfml_text.setPosition(sf::Vector2f(screen_pos.x, screen_pos.y));

	//float ratio = text_size * std::min(dimensions_projected.x / lbounds.width, dimensions_projected.y / lbounds.height);
	//text.setScale(ratio, ratio);

	//text.setScale(text_size, text_size);
}
//////////////////////////////////////////////////
thl::StaticBox::StaticBox(Scene& scene, vec2f position, vec2f dimensions)
	: Object(position, dimensions), MaterialObject()
{
	scene.staticbox_stack.emplace_back(this);
}
void thl::StaticBox::Refresh(mat3f projection_matrix)
{
	SetScreenPositions(projection_matrix);
	SetMaterials(position_screen, dimensions_screen);
}
//////////////////////////////////////////////////
thl::StaticText::StaticText(Scene& scene, vec2f position, vec2f dimensions, std::string text, thl::Font& font)
	: Object(position, dimensions), TextObject(text, vec4f(255), 1, &font)
{
	scene.statictext_stack.emplace_back(this);
}
void thl::StaticText::Refresh(mat3f projection_matrix)
{
	SetScreenPositions(projection_matrix);
	SetText(position_screen, dimensions_screen);
}
////////////////////////////////////////////////////
#ifdef THL_EVENTS_H
thl::Button::Button(Scene& scene, vec2f position, vec2f dimensions, std::string text, thl::Font& font)
	: Object(position, dimensions), MaterialObject(), TextObject(text, vec4f(255), 1, &font)
{
	hover_event = std::make_shared<MouseEvent>(scene, [](float, float){});
	click_event = std::make_shared<MousebuttonEvent>(scene, [](){}, sf::Mouse::Button::Left);
	scene.button_stack.emplace_back(this);
	scene.mouseevent_stack.emplace_back(hover_event.get());
	scene.mousebuttonevent_stack.emplace_back(click_event.get());
}
thl::Button::~Button()
{

}
void thl::Button::Refresh(mat3f projection_matrix)
{
	SetScreenPositions(projection_matrix);
	SetMaterials(position_screen, dimensions_screen);
	SetText(position_screen, dimensions_screen);
}
void thl::Button::Bind(Scene& scene, std::function<void()> hover, std::function<void()> click, std::function<void()> idle)
{
	hover_event->SetFunction(
		[this, hover, idle](float x, float y) mutable
		{
			sf::FloatRect fr_mouse = { x,y, 0.1f, 0.1f };
			sf::FloatRect fr_button = {
				position_screen.x - 0.5f * dimensions_screen.x,
				position_screen.y - 0.5f * dimensions_screen.y,
				dimensions_screen.x,
				dimensions_screen.y };

			if (fr_button.intersects(fr_mouse))
				__is_hovering = true;
			else
				__is_hovering = false;

			if (__is_hovering && !__is_clicked)
				hover();
			else if (!__is_hovering && !__is_clicked)
				idle();

		});
	click_event->SetActiveFunction(
		[this, click]()
		{
			if (__is_hovering)
			{
				__is_clicked = true;
				click();
			}
		});
	click_event->SetInactiveFunction(
		[this, hover, idle]()
		{
			if (__is_clicked && __is_hovering)
			{
				__is_clicked = false;
				hover();
			}
			else if (__is_clicked && !__is_hovering)
			{
				__is_clicked = false;
				idle();
			}
		});
}
#endif
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////
