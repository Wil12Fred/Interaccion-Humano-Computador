#pragma once
// Iostream - STD I/O Library
#include <iostream>
// Vector - STD Vector/Array Library
#include <vector>
// String - STD String Library
#include <string>
// fStream - STD File I/O Library
#include <fstream>
// Math.h - STD math Library
#include <math.h>
// Print progress to console while loading (large models)
#define OBJL_CONSOLE_OUTPUT

// Namespace: OBJL
//
// Description: The namespace that holds eveyrthing that
//	is needed and used for the OBJ Model Loader
namespace objl
{

	// Structure: Vector3
	//
	// Description: A 3D Vector that Holds Positional Data
	struct Vector3
	{
		// Default Constructor
		Vector3()
		{
			X = 0.0f;
			Y = 0.0f;
			Z = 0.0f;
		}
		// Variable Set Constructor
		Vector3(float X_, float Y_, float Z_)
		{
			X = X_;
			Y = Y_;
			Z = Z_;
		}
		// Bool Equals Operator Overload
		bool operator==(const Vector3& other) const
		{
			return (this->X == other.X && this->Y == other.Y && this->Z == other.Z);
		}
		// Bool Not Equals Operator Overload
		bool operator!=(const Vector3& other) const
		{
			return !(this->X == other.X && this->Y == other.Y && this->Z == other.Z);
		}
		// Addition Operator Overload
		Vector3 operator+(const Vector3& right) const
		{
			return Vector3(this->X + right.X, this->Y + right.Y, this->Z + right.Z);
		}
		// Subtraction Operator Overload
		Vector3 operator-(const Vector3& right) const
		{
			return Vector3(this->X - right.X, this->Y - right.Y, this->Z - right.Z);
		}
		// Float Multiplication Operator Overload
		Vector3 operator*(const float& other) const
		{
			return Vector3(this->X * other, this->Y * other, this->Z * other);
		}
		// Float Multiplication Operator Overload
		double operator*(const Vector3& other) const
		{
			return (double(this->X) * other.X+ double(this->Y) * other.Y+ double(this->Z) * other.Z);
		}
		// Float Division Operator Overload
		Vector3 operator/(const float& other) const
		{
			return Vector3(this->X / other, this->Y / other, this->Z / other);
		}
		Vector3 min(const Vector3& right) const
		{
			return Vector3(std::min(this->X, right.X), std::min(this->Y,right.Y), std::min(this->Z, right.Z));
		}
		Vector3 max(const Vector3& right) const
		{
			return Vector3(std::max(this->X, right.X), std::max(this->Y,right.Y), std::max(this->Z, right.Z));
		}
		double norm() const
		{
			return sqrt((*this)*(*this));
		}
		// Positional Variables
		float X;
		float Y;
		float Z;
	};
}
