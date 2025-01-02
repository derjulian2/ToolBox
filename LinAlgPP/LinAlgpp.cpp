//////////////////////////////////////////////////
/// LinAlg++ Library						   ///
///											   ///
/// Provides Helper Classes and Functions for  ///
/// handling Linear Algebra Math in C++		   ///
/// 2024 Julian Benzel | .cpp Source File	   ///
//////////////////////////////////////////////////
#include "LinAlgpp.hpp"
#ifdef LINALGPP_LIB
//////////////////////////////////////////////////
/// TWO DIMENSIONAL VECTOR
//////////////////////////////////////////////////
vec2f::vec2f() 
{
	this->x = 0, this->y = 0; 
};
vec2f::vec2f(float num)
{ 
	this->x = num, this->y = num;
};
vec2f::vec2f(float x, float y) : x(x), y(y) {

}
//////////////////////////////////////////////////
/// THREE DIMENSIONAL VECTOR
//////////////////////////////////////////////////
vec3f::vec3f()
{
	this->x = 0, this->y = 0, this->z = 0; 
};
vec3f::vec3f(float num)
{
	this->x = num, this->y = num, this->z = num; 
};
vec3f::vec3f(float x, float y, float z)
{
	this->x = x, this->y = y, this->z = z; 
}
vec3f::vec3f(vec2f v, float z) : x(v.x), y(v.y), z(z) {

}
//////////////////////////////////////////////////
/// FOUR DIMENSIONAL VECTOR
//////////////////////////////////////////////////
vec4f::vec4f() 
{ 
	this->x = 0, this->y = 0, this->z = 0, this->w = 0;
};
vec4f::vec4f(float num) 
{
	this->x = num, this->y = num, this->z = num, this->w = num;
};
vec4f::vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {

};
//////////////////////////////////////////////////
/// 2 X 2 SQUARE MATRIX
//////////////////////////////////////////////////
mat2f::mat2f() 
{
	this->x = vec2f(), this->y = vec2f(); 
}
mat2f::mat2f(float x)
{
	this->x = vec2f(x, 0), this->y = vec2f(0, x);
}
mat2f::mat2f(vec2f x, vec2f y) : x(x), y(y) {

}
//////////////////////////////////////////////////
/// 3 X 3 SQUARE MATRIX
//////////////////////////////////////////////////
mat3f::mat3f() 
{ 
	this->x = vec3f(), this->y = vec3f(), this->z = vec3f();
}
mat3f::mat3f(float x) 
{ 
	this->x = vec3f(x, 0, 0), this->y = vec3f(0, x, 0), this->z = vec3f(0, 0, x);
}
mat3f::mat3f(vec3f x, vec3f y, vec3f z) : x(x), y(y), z(z) {

}
//////////////////////////////////////////////////
/// 4 X 4 SQUARE MATRIX
//////////////////////////////////////////////////
mat4f::mat4f() {
	this->x = vec4f(), this->y = vec4f(), this->z = vec4f(), this->w = vec4f();
}
mat4f::mat4f(float x) {
	this->x = vec4f(x, 0, 0, 0), this->y = vec4f(0, x, 0, 0), this->z = vec4f(0, 0, x, 0), this->w = vec4f(0, 0, 0, x);
}
mat4f::mat4f(vec4f x, vec4f y, vec4f z, vec4f w) : x(x), y(y), z(z), w(w) {

}
//////////////////////////////////////////////////
Line2f::Line2f() : start(), end() {}
Line2f::Line2f(vec2f start, vec2f end) : start(start), end(end) {}
//////////////////////////////////////////////////
///
/// BASIC OPERATOR FUNCTIONALITY
/// 
//////////////////////////////////////////////////
/// ADDITION
vec2f operator+(vec2f v, vec2f w) { return vec2f(v.x + w.x, v.y + w.y); }
vec3f operator+(vec3f v, vec3f w) { return vec3f(v.x + w.x, v.y + w.y, v.z + w.z); }
/// SUBRACTION
vec2f operator-(vec2f v, vec2f w) { return vec2f(v.x - w.x, v.y - w.y); }
vec3f operator-(vec3f v, vec3f w) { return vec3f(v.x - w.x, v.y - w.y, v.z - w.z); }
/// MULTIPLICATION
vec2f operator*(vec2f v, vec2f w)  { return vec2f(v.x * w.x, v.y * w.y); }
vec2f operator*(vec2f v, float f) { return vec2f(v.x * f, v.y * f); }
vec2f operator*(float f, vec2f v) { return vec2f(v.x * f, v.y * f); }

