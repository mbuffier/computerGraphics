#ifndef operateur_H
#define operateur_H


#include <memory.h>

#define SQUARE(number) (number*number)
#define PI 3.141593 
#define DEG_TO_RAD(deg) ((float)(deg)*PI/180)

// Classes which define vector3f, matrix16 and matrix9 with their operateur

class vector3f
{
public:

	vector3f()
	{	memset(vertex, 0, sizeof(float[3]));	}

	vector3f(float x, float y, float z);

	~vector3f();

	float vertex[3];

	void LoadZero(void);
	float Length(void);
	float LengthSquared(void);

	vector3f& operator= (const vector3f &v1);

	friend bool operator== (const vector3f &v1, const vector3f &v2);
	friend bool operator<= (const vector3f &v1, const vector3f &v2);
	friend bool operator>= (const vector3f &v1, const vector3f &v2);

	friend bool operator< (const vector3f &v1, const vector3f &v2);
	friend bool operator> (const vector3f &v1, const vector3f &v2);

//	friend vector3f operator= (const vector3f &v1, const vector3f &v2);
	friend vector3f operator+ (const vector3f &v1, const vector3f &v2);
//	friend vector3f operator+= (const vector3f &v1);
	friend vector3f operator+ (const vector3f &v1, const float scalar);
	friend vector3f operator- (const vector3f &v1, const vector3f &v2);
	friend vector3f operator- (const vector3f &v1, const float scalar);
	friend vector3f operator* (const vector3f &v1, const vector3f &v2);
	friend vector3f operator* (const vector3f &v1, const float scalar);
	friend vector3f operator/ (const vector3f &v1, const vector3f &v2);
	friend vector3f operator/ (const vector3f &v1, const float scalar);

	vector3f Abs(void);

	friend float dot(const vector3f &v1, const vector3f &v2);
	friend vector3f Cross(const vector3f &v1, const vector3f &v2);

	//void Eq(vector3f *other);
	//float* Return4f();

	void Set(float x,float y,float z);
};

class matrix16f
{
public:
	float matrix[16];

	void LoadZero(void);
	void LoadIdentity(void);

	void print(); 

	void Translate(float x, float y, float z);
	void Scale(float x, float y, float z);

	void RotateX(int deg);
	void RotateY(int deg);
	void RotateZ(int deg);

	void RotateX(float rad);
	void RotateY(float rad);
	void RotateZ(float rad);

	void RotateXYZ(float x, float y, float z);
	void Rotate(matrix16f m1);
	
	void Set(vector3f right, vector3f up, vector3f out);
	
	//void Eq(float m[16]);
	matrix16f& operator= (const matrix16f &m1);

	
	vector3f Transform(vector3f point);
	matrix16f FlipOrientation(void);

	void Normalize(void);
	//matrix16f Invert(void);

	friend matrix16f operator+ (const matrix16f &m1, const matrix16f &m2);
	/// add vector to 12 13 14 portion of matrix
	friend matrix16f operator+ (const matrix16f &m1, const vector3f  &v2);
	friend matrix16f operator- (const matrix16f &m1, const matrix16f &m2);
	friend matrix16f operator* (const matrix16f &m1, const float scalar);
	friend matrix16f operator* (const matrix16f &m1, const matrix16f &m2);
	friend matrix16f operator/ (const matrix16f &m1, const float scalar);
	friend bool operator== (const matrix16f &m1, const matrix16f &m2);

	matrix16f()
		{	memset(matrix, 0, sizeof(float[16]));	}
};

class matrix9f
{
public:
	float matrix[9];

	matrix9f()
	{	memset(matrix, 0, sizeof(float[9]));	}

	void LoadZero(void);
	void LoadIdentity(void);
	
	
	void RotateX(int deg);
	void RotateY(int deg);
	void RotateZ(int deg);

	matrix9f& operator= (const matrix9f &m1);


	//vector3f Transform(vector3f point);
	matrix9f Transpose(void);
	matrix9f Inverse(void);
	//matrix16f Invert(void);

	friend matrix9f Star(vector3f a);


	friend matrix9f operator+ (const matrix9f &m1, const matrix9f &m2);
	friend matrix16f operator+ (const matrix16f &m1, const matrix9f &m2);
	friend matrix9f operator- (const matrix9f &m1, const matrix9f &m2);
	friend matrix9f operator* (const matrix9f &m1, const float scalar);
	friend matrix9f operator* (const matrix9f &m1, const matrix9f &m2);
	friend vector3f operator* (const matrix9f &m1, const vector3f &v2);

	friend matrix9f operator* (const matrix9f &m1, const matrix16f &m2);
	friend matrix9f operator* (const matrix16f &m1, const matrix9f &m2);

	friend matrix9f operator/ (const matrix9f &m1, const float scalar);
	friend bool operator== (const matrix9f &m1, const matrix9f &m2);
};


float det(float f0, float f1, float f2, float f3);

//float dot(vector3f &v1, vector3f &v2);
vector3f Cross(const vector3f &v1, const vector3f &v2);

typedef struct _triangleV {
	vector3f *vertNorm[3];
	vector3f *vertice[3];	
	vector3f norm;			// triangle norm
} triangleV;

vector3f findNorm(int index, int numSurfTriangles, vector3f **surfTriangles);

#endif

