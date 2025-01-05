//////////////////////////////////////////////////
/// THULIUM LIBRARY GUI WIDGET HEADER FILE     ///
/// 2024 Julian Benzel						   ///
//////////////////////////////////////////////////
#ifndef THL_WIDGETS_H
#define THL_WIDGETS_H
//////////////////////////////////////////////////
#if __has_include("LinAlgpp.hpp")
#include "LinAlgpp.hpp"
#endif
//////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <memory>
//////////////////////////////////////////////////
namespace thl
{
	class Texture;
	class Font;

	class Object
	{
	public:
		Object();
		Object(vec2f pos, vec2f dim);

		void SetPosition(vec2f pos);
		void SetDimensions(vec2f dim);

	protected:
		void SetScreenPositions(mat3f projection_matrix);

		vec2f position;
		vec2f dimensions;

		vec2f position_screen;
		vec2f dimensions_screen;
	};

	class MaterialObject
	{
	public:
		MaterialObject();
		MaterialObject(vec4f col, vec4f col_o, uint16_t w, Texture* tex);

		void SetTexture(thl::Texture& texture);
		void SetColor(vec4f color);
		void SetOutline(uint16_t width, vec4f color);

	protected:
		void SetMaterials(vec2f screen_pos, vec2f screen_dim);

		vec4f color_body = vec4f(255);
		vec4f color_outline;
		uint16_t outline_width = 0;
		Texture* texture = nullptr;

		sf::RectangleShape sfml_shape;
		sf::Sprite sfml_sprite;
	};

	class TextObject
	{
	public:
		TextObject();
		TextObject(std::string text, vec4f col, uint16_t size, Font* font);

		void SetTextColor(vec4f color);
		void SetTextSize(uint16_t size);
	protected:
		void SetText(vec2f screen_pos, vec2f screen_dim);

		std::string text_str = "";
		vec4f text_color = vec4f(255);
		uint16_t text_size = 1;
		Font* font = nullptr;

		sf::Text sfml_text;
	};

	class Scene;
	/*
	* \brief basic rectangle widget to be placed in your scene
	* \brief can have an outline and display textures
	* \param - scene : the scene in which the widget should be created
	* \param - position : 2D NDC position relative to the window e.g. (0,0) would be center, (1,1) would be topright
	* \param - dimensions : 2D dimensions relative to the window e.g. (0.5,0.5) would cover half of the window, (1,1) would fill the whole screen
	* \param - color_body, color_outline : RGBA values of the background/outline color
	* \param - outline_width : pixel width of the outline
	*/
	class StaticBox : public Object, public MaterialObject
	{
	public:
		StaticBox(Scene& scene, vec2f position, vec2f dimensions);

	private:
		friend Scene;

		void Refresh(mat3f projection_matrix);
	};

	//////////////////////////////////////////////////
	/*
	* \brief widget to display text in your scene.
	* \param - scene: 
	* \param - position:
	* \param - dimensions:
	* \param - text:
	* \param - text_color
	* \param - font:
	*/
	class StaticText : public Object, public TextObject
	{
	public:
		StaticText(Scene& scene, vec2f position, vec2f dimensions, std::string text, thl::Font& font);

	private:
		friend Scene;

		void Refresh(mat3f projection_matrix);
	};

	////////////////////////////////////////////////////
	#if __has_include("ThuliumEvents.hpp")
	#define THL_WIDGETS_HAS_EVENTS
	#endif

	#ifdef THL_WIDGETS_HAS_EVENTS
	class MouseEvent;
	class MousebuttonEvent;

	class Button : public Object, public MaterialObject, public TextObject
	{
	public:
		Button(Scene& scene, vec2f position, vec2f dimensions, std::string text, thl::Font& font);
		~Button();
		// binds functions to the buttons events. should be created in the same scene as the button was created in.
		void Bind(Scene& scene, std::function<void()> hover, std::function<void()> click, std::function<void()> idle);

	protected:
		friend Scene;

		void Refresh(mat3f projection_matrix);

		std::shared_ptr<MouseEvent> hover_event = nullptr;
		std::shared_ptr<MousebuttonEvent> click_event = nullptr;

		bool __is_hovering = false;
		bool __is_clicked = false;
	};
	#endif
	//////////////////////////////////////////////////////

	//class DropDownMenu
	//{
	//public:
	//	DropDownMenu(vec2f root_position, vec2f root_dimensions, std::string text, vec4f text_color);

	//};

	////////////////////////////////////////////////////

	//class Slider
	//{

	//};

	////////////////////////////////////////////////////

	//class TextInputField
	//{

	//};

};
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////