vec3f operator*(vec3f v, vec3f w)  { return vec3f(v.x * w.x, v.y * w.y, v.z * w.z); }
vec3f operator*(vec3f v, float f) { return vec3f(v.x * f, v.y * f, v.z * f); }
vec3f operator*(float f, vec3f v) { return vec3f(v.x * f, v.y * f, v.z * f); }

vec4f operator*(vec4f v, vec4f w)  { return vec4f(v.x * w.x, v.y* w.y, v.z* w.z, v.w * w.w); }
vec4f operator*(vec4f v, float f) { return vec4f(v.x * f, v.y * f, v.z * f, v.w * f); }
vec4f operator*(float f, vec4f v) { return vec4f(v.x * f, v.y * f, v.z * f, v.w * f); }

mat2f operator*(float f, mat2f m) { return mat2f(f * m.x, f * m.y); }
mat2f operator*(mat2f m, float f) { return mat2f(f * m.x, f * m.y); }
vec2f operator*(mat2f m, vec2f v) { return vec2f(dot(m.x, v), dot(m.y, v)); }
mat2f operator*(mat2f m, mat2f w) { // MATRIX MULTIPLICATION
	return mat2f(
		vec2f(dot(m.x, vec2f(w.x.x, w.y.x)), dot(m.x, vec2f(w.x.y, w.y.y))),
		vec2f(dot(m.y, vec2f(w.x.x, w.y.x)), dot(m.y, vec2f(w.x.y, w.y.y)))
	);
}

mat3f operator*(mat3f m, float f) { return mat3f(m.x * f, m.y * f, m.z * f); }
mat3f operator*(float f, mat3f m) { return mat3f(m.x * f, m.y * f, m.z * f); }
vec3f operator*(mat3f m, vec3f v) { return vec3f(dot(m.x, v), dot(m.y, v), dot(m.z, v)); }
mat3f operator*(mat3f m, mat3f w) { // MATRIX MULTIPLICATION
	return mat3f(
		vec3f(dot(m.x, vec3f(w.x.x, w.y.x, w.z.x)), dot(m.x, vec3f(w.x.y, w.y.y, w.z.y)), dot(m.x, vec3f(w.x.z, w.y.z, w.z.z))),
		vec3f(dot(m.y, vec3f(w.x.x, w.y.x, w.z.x)), dot(m.y, vec3f(w.x.y, w.y.y, w.z.y)), dot(m.y, vec3f(w.x.z, w.y.z, w.z.z))),
		vec3f(dot(m.z, vec3f(w.x.x, w.y.x, w.z.x)), dot(m.z, vec3f(w.x.y, w.y.y, w.z.y)), dot(m.z, vec3f(w.x.z, w.y.z, w.z.z)))
	);
}

/// COMPARISON
bool operator==(vec2f v, vec2f w) { if (v.x == w.x && v.y == w.y) return true; else return false; }
bool operator!=(vec2f v, vec2f w) { if (v.x == w.x && v.y == w.y) return false; else return true; }
bool operator> (vec2f v, vec2f w) { if (v.x >  w.x && v.y >  w.y) return true; else return false; }
bool operator>=(vec2f v, vec2f w) { if (v.x >= w.x && v.y >= w.y) return true; else return false; }
bool operator< (vec2f v, vec2f w) { if (v.x <  w.x && v.y <  w.y) return true; else return false; }
bool operator<=(vec2f v, vec2f w) { if (v.x <= w.x && v.y <= w.y) return true; else return false; }

bool operator==(vec3f v, vec3f w) { if (v.x == w.x && v.y == w.y && v.z == w.z) return true; else return false; }
bool operator!=(vec3f v, vec3f w) { if (v.x == w.x && v.y == w.y && v.z == w.z) return false; else return true; }
bool operator> (vec3f v, vec3f w) { if (v.x > w.x && v.y > w.y && v.z > w.z) return true; else return false; }
bool operator>=(vec3f v, vec3f w) { if (v.x >= w.x && v.y >= w.y && v.z >= w.z) return true; else return false; }
bool operator< (vec3f v, vec3f w) { if (v.x < w.x && v.y < w.y && v.z < w.z) return true; else return false; }
bool operator<=(vec3f v, vec3f w) { if (v.x <= w.x && v.y <= w.y && v.z <= w.z) return true; else return false; }

