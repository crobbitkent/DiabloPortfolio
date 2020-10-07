#pragma once


class MyGraphic
{
public:
	static int ColorToInt(int r, int g, int b, int a)
	{
		int color;

		char* arrColor = (char*)&color;

		arrColor[0] = r;
		arrColor[1] = g;
		arrColor[2] = b;
		arrColor[3] = a;

		return color;
	}


private:
	MyGraphic();
	virtual ~MyGraphic() = 0;
};

