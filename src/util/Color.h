#ifndef __COLOR_H__
#define __COLOR_H__

struct EDMODEL_API Color
{
public:
	float values[4];
	Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f) {values[0] = r; values[1] = g; values[2] = b; values[3] = a; }

	Color contrast(float val) const {
		Color color = Color(*this);
		float sum = (color.values[0] + color.values[1] + color.values[2]) / 3.0f;
		for (int i=0; i<3; i++) {
			color.values[i] = min(max( color.values[i] + ((color.values[i] - sum) * val), 0.0f), 1.0f);
		}
		return color;
	}
};

inline Color operator+(const Color& color, float val) {
	if (val > 0) {
		return Color( min(1.0f, color.values[0] + val),	min(1.0f, color.values[1] + val), min(1.0f, color.values[2] + val),	color.values[3]);
	} else {
		return Color( max(0.0f, color.values[0] + val),	max(0.0f, color.values[1] + val), max(0.0f, color.values[2] + val),	color.values[3]);
	}
}

inline Color operator-(const Color& color, float val) {
	if (val < 0) {
		return Color( min(1.0f, color.values[0] - val),	min(1.0f, color.values[1] - val), min(1.0f, color.values[2] - val),	color.values[3]);
	} else {
		return Color( max(0.0f, color.values[0] - val),	max(0.0f, color.values[1] - val), max(0.0f, color.values[2] - val),	color.values[3]);
	}
}

inline Color operator*(const Color& color, float val) {
	if (val > 0) {
		return Color( min(1.0f, color.values[0] * val),	min(1.0f, color.values[1] * val), min(1.0f, color.values[2] * val),	color.values[3]);
	} else {
		return Color( max(0.0f, color.values[0] * val),	max(0.0f, color.values[1] * val), max(0.0f, color.values[2] * val),	color.values[3]);
	}
}

#endif
