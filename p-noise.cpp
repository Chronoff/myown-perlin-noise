#include <cstdlib>
#include <iostream>
#include <cmath>

const float PI = 3.14159;
const unsigned int HE = 10;
const unsigned int WI = 10;

int hash(int x, int y, int seed)
{
	int result = y ^ x ^ seed;
	result *= 0x45d9f3b;
	result ^= result >> 4;
	result *= 0xc2b2ae35;
	result ^= result >> 11;
	return result;
}

float getGradientAngle(int seed1, int seed2, int seed3)
{
	int result = hash(seed1, seed2, seed3);
	return result%static_cast<int>(2*PI*10000+1)/10000.0;
}

int simpleHash(int a, int b) {
    return (a << 16) ^ b; // Shift 'a' and combine with 'b'
}

float fadeFunction(float t) // t between 0 and 1
{
	return 6*std::pow(t, 5) - 15*std::pow(t, 4) + 10*std::pow(t, 3); // this works
}


class Vector2 // starts normalized with random direction
{
	private:
		float n;
	public:
		int pos_x; //where it is in space
		int pos_y;
		float x; // for direction normalized to unit
		float y;
		float disp_x; // how it is displaced/offset from a chosen point
		float disp_y;

		Vector2(int _x, int _y, float dis_x, float dis_y, int seed) : pos_x(_x), pos_y(_y), disp_x(dis_x), disp_y(dis_y)
    		{
        		float angle = getGradientAngle(pos_x, pos_y, seed);
        		x = std::cos(angle);
        		y = std::sin(angle);
    		}

		float dotProduct() // figures_out the dot product between gradient and offset vector
		{
			float result = 0;
			// dotProd = g1*o1 + g2*o2
			result = x*disp_x + y*disp_y;

			return result;
		}

};


class Grid
{
	public:
		float field[HE][WI];
		Grid() : field() {}
};

float makePerlinNoise(float x, float y, int seed)
{
	int grid_x = static_cast<int>(x);
	int grid_y = static_cast<int>(y);

	float dot1, dot2, dot3, dot4;

	float x_axis_fade;
	float y_axis_fade;

	float i1, i0;

	Vector2 ve1(grid_x, grid_y, x-grid_x, y-grid_y, seed); // i am sorry for this abomination
	Vector2 ve2(grid_x+1, grid_y, x-(grid_x+1), y-grid_y, seed);
	Vector2 ve3(grid_x, grid_y+1, x-grid_x, y-(grid_y+1), seed);
	Vector2 ve4(grid_x+1, grid_y+1, x-(grid_x+1), y-(grid_y+1), seed); // corner vectors should be setup

	dot1 = ve1.dotProduct(); // dot products between ofset and gradient vector of all 4 corners
	dot2 = ve2.dotProduct();
	dot3 = ve3.dotProduct();
	dot4 = ve4.dotProduct();

	x_axis_fade = fadeFunction(x-ve1.pos_x); // fade function aplied for y and x axis
	y_axis_fade = fadeFunction(y-ve1.pos_y);

	i0 = (1-x_axis_fade)*dot1 + x_axis_fade*dot2; //interpolate along axis
	i1 = (1-x_axis_fade)*dot3 + x_axis_fade*dot4;

	return ((1 - y_axis_fade) * i0 + y_axis_fade * i1) * 2;
	}

float combined(float x, float y, int seed)
{
	return ((makePerlinNoise(x*4, y*4, seed)/2) + (makePerlinNoise(x, y, seed)))/1.5; 
}

int main()
{
	unsigned int seed = 54525364;
	std::cout << makePerlinNoise(1.3, 1.6, seed) << std::endl;
	std::cout << getGradientAngle(1, 3, seed) << std::endl;
	std::cout << getGradientAngle(7, 2, seed) << std::endl;
	for (float y = 0.11; y < 3; y += 0.05)
	{
		for (float x = 0.11; x < 3; x += 0.05)
		{
			if (combined(x, y, seed) > 0.75) {std::cout << "." << " ";}
			else if (combined(x, y, seed) > 0.5) {std::cout << "_" << " ";}
			else if (combined(x, y, seed) > 0.25) {std::cout << "+" << " ";}
			else if (combined(x, y, seed) > 0) {std::cout << "| ";}
			else if (combined(x, y, seed) > -0.25) {std::cout << "O" << " ";}
			else if (combined(x, y, seed) > -0.5) {std::cout << "Q" << " ";}
			else {std::cout << "8 ";}
			
			//std::cout << makePerlinNoise(x, y, seed) << " ";
		}
		std::cout << std::endl;
	}
	
		
	return 0;
}
