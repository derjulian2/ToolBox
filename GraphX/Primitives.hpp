//////////////////////////////////////////////////
#ifndef GRX_PRIMITIVES_H
#define GRX_PRIMITIVES_H
//////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
//////////////////////////////////////////////////
#include <array>
#include <memory>
//////////////////////////////////////////////////
namespace grx
{
	struct Position2D
	{
		float x;
		float y;
	};

	struct Size2D
	{
		float width;
		float height;
	};

	struct Rotation2D
	{
		float angle;
	};

	class Transformable2D
	{
	public:
		Transformable2D(float x = 0.0f,
			float y = 0.0f,
			float width = 10.0f,
			float height = 10.0f,
			float angle = 0.0f);

		Position2D getPosition() const;
		Size2D getSize() const;
		Rotation2D getRotation() const;

		virtual void setPosition(Position2D pos);
		virtual void setSize(Size2D size);
		virtual void setRotation(Rotation2D rotation);

		virtual void transformPosition(Position2D pos);
		virtual void transformSize(Size2D size);
		virtual void transformRotation(Rotation2D rotation);
	protected:
		virtual void updateVertices();
		Position2D pos;
		Size2D size;
		Rotation2D rotation;
	};

	class Colorable
	{
	public:
		sf::Color getColor();

		void setColor(sf::Color color);
	protected:
		virtual void updateVertices();
		sf::Color color;
	};



	class Drawable
	{
	public:
		Drawable() = default;
		virtual ~Drawable() = default;
		// copyable
		Drawable(const Drawable&) = default;
		Drawable& operator=(const Drawable&) = default;
		// non-movable as window holds list of memory-locations to drawables
		// if these were to change via moving, it would likely result in undefined behaviour
		// alternative: notify the list in the scene/window the pointer is in that
		// the location has changed -> probably more expensive than it would actually be advantageous
		Drawable(const Drawable&&) = delete;
		const Drawable& operator=(const Drawable&&) = delete;
		// probably need to add destructor/undraw function that erases that pointer from the list that it
		// is saved in to avoid undefined behaviour
		virtual void drawtotarget(sf::RenderTarget& target, sf::BlendMode blendmode = sf::BlendAdd) const;
		//virtual void undisplay() const; // removes pointer from the scene that holds it
		bool SHOW = true;
	};
	
	class Triangle : public Drawable
	{
	public:
		sf::VertexArray vertices = sf::VertexArray(sf::PrimitiveType::Triangles, 3);
		
		Triangle() = default;

		Triangle(const Triangle&&) = delete;
		const Triangle& operator=(const Triangle&&) = delete;

		void drawtotarget(sf::RenderTarget& target, sf::BlendMode blendmode = sf::BlendAdd) const override;
	};


	class Rectangle : public Drawable, public Transformable2D, public Colorable
	{
	public:
		Rectangle(float x = 0.0f,
			float y = 0.0f,
			float width = 10.0f,
			float height = 10.0f,
			sf::Color color = sf::Color::Red);

		Rectangle(const Rectangle&&) = delete;
		const Rectangle& operator=(const Rectangle&&) = delete;

		void drawtotarget(sf::RenderTarget& target, sf::BlendMode blendmode = sf::BlendAdd) const override;

	private:
		void updateVertices() override;

		std::array<Triangle, 2> triangle_array = {};
	};
}
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////