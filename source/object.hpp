#pragma once

#include <SFML/Graphics.hpp>

inline constexpr sf::Color Sky(135, 206, 235);

namespace e3
{
namespace object
{
class abstract
{
	public:
		enum types
		{
			air,
			block,
			textured,
		};

	private:
		sf::Vector2u position;
		types        type;

	public:
		abstract(sf::Vector2u p, types t)
			: position(p),
			  type(t) { }

		sf::Vector2u get_position() const
		{
			return position;
		}

		types get_type() const
		{
			return type;
		}
};

class air : public abstract
{
	public:
		air(sf::Vector2u p)
			: abstract(p, types::air) { }
};

class block : public abstract
{
	private:
		sf::Color color;

	public:
		block(sf::Vector2u p, sf::Color c)
			: abstract(p, types::block),
			  color(c) { }

		sf::Color get_color() const
		{
			return color;
		}
};

class textured : public abstract
{
	private:
		sf::Texture texture;

	public:
		textured(sf::Vector2u p, const std::string_view & path)
			: abstract(p, types::textured),
			  texture(path) { }

		const sf::Texture& get_texture() const
		{
			return texture;
		}
};

}
}
