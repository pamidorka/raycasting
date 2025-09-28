#pragma once

#include <SFML/Graphics.hpp>

#include "utility.hpp"

namespace e3
{
class camera
{
	private:
		sf::Vector2f position;
		sf::Vector2u size;
		float        horizont;
		float        direction;
		float        fov;
		float        distance;
		float        detalization;
		float        color_rate;

		float direction_normalize(float angle)
		{
			if (angle >= utility::degress_to_radians(360))
			{
				angle -= utility::degress_to_radians(360);
			}
			else if (angle < 0)
			{
				angle += utility::degress_to_radians(360);
			}

			return angle;
		}

	public:
		camera(sf::Vector2u size, sf::Vector2f position, float direction, float fov, float distance)
			: size        (size),
			  horizont    ($(size.y) / 2),
			  position    (position),
			  direction   (utility::degress_to_radians(direction)),
			  fov         (utility::degress_to_radians(fov)),
			  detalization(this->fov / size.x),
			  distance    (distance),
			  color_rate  (255 / distance) { }

		sf::Vector2f next_pos(sf::Vector2f offset)
		{
			return position + offset;
		}

		void move(sf::Vector2f offset)
		{
			position = next_pos(offset);
		}

		void move_horizont(float offset)
		{
			horizont -= offset;
		}

		void rotate(float degrees)
		{
			direction += utility::degress_to_radians(degrees);

			direction = direction_normalize(direction);
		}

		const sf::Vector2f & pos()              const { return position; }
		float                get_direction()    const { return direction; }
		float                get_fov()          const { return fov; }
		float                get_distance()     const { return distance; }
		float                get_detalization() const { return detalization; }
		float                get_color_rate()   const { return color_rate; }
		float                get_horizont()     const { return horizont; }
		const sf::Vector2u & get_size()         const { return size; }
};
}
