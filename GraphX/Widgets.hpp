//////////////////////////////////////////////////
#ifndef GRX_WIDGETS_H
#define GRX_WIDGETS_H
//////////////////////////////////////////////////
#include "Events.hpp"
#include <vector>
#include <string>
//////////////////////////////////////////////////
namespace grx
{
	static sf::Vector2f calcSize(const sf::Vector2u& current_size, const sf::Vector2u& target_size)
	{
		return sf::Vector2f(
			{ static_cast<float>(target_size.x) / static_cast<float>(current_size.x),
			 static_cast<float>(target_size.y) / static_cast<float>(current_size.y)
			});
	}

	struct AlphaMask
	{
		AlphaMask(const sf::Texture& tex);
		static AlphaMask RoundedCorners(float curvature, float size = 2048.0f);
		//AlphaMask CheckerBoard(sf::Vector2f size, float raster);
		//AlphaMask CenterHole(sf::Vector2f size, float hole_size);
		sf::Texture mask;
	};

	class Maskable
	{
	public:
		Maskable() = default;
		Maskable(const Maskable&);
		Maskable& operator=(const Maskable&);
		virtual void applyAlphaMask(const AlphaMask& mask); // should render the drawable to a rendertexture and then apply the alphamask
	protected:
		bool is_masked = false;
		std::shared_ptr<AlphaMask> mask = nullptr; // mask that was used
		std::shared_ptr<sf::Texture> drawable_masked = nullptr; // texture that holds the masked result
	};

	class Clickable : public Position2D, public Size2D, public MouseButtonEvent
	{
	public:
		Clickable(Position2D pos, Size2D size);
		bool is();
	protected:
		bool is_clicked = false;
	};

	class Dragable : public Clickable
	{

	};

	class BoxWidget : public Drawable, public Maskable, public Transformable2D
	{
	public:
		BoxWidget(float x = 0.0f,
			float y = 0.0f,
			float width = 10.0f,
			float height = 10.0f,
			sf::Color color = sf::Color::Red);

		void setOutline(float width, sf::Color color);

		void applyAlphaMask(const AlphaMask& mask) override;
		void drawtotarget(sf::RenderTarget& target, sf::BlendMode blendmode = sf::BlendAdd) const override;

	protected:
		void updateVertices() override;

		bool has_outline = false;
		float outline_width = 0.0f;
		sf::Color fill_color;
		sf::Color outline_color;
		std::array<Rectangle, 2> rectangle_array;
	};

	class TextWidget : public Drawable, public Transformable2D
	{
	public:
		TextWidget(const sf::Font& font,
			const std::string& text = "default",
			float x = 0.0f,
			float y = 0.0f,
			float width = 200.0f,
			float height = 100.0f,
			sf::Color textcolor = sf::Color::White);
	
		void drawtotarget(sf::RenderTarget& target, sf::BlendMode blendmode = sf::BlendAdd) const override;

	protected:
		void updateVertices() override;

		sf::Text sfml_text;
		sf::Font sfml_font;
	};

	class Button : public BoxWidget, public Clickable
	{
	public:
		Button(float x = 0.0f,
			float y = 0.0f,
			float width = 10.0f,
			float height = 10.0f,
			sf::Color color = sf::Color::Red);
	};

	class DragBox : public Drawable, public Dragable
	{

	};
}
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////