bool operator==(vec4f v, vec4f w) { if (v.x == w.x && v.y == w.y && v.z == w.z && v.w == w.w) return true; else return false; }
bool operator!=(vec4f v, vec4f w) { if (v.x == w.x && v.y == w.y && v.z == w.z && v.w == w.w) return false; else return true; }
bool operator> (vec4f v, vec4f w) { if (v.x > w.x && v.y > w.y && v.z > w.z && v.w > w.w) return true; else return false; }
bool operator>=(vec4f v, vec4f w) { if (v.x >= w.x && v.y >= w.y && v.z >= w.z && v.w >= w.w) return true; else return false; }
bool operator< (vec4f v, vec4f w) { if (v.x < w.x && v.y < w.y && v.z < w.z && v.w < w.w) return true; else return false; }
bool operator<=(vec4f v, vec4f w) { if (v.x <= w.x && v.y <= w.y && v.z <= w.z && v.w <= w.w) return true; else return false; }
//////////////////////////////////////////////////
///
/// ADVANCED VECTOR OPERATIONS
/// 
//////////////////////////////////////////////////
/// VECTOR LENGTH
float length(vec2f v) { return sqrt(pow(v.x, 2) + pow(v.y, 2)); }
float length(vec3f v) { return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2)); }
float length(vec4f v) { return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2) + pow(v.w, 2)); }
/// DOT PRODUCT
float dot(vec2f v, vec2f w) { return v.x * w.x + v.y * w.y; }
float dot(vec3f v, vec3f w) { return v.x * w.x + v.y * w.y + v.z * w.z; }
float dot(vec4f v, vec4f w) { return v.x * w.x + v.y * w.y + v.z * w.z + v.w * w.w; }
/// CROSS PRODUCT
vec3f cross(vec3f v, vec3f w) { return vec3f(v.y * w.z - v.z * w.y, v.z * w.x - v.x * w.z, v.x * w.y - v.y * w.x ); }
/// UNIT VECTOR
vec2f unit(vec2f v) { if (length(v) != 0) return v * float(1.0f / length(v)); else return vec2f(); }
vec3f unit(vec3f v) { if (length(v) != 0) return v * float(1.0f / length(v)); else return vec3f(); }
vec4f unit(vec4f v) { if (length(v) != 0) return v * float(1.0f / length(v)); else return vec4f(); }
/// ANGLE BETWEEN VECTORS
float angle(vec2f v, vec2f w) {
	return degr(acos(dot(v,w) / (length(v) * length(w))));
}
float angle(vec3f v, vec3f w) {
	return degr(acos(dot(v, w) / (length(v) * length(w))));
}
float angle(vec4f v, vec4f w) {
	return degr(acos(dot(v, w) / (length(v) * length(w))));
}
//////////////////////////////////////////////////
///
/// ADVANCED MATRIX OPERATIONS
///
//////////////////////////////////////////////////

float determinant(mat2f m) { return m.x.x * m.y.y - m.x.y * m.y.x; }
float determinant(mat3f m) {
	return m.x.x * m.y.y * m.z.z + m.x.y * m.y.z * m.z.x + m.x.z * m.y.x * m.z.y
		- m.z.x * m.y.y * m.x.z - m.z.y * m.y.z * m.x.x - m.z.z * m.y.x * m.x.y;
}

mat3f scale(mat3f m, vec2f s) {
	return mat3f(vec3f(s.x, m.x.y, m.x.z), vec3f(m.y.x, s.y, m.y.z), m.z);
}

mat3f translate(mat3f m, vec2f t) {
	return mat3f(vec3f(m.x.x, m.x.y, t.x), vec3f(m.y.x, m.y.y, t.y), m.z);
}

mat2f inverse(mat2f m)
{
	if (determinant(m) == 0.0f)
		return m;
	return (1.0f / determinant(m)) * mat2f(vec2f(m.y.y, -m.x.y), vec2f(-m.y.x, m.x.x));
}
mat3f inverse(mat3f m) {
	// check if matrix has an inverse
	if (determinant(m) == 0.0f)
		return m;
	return adjunct(m) * (1 / determinant(m));
}

