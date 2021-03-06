#include "Game2D.h"
#include "Examples/PrimitivesGallery.h"
#include "RandomNumberGenerator.h"
#include "RigidCircle.h"
#include <vector>
#include <memory>
#include <math.h>
#include "RigidBox.h"


namespace jm
{
	const double PI = 3.1415926;

	const int Number = 20;

	class Example : public Game2D
	{


	public:
		RandomNumberGenerator rnd;

		RigidCircle rb0, rb1;
		RigidBox box;
		weightBox WB;

		RigidCircle RC[Number];

		float boxY = -0.7f;
		//float RCposY = 2.0f;

		int score;

		Example()
			: Game2D()
		{
			reset();
		}

		void reset()
		{
			WB.WB_X = 0.0f;
			WB.angle = 0.0f;
			box.angle = 0.0f;
			score = 0;


			for (int i = 0; i < Number; i++)
			{
				RC[i].pos = vec2(rnd.getFloat(-1.0f, 1.0f), rnd.getFloat(0.5f, 3.0f));
				RC[i].vel = vec2(rnd.getFloat(-5.0f, 5.0f), 0.0f);
				RC[i].radius = rnd.getFloat(0.05f, 0.15f);
				RGB TemColor{ rnd.getInt(0, 255), rnd.getInt(0, 255), rnd.getInt(0, 255) };
				RC[i].color = TemColor;
			}


		}

		void drawWall()
		{
			setLineWidth(5.0f);
			//drawLine(Colors::blue, { -1.0f, -1.0f }, Colors::blue, { 1.0f, -1.0f });
			drawLine(Colors::blue, { 1.0f, -1.0f }, Colors::blue, { 1.0f, 1.0f });
			drawLine(Colors::blue, { -1.0f, -1.0f }, Colors::blue, { -1.0f, 1.0f });
		}

		void update() override
		{
			const float dt = getTimeStep() * 0.4f;
			const float epsilon = 0.6f;
			static const float coef_res = 0.7f;
			float Power = WB.WB_X * 3;


			for (int i = 0; i < Number; i++)
			{
				RC[i].update(dt);
			}

			/////////////////////////////////////////////////
			for (int i = 0; i < Number; i++)
			{
				for (int j = 0; j < Number; ++j)
				{
					if (i != j)
					{
						const float distance = (RC[i].pos - RC[j].pos).getMagnitude();

						if (distance <= RC[i].radius + RC[j].radius)
						{
							// compute impulse
							const auto vel_rel = RC[i].vel - RC[j].vel;
							const auto normal = -(RC[j].pos - RC[i].pos) / (RC[j].pos - RC[i].pos).getMagnitude();


							if (vel_rel.getDotProduct(normal) < 0.0f) // approaching
							{

								const auto impulse = normal * -(1.0f + epsilon) * vel_rel.getDotProduct(normal) /
									((1.0f / RC[i].mass) + (1.0f / RC[j].mass));

								// update velocities of two bodies
								RC[i].vel += impulse / RC[i].mass;
								RC[j].vel -= impulse / RC[j].mass;
							}
						}

					}
				}
			}
			/////////////////////////////////////////////////
			for (int i = 0; i < Number; i++)
			{

				if (RC[i].pos.x <= 0.5f * cos(box.angle * PI / 180) && RC[i].pos.x >= -0.5f * cos(box.angle * PI / 180))
				{
					float a1 = tan(box.angle * PI / 180);
					float b1 = -1.0f;
					float c1 = -0.7f;

					float a2 = tan((box.angle + 90.0f) * PI / 180);
					float b2 = -1.0f;
					float c2 = RC[i].pos.y - tan((box.angle + 90.0f) * PI / 180) * RC[i].pos.x;

					float Px = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
					float Py = -(a1 / b1) * ((b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1)) - c1 / b1;

					vec2 tmp = { Px, Py };

					const float distance = (RC[i].pos - tmp).getMagnitude();

					if (distance <= RC[i].radius)
					{
						Power += RC[i].pos.x * RC[i].mass * RC[i].vel.getMagnitude() * pow(10, -16) * 4;

						RC[i].vel.x *= cos((box.angle + 90.0f) * PI / 180) * -1.3f;
						RC[i].vel.y *= sin((box.angle + 90.0f) * PI / 180) * -1.3f;

					}

				}



			}
			/////////////////////////////////////////////////

			for (int i = 0; i < Number; i++)
			{

				if (RC[i].pos.y <= -1.0f + RC[i].radius) // ground
				{

					RC[i].pos = vec2(rnd.getFloat(-1.0f, 1.0f), 1.0f);
					RC[i].vel = vec2(rnd.getFloat(-5.0f, 5.0f), 1.0f);

					RC[i].radius = rnd.getFloat(0.01f, 0.15f);
					RGB TemColor{ rnd.getInt(0, 255), rnd.getInt(0, 255), rnd.getInt(0, 255) };
					RC[i].color = TemColor;

					score++;
				}
			}


			/////////////////////////////////////////////////


			box.update(dt, Power);
			WB.update(dt, Power);


			// draw
			drawWall();
			box.draw();
			WB.draw();


			for (int i = 0; i < Number; i++)
			{
				RC[i].draw();
			}

			if (abs(sin(box.angle * PI / 180) * (box.boxWitdh / 2)) >= (1 - abs(boxY)))
			{
				reset();
			}

			// reset button
			if (isKeyPressedAndReleased(GLFW_KEY_R)) reset();

			if (isKeyPressed(GLFW_KEY_A) && WB.WB_X >= -0.4) WB.WB_X -= 0.02;
			if (isKeyPressed(GLFW_KEY_D) && WB.WB_X <= 0.4) WB.WB_X += 0.02;


			std::cout << score << std::endl;
		}

	};
}

int main(void)
{
	jm::Example().run();


	return 0;
}
