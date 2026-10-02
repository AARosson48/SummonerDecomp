// One pass of bank/004A5750. Strings in this block: Sets avoidance area parameters, avoidance.

struct ConsoleCmd {
	void fn_0050A180(const char* name, const char* help, void (*handler)(void));
};

extern "C" {

void fn_004A67E0(void)
{
	((ConsoleCmd*)0x246b918)->fn_0050A180((char*)0x59ce98, (char*)0x59cea4, (void (*)(void))0x4a6810);
}

struct Point {
	float x;
	float y;
};

static int same_point(Point* a, Point* b)
{
	return a->x == b->x && a->y == b->y;
}

static void write_point(Point* out, Point* p)
{
	if (out)
		*out = *p;
}

// Two segments. Each argument is a pair of points. A shared endpoint returns 3.
// A crossing returns 1, a crossing on an endpoint returns 4, a miss returns 0.
// Parallel segments return -1. Collinear overlap returns 2. The cross products
// are stored at 0x245f3a4, 0x245f3a8, and 0x245f3ac.
int fn_004A5750(Point** seg_a, Point** seg_b, Point* out)
{
	Point* a = seg_a[0];
	Point* b = seg_a[1];
	Point* c = seg_b[0];
	Point* d = seg_b[1];
	float abx;
	float aby;
	float cdx;
	float cdy;
	float acx;
	float acy;
	float den;
	float num;
	float cross;
	Point* lo;
	Point* hi;

	if (same_point(a, c) || same_point(a, d)) {
		write_point(out, a);
		return 3;
	}
	if (same_point(b, c)) {
		write_point(out, d);
		return 3;
	}
	if (same_point(b, d)) {
		write_point(out, b);
		return 3;
	}

	abx = b->x - a->x;
	aby = b->y - a->y;
	cdx = d->x - c->x;
	cdy = d->y - c->y;
	acx = c->x - a->x;
	acy = c->y - a->y;
	den = cdy * abx - cdx * aby;
	num = acx * aby - acy * abx;
	cross = acx * cdy - acy * cdx;
	*(float*)0x245f3a4 = den;
	*(float*)0x245f3a8 = num;
	*(float*)0x245f3ac = cross;

	if (den != 0.0f) {
		if (den < 0.0f) {
			if (num > 0.0f || num < den || cross > 0.0f || cross < den)
				return 0;
		} else if (num < 0.0f || num > den || cross < 0.0f || cross > den) {
			return 0;
		}
		if (out) {
			out->x = cross * abx / den + a->x;
			out->y = cross * aby / den + a->y;
		}
		if (num != 0.0f && num != den && cross != 0.0f && cross != den)
			return 1;
		return 4;
	}
	if (num != 0.0f)
		return -1;

	lo = c;
	hi = d;
	if (abx != 0.0f) {
		if (c->x > d->x) {
			lo = d;
			hi = c;
		}
		if (a->x > lo->x && a->x < hi->x) {
			write_point(out, a);
			return 2;
		}
		if (a->x < lo->x && b->x < lo->x)
			return -1;
		if (a->x > hi->x && b->x > hi->x)
			return -1;
		if (out)
			*out = (b->x - lo->x >= b->x - hi->x) ? *hi : *lo;
		return 2;
	}

	if (c->y > d->y) {
		lo = d;
		hi = c;
	}
	if (a->y > lo->y && a->y < hi->y) {
		write_point(out, a);
		return 2;
	}
	if (a->y < lo->y && b->y < lo->y)
		return -1;
	if (a->y > hi->y && b->y > hi->y)
		return -1;
	if (out)
		*out = (b->y - lo->y >= b->y - hi->y) ? *hi : *lo;
	return 2;
}

}
