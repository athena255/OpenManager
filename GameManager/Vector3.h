#pragma once
// Vector3 API 
#include <sstream>
class GAMEMANAGER_API Vector3
{
public:
	virtual float& x() = 0;
	virtual float& y() = 0;
	virtual float& z() = 0;


    inline bool is_valid()
    {
        return std::isfinite(x()) && std::isfinite(y()) && std::isfinite(z());
    }
	inline float& operator[](int i)
	{
		return ((float*)this)[i];
	}

	inline bool operator==(Vector3& src)
	{
		return (src.x() == x()) && (src.y() == y()) && (src.z() == z());
	}
    inline bool operator!=(Vector3& src)
    {
        return (src.x() != x()) || (src.y() != y()) || (src.z() != z());
    }
    inline void Zero()
    {
        x() = y() = z() = 0.0f;
    }
    inline Vector3& operator+=(Vector3& v)
    {
        x() += v.x(); y() += v.y(); z() += v.z();
        return *this;
    }
    inline Vector3& operator-=(Vector3& v)
    {
        x() -= v.x(); y() -= v.y(); z() -= v.z();
        return *this;
    }
    inline Vector3& operator*=(float fl)
    {
        x() *= fl;
        y() *= fl;
        z() *= fl;
        return *this;
    }
    inline Vector3& operator*=(Vector3& v)
    {
        x() *= v.x();
        y() *= v.y();
        z() *= v.z();
        return *this;
    }
    inline Vector3& operator/=(Vector3& v)
    {
        x() /= v.x();
        y() /= v.y();
        z() /= v.z();
        return *this;
    }
    inline Vector3& operator+=(float fl)
    {
        x() += fl;
        y() += fl;
        z() += fl;
        return *this;
    }
    inline Vector3& operator/=(float fl)
    {
        x() /= fl;
        y() /= fl;
        z() /= fl;
        return *this;
    }
    inline Vector3& operator-=(float fl)
    {
        x() -= fl;
        y() -= fl;
        z() -= fl;
        return *this;
    }

    inline float Dot(Vector3& vOther)
    {
        return (x() * vOther.x() + y() * vOther.y() + z() * vOther.z());
    }
    inline float Length()
    {
        return sqrt(x() * x() + y() * y() + z() * z());
    }
    inline float LengthSqr(void)
    {
        return (x() * x() + y() * y() + z() * z());
    }
    inline float Length2D()
    {
        return sqrt(x() * x() + y() * y());
    }
    Vector3& Normalized()
    {
        float l = this->Length();
        if (l != 0.0f) {
            *this /= l;
        }
        else {
            Zero();
        }
        return *this;
    }

    float DistTo(Vector3& vOther, Vector3& vOutDelta)
    {
        vOutDelta.x() = x() - vOther.x();
        vOutDelta.y() = y() - vOther.y();
        vOutDelta.z() = z() - vOther.z();

        return vOutDelta.Length();
    }

    float DistToSqr(Vector3& vOther, Vector3& vOutDelta)
    {
        vOutDelta.x() = x() - vOther.x();
        vOutDelta.y() = y() - vOther.y();
        vOutDelta.z() = z() - vOther.z();

        return vOutDelta.LengthSqr();
    }
};