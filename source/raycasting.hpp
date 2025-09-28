#pragma once

#include <stdexcept>
#include <utility>

#include "map.hpp"
#include "camera.hpp"
#include "object.hpp"

namespace e3
{
namespace pseudo
{

class raycasting
{
	private:
		float ray_step_base;
		float ray_step_repeat;

		sf::Color color_by_distance(const sf::Color& base, float distance, float rate)
		{
			float alpha = base.a - (distance * rate);

			sf::Vector3i color(
				base.r  - (distance * rate),
				base.g  - (distance * rate),
				base.b  - (distance * rate));

			if (alpha   < 0) alpha   = 0;
			if (color.x < 0) color.x = 0;
			if (color.y < 0) color.y = 0;
			if (color.z < 0) color.z = 0;

			return sf::Color(color.x, color.y, color.z, alpha);
		}

	public:
		using object_packed = std::pair <const object::abstract*, float>;

		raycasting(float ray_step_base, float ray_step_repeat)
			: ray_step_base  (ray_step_base),
			  ray_step_repeat(ray_step_repeat) { }

		sf::RectangleShape create_drawable_object(
			const camera& cam, object_packed object_packed, unsigned int ray, float offset)
		{
			const object::abstract* object   = object_packed.first;
			float                   distance = object_packed.second;

			sf::RectangleShape drawable;

			float column_heigth = cam.get_size().y / (distance * cosf(offset));

			drawable.setSize    ({1,      column_heigth});
			drawable.setPosition({$(ray), cam.get_horizont() - (column_heigth / 2)});

			if (!object ||
			    object->get_type() == object::abstract::air)
			{
				drawable.setFillColor(
					color_by_distance(
						sf::Color::White, distance,
						cam.get_color_rate()));

				return drawable;
			}

			switch (object->get_type())
			{
				case object::abstract::block:
				{
					const object::block* block = (object::block*)object;
					
					drawable.setFillColor(
						color_by_distance(
							block->get_color(),
							distance,
							cam.get_color_rate()));
					
					break;
				}
				case object::abstract::textured:
				{
					/*
						Этот тип объекта работает неправильно.

						Я так и не смог придумать нормального решения по отображению
						текстур на объекты. Был придуман костыль на static переменных,
						но по итогу я решил его удалить.

						Проблема заключается в непонимании выбора части текстуры, необходимой
						для отрисовки. В независимости от номера луча, когда первый луч попадает
						на текстурированный объект мы должны нарисовать первую часть текстуры,
						но я не совсем понял как определить, что это первый луч на этом объекте
						без static переменных. По этой причине в текущей реализации текстура
						на объекте "двигается".
					*/

					const object::textured* textured = (object::textured*)object;
					sf::Vector2u size                = textured->get_texture().getSize();

					drawable.setTextureRect(sf::IntRect({__(ray % size.x), 0}, {1, __(size.y)}));
					drawable.setTexture(&textured->get_texture());

					break;
				}
				default:
				{
					std::runtime_error("Can't draw the block type");
				}
			}

			return drawable;
		}

		object_packed throw_ray(const map& map, const camera& cam, float offset)
		{
			float distance = 0;
			float cos      = cosf(offset);
			float sin      = sinf(offset);

			float x = cam.pos().x;
			float y = cam.pos().y;

			while (map.object({_(x), _(y)})->get_type() == object::abstract::air &&
			       distance < cam.get_distance())
			{
				distance += ray_step_base;
				x = cam.pos().x + distance * cos;
				y = cam.pos().y + distance * sin;
			}

			if (map.object({_(x), _(y)})->get_type() != object::abstract::air)
			{
				distance -= ray_step_base;
				x = cam.pos().x + distance * cos;
				y = cam.pos().y + distance * sin;

				while (map.object({_(x), _(y)})->get_type() == object::abstract::air)
				{
					distance += ray_step_repeat;
					x = cam.pos().x + distance * cos;
					y = cam.pos().y + distance * sin;
				}
			}

			return {map.object({_(x), _(y)}), distance};
		}

		std::vector <sf::RectangleShape> get_window_draw_objects(const camera& cam, const map& map)
		{
			float ray_start = cam.get_direction() - (cam.get_fov() / 2);
			float ray_end   = cam.get_direction() + (cam.get_fov() / 2);

			std::vector <sf::RectangleShape> drawables;

			unsigned int ray = 0;

			for (float offset = ray_start;
			     offset < ray_end;
			     offset += cam.get_detalization(), ray++)
			{
				object_packed object_packed = throw_ray(map, cam, offset);

				drawables.emplace_back(
					create_drawable_object(
						cam,
						object_packed,
						ray,
						cam.get_direction() - offset));
			}

			return drawables;
		}
};

}
}
