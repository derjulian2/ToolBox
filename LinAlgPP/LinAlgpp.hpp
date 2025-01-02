//////////////////////////////////////////////////
/// LinAlg++ Library                           ///
///                                            ///
/// Provides Helper Classes and Functions for  ///
/// handling Linear Algebra Math in C++        ///
/// 2024 Julian Benzel | .hpp Header File      ///
//////////////////////////////////////////////////
#ifndef LINALGPP_LIB
#define LINALGPP_LIB
//////////////////////////////////////////////////
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>
//////////////////////////////////////////////////
/// 
/// CLASSES
/// 
//////////////////////////////////////////////////
/// TWO DIMENSIONAL VECTOR
//////////////////////////////////////////////////
/// <summary>
/// <para> <b> Two-Dimensional Vector </b> </para>
/// <para> Bundles two float numbers to x and y coordinates of a vector. </para>
/// </summary>
struct vec2f
{
	float x, y;

	vec2f();
	vec2f(float num);
	vec2f(float x, float y);
};
//////////////////////////////////////////////////
/// THREE DIMENSIONAL VECTOR
//////////////////////////////////////////////////
struct vec3f
{
	float x, y, z;


	vec3f();
	vec3f(float num);
	vec3f(float x, float y, float z);
	vec3f(vec2f v, float z);
};
//////////////////////////////////////////////////
/// FOUR DIMENSIONAL VECTOR
//////////////////////////////////////////////////
struct vec4f {
	float x, y, z, w;

	vec4f();
	vec4f(float num);
	vec4f(float x, float y, float z, float w);
};
//////////////////////////////////////////////////
/// 2 X 2 SQUARE MATRIX
//////////////////////////////////////////////////
struct mat2f {
	vec2f x, y;

	mat2f();
	mat2f(float x);
	mat2f(vec2f x, vec2f y);
};
//////////////////////////////////////////////////
/// 3 X 3 SQUARE MATRIX
//////////////////////////////////////////////////
struct mat3f {
	vec3f x, y, z;

	mat3f();
	mat3f(float x);
	mat3f(vec3f x, vec3f y, vec3f z);
};
//////////////////////////////////////////////////
/// 4 X 4 SQUARE MATRIX
//////////////////////////////////////////////////
struct mat4f {
	vec4f x, y, z, w;

	mat4f();
	mat4f(float x);
	mat4f(vec4f x, vec4f y, vec4f z, vec4f w);
};
//////////////////////////////////////////////////
/// LINE STRUCT
/// STRICTLY DEFINED BY START AND END POINT
/// THUS NOT INFINITELY LONG
//////////////////////////////////////////////////
struct Line2f {
	Line2f();
	Line2f(vec2f start, vec2f end);

	vec2f GetDir();
	float Len();

	vec2f start;
	vec2f end;
};
//////////////////////////////////////////////////
/// POLYGON
/// DEFINED BY VECTOR OF POINTS
//////////////////////////////////////////////////
typedef std::vector<vec2f> Polygon2f;
//////////////////////////////////////////////////
///
/// BASIC OPERATOR FUNCTIONALITY
/// 
//////////////////////////////////////////////////
// ADDITION
vec2f operator+(vec2f v, vec2f w);
vec3f operator+(vec3f v, vec3f w);
// SUBRACTION
vec2f operator-(vec2f v, vec2f w);
vec3f operator-(vec3f v, vec3f w);
// MULTIPLICATION
vec2f operator*(vec2f v, vec2f w);
vec2f operator*(vec2f v, float f);
vec2f operator*(float f, vec2f v);

vec3f operator*(vec3f v, vec3f w);
vec3f operator*(vec3f v, float f);
vec3f operator*(float f, vec3f v);

vec4f operator*(vec4f v, vec4f w);
vec4f operator*(vec4f v, float f);
vec4f operator*(float f, vec4f v);

mat2f operator*(float f, mat2f m);
mat2f operator*(mat2f m, float f);
vec2f operator*(mat2f m, vec2f v);
mat2f operator*(mat2f m, mat2f w);

mat3f operator*(mat3f m, float f);
mat3f operator*(float f, mat3f m);
vec3f operator*(mat3f m, vec3f v);
mat3f operator*(mat3f m, mat3f w);
// COMPARISON
bool operator==(vec2f v, vec2f w);
bool operator!=(vec2f v, vec2f w);
bool operator> (vec2f v, vec2f w);
bool operator>=(vec2f v, vec2f w);
bool operator< (vec2f v, vec2f w);
bool operator<=(vec2f v, vec2f w);