mat3f adjunct(mat3f m) {
	mat3f adjunct =
	{
		vec3f(determinant(mat2f(vec2f(m.y.y,m.y.z),vec2f(m.z.y,m.z.z))),determinant(mat2f(vec2f(m.y.x,m.y.z),vec2f(m.z.x,m.z.z))),determinant(mat2f(vec2f(m.y.x,m.y.y),vec2f(m.z.x,m.z.y)))),
		vec3f(determinant(mat2f(vec2f(m.x.y,m.x.z),vec2f(m.z.y,m.z.z))),determinant(mat2f(vec2f(m.x.x,m.x.z),vec2f(m.z.x,m.z.z))),determinant(mat2f(vec2f(m.x.x,m.x.y),vec2f(m.z.x,m.z.y)))),
		vec3f(determinant(mat2f(vec2f(m.x.y,m.x.z),vec2f(m.y.y,m.y.z))),determinant(mat2f(vec2f(m.x.x,m.x.z),vec2f(m.y.x,m.y.z))),determinant(mat2f(vec2f(m.x.x,m.x.y),vec2f(m.y.x,m.y.y))))
	};
	adjunct =
	{
		vec3f(0 + adjunct.x.x, 0 - adjunct.x.y, 0 + adjunct.x.z),
		vec3f(0 - adjunct.y.x, 0 + adjunct.y.y, 0 - adjunct.y.z),
		vec3f(0 + adjunct.z.x, 0 - adjunct.z.y, 0 + adjunct.z.z)
	};
	adjunct = transpose(adjunct);
	return adjunct;
}

mat3f transpose(mat3f m) {
	mat3f transpose = mat3f(
		vec3f(m.x.x, m.y.x, m.z.x),
		vec3f(m.x.y, m.y.y, m.z.y),
		vec3f(m.x.z, m.y.z, m.z.z)
	);
	return transpose;
}
//////////////////////////////////////////////////
///
/// NaN / Inf TESTS FOR VECTORS
/// 
//////////////////////////////////////////////////
bool isnanvec2f(vec2f v)
{
	if (std::isnan(v.x) && std::isnan(v.y))
		return true;
	return false;
}
bool isinfvec2f(vec2f v)
{
	if (std::isinf(v.x) && std::isinf(v.y))
		return true;
	return false;
}
bool isnanvec4f(vec4f v)
{
	if (std::isnan(v.x) && std::isnan(v.y) && std::isnan(v.z) && std::isnan(v.w))
		return true;
	return false;
}
//////////////////////////////////////////////////
///
/// GENERAL LINEAR ALGEBRA HELPER FUNCTIONS
///
//////////////////////////////////////////////////
bool FloatEquals(float f, float v) {
	if (std::abs(f - v) < 0.00001f)
		return true;
	else
		return false;
}
bool FloatGreaterEquals(float f, float v) {
	if (FloatEquals(f, v) || f > v)
		return true;
	else
		return false;
}
bool FloatSmallerEquals(float f, float v) {
	if (FloatEquals(f, v) || f < v)
		return true;
	else
		return false;
}

vec2f Line2f::GetDir() {
	return end - start;
}
float Line2f::Len() {
	return length(GetDir());
}

bool LinearDependence(vec2f v, vec2f w)
{
	if ((v.x == 0 && w.x == 0) || (v.y == 0 && w.y == 0))
		return true;
	else if (w.x != 0 && w.y != 0 && v.x / w.x == v.y / w.y)
		return true;
	return false;
}
bool LinearDependence(vec3f v, vec3f w)
{
	if ((v.x == 0 && w.x == 0) || (v.y == 0 && w.y == 0))
		return true;
	else if (w.x != 0 && w.y != 0 && v.x / w.x == v.y / w.y)
		return true;
	return false;
}

vec2f SolveLSE(mat3f LSE)
{
	if (determinant(mat2f(vec2f(LSE.x.x, LSE.x.y), vec2f(LSE.y.x, LSE.y.y))) != 0.0f)
		return inverse(mat2f(vec2f(LSE.x.x, LSE.x.y), vec2f(LSE.y.x, LSE.y.y))) * vec2f(LSE.x.z, LSE.y.z);
	else
		return vec2f(NAN);
}

