
#include "raycasting.hpp"

sf::Vector2f getVector(float direction)
{
	sf::Vector2f speed;

	speed.x = 0.003 * cosf(direction);
	speed.y = 0.003 * sinf(direction);

	return speed;
}

void move_event(const e3::map& map, e3::camera& cam, float time)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		sf::Vector2f speed = getVector(cam.get_direction() + e3::utility::degress_to_radians(90));
		sf::Vector2f pos   = cam.next_pos(speed * time);

		if (map.object({_(pos.x), _(pos.y)})->get_type() == e3::object::abstract::air)
		{
			cam.move(speed * time);
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		sf::Vector2f speed = getVector(cam.get_direction() - e3::utility::degress_to_radians(90));
		sf::Vector2f pos   = cam.next_pos(speed * time);

		if (map.object({_(pos.x), _(pos.y)})->get_type() == e3::object::abstract::air)
		{
			cam.move(speed * time);
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		sf::Vector2f speed = getVector(cam.get_direction());
		sf::Vector2f pos   = cam.next_pos(speed * time);

		if (map.object({_(pos.x), _(pos.y)})->get_type() == e3::object::abstract::air)
		{
			cam.move(speed * time);
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		sf::Vector2f speed = getVector(cam.get_direction());
		sf::Vector2f pos   = cam.next_pos(-speed * time);

		if (map.object({_(pos.x), _(pos.y)})->get_type() == e3::object::abstract::air)
		{
			cam.move(-speed * time);
		}
	}
}

void draw_camera_view(const std::vector <sf::RectangleShape>& drawables, sf::RenderWindow& window)
{
	for (auto drawable = drawables.begin(); drawable != drawables.end(); drawable++)
	{
		window.draw(*drawable);
	}
}

int main()
{
	sf::Vector2u window_size(1280, 720);

	sf::RenderWindow window(
		sf::VideoMode(window_size),
		"Raycasting",
		sf::Style::Close);

	e3::map map(
		{
			"###################",
			"#           #     #",
			"#   #   #   %     #",
			"#           #     #",
			"#    #      #     #",
			"#       #   #     #",
			"#   #       ##### #",
			"#                 #",
			"#                 #",
			"#   $             #",
			"#                 #",
			"#   #             #",
			"#                 #",
			"#   #             #",
			"#   #             #",
			"#                 #",
			"##########        #",
			"#                 #",
			"###################"
		});

	e3::camera cam(window_size, {10, 10}, 0, 90, 30);

	e3::pseudo::raycasting engine(0.03, 0.0015);

	sf::Clock clock;

	while (window.isOpen())
	{
		float time     = $(clock.getElapsedTime().asMicroseconds()) / 800;
		float fps_time = $(clock.getElapsedTime().asSeconds());

		clock.restart();

		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}

			if (event->is<sf::Event::KeyPressed>())
			{
				const sf::Event::KeyPressed* key = event->getIf<sf::Event::KeyPressed>();

				if (key && key->code == sf::Keyboard::Key::Escape)
				{
					window.close();
				}
			}

			if (event->is<sf::Event::MouseMovedRaw>())
			{
				const sf::Event::MouseMovedRaw* mouse = event->getIf<sf::Event::MouseMovedRaw>();

				if (mouse)
				{
					sf::Vector2i delta = mouse->delta;

					cam.rotate       (0.007 * delta.x * time);
					cam.move_horizont(0.05 * delta.y * time);
				}
			}
		}

		move_event(map, cam, time);

		window.clear(Sky);

		std::vector <sf::RectangleShape> drawables = engine.get_window_draw_objects(cam, map);

		draw_camera_view(drawables, window);

		window.display();
	}
}
