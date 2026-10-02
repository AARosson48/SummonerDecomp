// Bank 0048DE60. Adds one vec3 into this.

struct Vec3 {
	float x;
	float y;
	float z;

	void fn_0048E440(Vec3* src);
};

void Vec3::fn_0048E440(Vec3* src)
{
	x += src->x;
	y += src->y;
	z += src->z;
}