vec2f LineIntersection(Line2f line_1, Line2f line_2)
{

	if (LinearDependence(line_1.GetDir(), line_2.GetDir())) {
		if (!isnanvec2f(SolveLSE(mat3f(
			vec3f(line_1.start.x - line_2.start.x, line_1.GetDir().x, 0.0f),
			vec3f(line_1.start.y - line_2.start.y, line_1.GetDir().y, 0.0f),
			vec3f()))))
			return vec2f(NAN); // lines are parallel
		else
			return vec2f(INFINITY); // lines are identical
	}

	vec2f solutions = SolveLSE(mat3f(
		vec3f(line_1.GetDir().x, -line_2.GetDir().x, line_2.start.x - line_1.start.x),
		vec3f(line_1.GetDir().y, -line_2.GetDir().y, line_2.start.y - line_1.start.y),
		vec3f()));
	if (isnanvec2f(solutions))
		return vec2f(NAN);


	vec2f intersection = line_1.start + solutions.x * line_1.GetDir();

	bool cond_1 = (FloatGreaterEquals(intersection.x, std::min(line_1.start.x, line_1.end.x)) && FloatSmallerEquals(intersection.x, std::max(line_1.start.x, line_1.end.x)));
	bool cond_2 = (FloatGreaterEquals(intersection.y, std::min(line_1.start.y, line_1.end.y)) && FloatSmallerEquals(intersection.y, std::max(line_1.start.y, line_1.end.y)));
	bool cond_3 = (FloatGreaterEquals(intersection.x, std::min(line_2.start.x, line_2.end.x)) && FloatSmallerEquals(intersection.x, std::max(line_2.start.x, line_2.end.x)));
	bool cond_4 = (FloatGreaterEquals(intersection.y, std::min(line_2.start.y, line_2.end.y)) && FloatSmallerEquals(intersection.y, std::max(line_2.start.y, line_2.end.y)));

	if (cond_1 && cond_2 && cond_3 && cond_4)
		return intersection;
	else
		return vec2f(NAN);
}

bool PointInRectangle(vec2f point, vec2f rect_topleft, vec2f rect_bottomright) {
	if (FloatGreaterEquals(point.x, rect_topleft.x) && FloatSmallerEquals(point.x, rect_bottomright.x) &&
		FloatGreaterEquals(point.y, rect_bottomright.y) && FloatSmallerEquals(point.y, rect_topleft.y))
		return true;
	else
		return false;
}
bool PointInPolygon(vec2f point, Polygon2f vertices) {
	/// not implemented yet
	return false;
}
//////////////////////////////////////////////////
float rad(float degr) {
	return degr * (M_PI / 180.0f);
}
float degr(float rad) {
	return rad * (180.0f / M_PI);
}
//////////////////////////////////////////////////
///
/// PRINT FUNCTIONS USING 'std::cout'
/// 
//////////////////////////////////////////////////
void print(vec2f v) { std::cout << "\n ( " << v.x << " " << v.y << " )\n"; }
void print(vec3f v) { std::cout << "\n ( " << v.x << " " << v.y << " " << v.z << " )\n"; }
void print(vec4f v) { std::cout << "\n ( " << v.x << " " << v.y << " " << v.z << " " << v.w << " )\n"; }
void print(mat2f m) {
	std::cout << "[ " << m.x.x << " " << m.x.y << " ]\n"
		<< "[ " << m.y.x << " " << m.y.y << " ]\n";
}
void print(mat3f m) {
	std::cout << "\n [ " << m.x.x << " " << m.x.y << " " << m.x.z << " ]\n"
		<< "[ " << m.y.x << " " << m.y.y << " " << m.y.z << " ]\n"
		<< "[ " << m.z.x << " " << m.z.y << " " << m.z.z << " ]\n";
}
void print(mat4f m) {
	std::cout << "\n [ " << m.x.x << " " << m.x.y << " " << m.x.z << " " << m.x.w << " ]\n"
		<< "[ " << m.y.x << " " << m.y.y << " " << m.y.z << " " << m.y.w << " ]\n"
		<< "[ " << m.z.x << " " << m.z.y << " " << m.z.z << " " << m.z.w << " ]\n"
		<< "[ " << m.w.x << " " << m.w.y << " " << m.z.w << " " << m.w.w << " ]\n";
}
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////