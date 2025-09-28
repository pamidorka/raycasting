#pragma once

#include "object.hpp"

#include <vector>

namespace e3
{

class map
{
	private:
		std::vector <std::vector <std::unique_ptr <object::abstract>>> objects;

	public:
		map(std::vector <std::string> raw)
		{
			for (int i = 0; i < raw.size(); i++)
			{
				objects.emplace_back(std::vector <std::unique_ptr <object::abstract>>());

				for (int j = 0; j < raw[i].length(); j++)
				{
					std::unique_ptr <object::abstract> object;

					switch (raw[i][j])
					{
						case ' ':
						{
							object = std::make_unique <object::air>(sf::Vector2u(j, i));
							break;
						}
						case '#':
						{
							object =
								std::make_unique <object::block>(
									sf::Vector2u(j, i), sf::Color::Green);
							break;
						}
						case '$':
						{
							object =
								std::make_unique <object::block>(
									sf::Vector2u(j, i), sf::Color::Blue);
							break;
						}
						case '&':
						{
							/* Этот тип объекта работает неправильно */
							object =
								std::make_unique <object::textured>(
									sf::Vector2u(j, i), "texture/wall.jpg");
							break;
						}
						case '%':
						{
							object =
								std::make_unique <object::block>(
									sf::Vector2u(j, i), sf::Color::Red);
							break;
						}
						default:
						{
							throw std::runtime_error("undefined object type in map");
						}
					}

					objects[i].push_back(std::move(object));
				}
			}
		}

		const object::abstract * object(sf::Vector2u pos) const
		{
			return objects[pos.y][pos.x].get();
		}
};

}
