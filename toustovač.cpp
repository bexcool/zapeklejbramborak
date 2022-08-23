#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <format>

static float sxm = 1.4;
static float sym = 1.4;
static float fs = false;
static bool loop = true;

// Override base class with your custom functionality
class Bramborak : public olc::PixelGameEngine
{
public:
	Bramborak(int32_t x, int32_t y)
	{
		sAppName = std::format("ZapeklejBramborák - {}x{}", x, y);
	}

public:
	bool OnUserCreate() override
	{
		return true;
	}

	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t r0 = 0;
	uint8_t g0 = 0;
	uint8_t b0 = 0;
	float mindelta = 0.0f;
	float maxdelta = 1.0f;
	float canUpdate = 0.0f;

	bool OnUserUpdate(float elapsedTime) override
	{
		// NOTE(spejik): this is ugly

		if ((canUpdate += elapsedTime) < 0.1)
			if (elapsedTime < mindelta) { mindelta -= 0.00001f;		g = (g + 25) % 256; }
			else if (elapsedTime > maxdelta) { maxdelta += 0.00001f;	r = (r + 25) % 256; }
			else if (elapsedTime > mindelta && elapsedTime < maxdelta) { mindelta *= 69.0f; maxdelta /= 69.0f; b = (b + 25) % 256; }
			else { r = (r + 5) % 256; g = (g + 5) % 256; b = (b + 5) % 256; }
		else if ((canUpdate += elapsedTime) > 0.2f)
		{
			canUpdate = 0.0f;
			mindelta = maxdelta = elapsedTime;
		}

		if (std::fmod(canUpdate, 0.05) < 0.02)
		{
			if (r0 > r)		r0 -= 1;
			else if (r0 < r)r0 += 1;
			if (g0 > g)		g0 -= 1;
			else if (g0 < g)g0 += 1;
			if (b0 > b)		b0 -= 1;
			else if (b0 < b)b0 += 1;
		}

		Clear(olc::Pixel(r0, g0, b0));

		bool hx = false;
		bool hy = false;

		if (GetKey(olc::F).bHeld || GetKey(olc::F).bReleased)
		{
			DrawString({ 20, 40 }, std::format("{} fullscreen on release", !fs ? "Enable" : "Disable"), olc::Pixel(256 - r0, 256 - g0, 256 - b0), 2);
			if (GetKey(olc::F).bReleased)
			{
				fs = !fs;
				loop = true;
				olc_Terminate();
			}
		}
		if (hx = GetKey(olc::X).bHeld)
		{
			DrawString({ 20, 40 }, "Modifying: X", olc::Pixel(256 - r0, 256 - g0, 256 - b0), 2);
		}
		if (hy = GetKey(olc::Y).bHeld)
		{
			DrawString({ 20, 60 }, "Modifying: Y", olc::Pixel(256 - r0, 256 - g0, 256 - b0), 2);
		}
		if (GetKey(olc::NP_ADD).bReleased)
		{
			if (hx) sxm = std::clamp(sxm += 0.2, 0.2f, 6.0f);
			if (hy) sym = std::clamp(sym += 0.2, 0.2f, 6.0f);
			if (hx || hy) { loop = true; olc_Terminate(); }
		}
		else if (GetKey(olc::NP_SUB).bReleased)
		{
			if (hx) sxm = std::clamp(sxm -= 0.2, 0.2f, 6.0f);
			if (hy) sym = std::clamp(sym -= 0.2, 0.2f, 6.0f);
			if (hx || hy) { loop = true; olc_Terminate(); }
		}

		return true;
	}
};

int main()
{
a_terrible_loop:
	loop = !loop;
	int32_t a = 1920 * sxm;
	int32_t b = 1080 * sym;
	Bramborak tortila(a, b);
	std::cout << std::format("{}x{}:{}", a, b, fs) << std::endl;
	if (tortila.Construct(a, b, 1, 1, fs))
		tortila.Start();
	if (loop)
		goto a_terrible_loop;
	return 0;
}