#pragma once


namespace ServerEngine
{
	class CPosition
	{
	public:
		CPosition(void)
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			d = 0.0f;
		}

		~CPosition(void)
		{

		}

	public:

		FLOAT x, y, z, d;

	};

}