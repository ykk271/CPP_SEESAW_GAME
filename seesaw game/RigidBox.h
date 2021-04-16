#pragma once
#include "Game2D.h"
#include <Windows.h>

namespace jm
{
	class weightBox
	{

	public:
		vec2 vel;
		RGB color = Colors::red;

		float mass = 1.0f;
		float timeCheck;
		float boxWitdh = 0.1f;
		float boxHeight = 0.1f;

		float WB_X = 0.0f;
		float angle = 0.0f;

		void draw()
		{

			beginTransformation();
			{
				translate(vec2(0.0f, -0.7f));
				rotate(angle);
				translate(vec2(WB_X, boxHeight/2 + 0.01f));

				drawFilledBox(color, boxWitdh, boxHeight);

			}
			endTransformation();

		}



		void update(const float& dt, const float & Power)
		{
			Timer timer;
			timer.start();

			angle += angle * 0.02;
			angle -= Power;


			timeCheck = timer.stopAndGetElapsedMilli();
			Sleep(10 - timeCheck);
		}
	};

	class RigidBox
	{

	public:
		vec2 pos = { 0.0f, -0.7f };
		vec2 vel;
		RGB color = Colors::black;

		float mass = 1.0f;
		float timeCheck;
		float boxWitdh = 1.0f;
		float boxHeight = 0.02f;
		float angle = 0.0f;

		void draw()
		{
			beginTransformation();
			{
				translate(pos);
				rotate(angle);
				drawFilledBox(color, boxWitdh, boxHeight);
				drawFilledCircle(Colors::red, 0.01);
			}
			endTransformation();
		}

		void update(const float& dt, const float& Power)
		{
			Timer timer;
			timer.start();

			angle += angle * 0.02;
			angle -= Power;
			
			//angle = 30;

			timeCheck = timer.stopAndGetElapsedMilli();
			Sleep(10 - timeCheck);
		}
	};
}