bool operator==(vec3f v, vec3f w);
bool operator!=(vec3f v, vec3f w);
bool operator> (vec3f v, vec3f w);
bool operator>=(vec3f v, vec3f w);
bool operator< (vec3f v, vec3f w);
bool operator<=(vec3f v, vec3f w);

bool operator==(vec4f v, vec4f w);
bool operator!=(vec4f v, vec4f w);
bool operator> (vec4f v, vec4f w);
bool operator>=(vec4f v, vec4f w);
bool operator< (vec4f v, vec4f w);
bool operator<=(vec4f v, vec4f w);
//////////////////////////////////////////////////
///
/// ADVANCED VECTOR OPERATIONS
/// 
//////////////////////////////////////////////////
/// VECTOR LENGTH
float length(vec2f v);
float length(vec3f v);
float length(vec4f v);
/// DOT PRODUCT
float dot(vec2f v, vec2f w);
float dot(vec3f v, vec3f w);
float dot(vec4f v, vec4f w);
/// CROSS PRODUCT
vec3f cross(vec3f v, vec3f w);
/// UNIT VECTOR
vec2f unit(vec2f v);
vec3f unit(vec3f v);
vec4f unit(vec4f v);
/// ANGLE BETWEEN VECTORS
/// <summary>
/// Returns the angle between two vectors in degrees.
/// </summary>
float angle(vec2f v, vec2f w);
float angle(vec3f v, vec3f w);
float angle(vec4f v, vec4f w);
//////////////////////////////////////////////////
///
/// ADVANCED MATRIX OPERATIONS
///
//////////////////////////////////////////////////
float determinant(mat2f m);
float determinant(mat3f m);

mat3f scale(mat3f m, vec2f s);
mat3f translate(mat3f m, vec2f t);

mat2f inverse(mat2f m);
mat3f inverse(mat3f m);

mat3f adjunct(mat3f m);
mat3f transpose(mat3f m);
//////////////////////////////////////////////////
///
/// NaN / Inf TESTS FOR VECTORS
/// 
//////////////////////////////////////////////////
bool isnanvec2f(vec2f v);
bool isinfvec2f(vec2f v); 
bool isnanvec4f(vec4f v);
//////////////////////////////////////////////////
///
/// GENERAL LINEAR ALGEBRA HELPER FUNCTIONS
///
//////////////////////////////////////////////////
bool FloatEquals(float f, float v);
bool FloatGreaterEquals(float f, float v);
bool FloatSmallerEquals(float f, float v);
/// <summary>
/// Analyzes passed vectors for linear dependence.
/// </summary>
/// <param name="v">: First vector.</param>
/// <param name="w">: Second vector.</param>
/// <returns>True if linearly dependent, false if not.</returns>
bool LinearDependence(vec2f v, vec2f w);

bool LinearDependence(vec3f v, vec3f w);

/// <summary>
/// <para> Solves linear system of equations of the form </para>
/// <para> |  I | a + b = c | </para>
/// <para> | II | d + e = f | </para>
/// <para> via matrix inversion. </para>
/// </summary>
/// <param name = "LSE">
/// : The LSE in matrix form (the bottom row will be ignored).
/// </param>
/// <returns>
/// <para> Two solutions in a vector if the LSE is solvable. </para>
/// NaN vector if there are no solutions.
/// </returns>
vec2f SolveLSE(mat3f LSE);

vec2f LineIntersection(Line2f line_1, Line2f line_2);

bool PointInRectangle(vec2f point, vec2f rect_topleft, vec2f rect_bottomright);
bool PointInPolygon(vec2f point, Polygon2f vertices);
//////////////////////////////////////////////////
///
/// TRIGONOMETRY FUNCTIONS
///
//////////////////////////////////////////////////
float rad(float degr);
float degr(float rad);
//////////////////////////////////////////////////
///
/// PRINT FUNCTIONS USING 'std::cout'
/// 
//////////////////////////////////////////////////
void print(vec2f v);
void print(vec3f v);
void print(vec4f v);
void print(mat2f m);
void print(mat3f m);
void print(mat4f m);
//////////////////////////////////////////////////
#endif
//////////////////////////////////////////////////
