#pragma once

//Custom Vector class to assist with variable storage and vector math
struct Vec2 {
	float x, y;
	
	inline Vec2() {
		x = 0.0f;
		y = 0.0f;
	}

	inline Vec2(float x_, float y_) {
		x = x_;
		y = y_;
	}

	inline Vec2(const Vec2& vec_) {
		x = vec_.x;
		y = vec_.y;
	}

	inline const Vec2 operator + (const Vec2& vec_) const {
		return Vec2(x + vec_.x, y + vec_.y);
	}

	inline Vec2 operator += (const float value_) {
		x += value_;
		y += value_;

		return *this;
	}

	inline Vec2 operator += (const Vec2& vec_) {
		x += vec_.x;
		y += vec_.y;

		return *this;
	}

	inline const Vec2 operator - (const Vec2& vec_) const {
		return Vec2(x - vec_.x, y - vec_.y);
	}

	inline Vec2 operator -= (const float value_) {
		x -= value_;
		y -= value_;

		return *this;
	}

	inline Vec2 operator -= (const Vec2& vec_) {
		x -= vec_.x;
		y -= vec_.y;

		return *this;
	}

	inline const Vec2 operator * (const float value_) const {
		return Vec2(x * value_, y * value_);
	}

	inline Vec2 operator *= (const float value_) {
		x *= value_;
		y *= value_;

		return *this;
	}

	inline const Vec2 operator * (const Vec2& vec_) const {
		return Vec2(x * vec_.x, y * vec_.y);
	}

	inline Vec2 operator *= (const Vec2& vec_) {
		x *= vec_.x;
		y *= vec_.y;

		return *this;
	}

	inline const Vec2 operator / (const Vec2& vec_) const {
		Vec2 result;
		if (vec_.x != 0.0f) {
			result.x = x / vec_.x;
		}
		if (vec_.y != 0.0f) {
			result.y = y / vec_.y;
		}
		
		return result;
	}

	inline Vec2 operator /= (const Vec2& vec_) {
		if (vec_.x != 0.0f) x /= vec_.x;
		else x = 0.0f;

		if (vec_.y != 0.0f) y /= vec_.y;
		else y = 0.0f;

		return *this;
	}

	inline const Vec2 operator / (const float value_) const {
		if (value_ == 0.0f) {
			return Vec2();
		}

		return Vec2(x / value_, y / value_);
	}

	inline Vec2 operator /= (const float value_) {
		if (value_ == 0.0f) {
			return Vec2();
		}

		x /= value_;
		y /= value_;

		return *this;
	}

};

struct Vec3 : Vec2 {
	float z;

	inline Vec3() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	inline Vec3(float x_, float y_, float z_) {
		x = x_;
		y = y_;
		z = z_;
	}

	inline Vec3(const Vec3& vec_) {
		x = vec_.x;
		y = vec_.y;
		z = vec_.z;
	}

	inline const Vec3 operator + (const Vec3& vec_) const {
		return Vec3(x + vec_.x, y + vec_.y, z + vec_.z);
	}

	inline Vec3 operator += (const float value_) {
		x += value_;
		y += value_;
		z += value_;

		return *this;
	}

	inline Vec3 operator += (const Vec3& vec_) {
		x += vec_.x;
		y += vec_.y;
		z += vec_.z;

		return *this;
	}

	inline const Vec3 operator - (const Vec3& vec_) const {
		return Vec3(x - vec_.x, y - vec_.y, z - vec_.z);
	}

	inline Vec3 operator -= (const float value_) {
		x -= value_;
		y -= value_;
		z -= value_;

		return *this;
	}

	inline Vec3 operator -= (const Vec3& vec_) {
		x -= vec_.x;
		y -= vec_.y;
		z -= vec_.z;

		return *this;
	}

	inline const Vec3 operator * (const float value_) const {
		return Vec3(x * value_, y * value_, z * value_);
	}

	inline Vec3 operator *= (const float value_) {
		x *= value_;
		y *= value_;
		z *= value_;

		return *this;
	}

	inline const Vec3 operator / (const float value_) const {
		if (value_ == 0.0f) {
			return Vec3();
		}
		else {
			return Vec3(x / value_, y / value_, z / value_);
		}
	}

	inline Vec3 operator /= (const float value_) {
		if (value_ == 0.0f) {
			return Vec3();
		}
		else {
			x /= value_;
			y /= value_;
			z /= value_;

			return *this;
		}
	}
};

