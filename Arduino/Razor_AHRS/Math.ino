/* This file is part of the Razor AHRS Firmware */
//==============================================//

// Computes the dot product of two vectors
float Vector_Dot_Product(const float v1[3], const float v2[3])
{
  float result = 0;
  
  for(int c = 0; c < 3; c++)
  {
    result += v1[c] * v2[c];
  }
  
  return result; 
}

// Computes the cross product of two vectors
// out has to different from v1 and v2 (no in-place)!
void Vector_Cross_Product(float out[3], const float v1[3], const float v2[3])
{
  out[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
  out[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
  out[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
}

// Multiply the vector by a scalar
void Vector_Scale(float out[3], const float v[3], float scale)
{
  for(int c = 0; c < 3; c++)
  {
    out[c] = v[c] * scale; 
  }
}

// Adds two vectors
void Vector_Add(float out[3], const float v1[3], const float v2[3])
{
  for(int c = 0; c < 3; c++)
  {
    out[c] = v1[c] + v2[c];
  }
}

// Multiply two 3x3 matrices: out = a * b (float a, float b, float out)
// out has to different from a and b (no in-place)!
void Matrix_Multiply(const float a[3][3], const float b[3][3], float out[3][3])	//override
{
  for(int x = 0; x < 3; x++)  // rows
    for(int y = 0; y < 3; y++)  // columns
      out[x][y] = a[x][0] * b[0][y] + a[x][1] * b[1][y] + a[x][2] * b[2][y];
}

//------------------------------------------------------------------------------
// U=s*b, (b = -1 or 0 or 1)
float U(float s, char b)
{
	if     (b == 0) return( 0);
	else if (b < 0) return(-s);
	else						return( s);
}
//------------------------------------------------------------------------------
// Multiply two 3x3 matrices: out = a * b (float a, char b, float out)
// out has to different from a and b (no in-place)!
void Matrix_Multiply33(const float a[3][3], const char b[3][3], float out[3][3])	//override
{
  for(int x = 0; x < 3; x++)
		for(int y = 0; y < 3; y++)
			out[x][y] = U(a[x][0],b[0][y]) + U(a[x][1],b[1][y]) + U(a[x][2],b[2][y]);
//			out[x][y] = (a[x][0]*b[0][y]) + (a[x][1]*b[1][y]) + (a[x][2]*b[2][y]);
}
//------------------------------------------------------------------------------
//Sets matrix M to Identity Matrix
void IdentityMatrix(float M[3][3])
{
  for(int x=0; x<3; x++) for(int y=0; y<3; y++)
	{
		if (x == y) M[x][y] = 1.0f;
		else				M[x][y] = 0.0f;
	}
}
//------------------------------------------------------------------------------
float Len(float V[3])
{
	return (sqrt(V[0]*V[0] + V[1]*V[1] + V[2]*V[2]));
}
//------------------------------------------------------------------------------
float	Weight(const float V[3])
{
//	float R = 1.0f - 2.0f * abs(1.0f - Len(V));
	float R = 1.0f - 2.0f * abs(1.0f - (sqrt(V[0]*V[0] + V[1]*V[1] + V[2]*V[2])));
  if (R < 0.0) R = 0.0;
	return R;
}
//------------------------------------------------------------------------------
// Multiply 3x3 matrix with vector: a * b = out
// out has to different from b (no in-place)!
void Matrix_Vector_Multiply(const float a[3][3], const float b[3], float out[3])
{
  for(int x = 0; x < 3; x++)
  {
    out[x] = a[x][0] * b[0] + a[x][1] * b[1] + a[x][2] * b[2];
  }
}

// Init rotation matrix using euler angles
void init_rotation_matrix(float m[3][3], float yaw, float pitch, float roll)
{
  float c1 = cos(roll);
  float s1 = sin(roll);
  float c2 = cos(pitch);
  float s2 = sin(pitch);
  float c3 = cos(yaw);
  float s3 = sin(yaw);

  // Euler angles, right-handed, intrinsic, XYZ convention
  // (which means: rotate around body axes Z, Y', X'') 
  m[0][0] = c2 * c3;
  m[0][1] = c3 * s1 * s2 - c1 * s3;
  m[0][2] = s1 * s3 + c1 * c3 * s2;

  m[1][0] = c2 * s3;
  m[1][1] = c1 * c3 + s1 * s2 * s3;
  m[1][2] = c1 * s2 * s3 - c3 * s1;

  m[2][0] = -s2;
  m[2][1] = c2 * s1;
  m[2][2] = c1 * c2;
}
