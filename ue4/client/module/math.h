#pragma once
#include <xmmintrin.h>
#include <immintrin.h>

#define	wsize	sizeof(u_int)
#define	wmask	(wsize - 1)

#ifdef BZERO
#define	RETURN	return
#define	VAL	0
#define	WIDEVAL	0
void
bzero( dst0, length )
void *dst0;
register size_t length;
#else
#define	RETURN	return (dst0)
#define	VAL	c0
#define	WIDEVAL	c5
#endif

#define UC(c)	((unsigned char)c)

namespace custom
{

	int toupper( int c )
	{
		if ( c >= 'a' && c <= 'z' ) return c - 'a' + 'A';
		return c;
	}

	int tolower( int c )
	{
		if ( c >= 'A' && c <= 'Z' ) return c - 'A' + 'a';
		return c;
	}

	void *memcpy( void *dest, const void *src, unsigned __int64 count )
	{
		char *char_dest = ( char * ) dest;
		char *char_src = ( char * ) src;
		if ( ( char_dest <= char_src ) || ( char_dest >= ( char_src + count ) ) )
		{
			while ( count > 0 )
			{
				*char_dest = *char_src;
				char_dest++;
				char_src++;
				count--;
			}
		}
		else
		{
			char_dest = ( char * ) dest + count - 1;
			char_src = ( char * ) src + count - 1;
			while ( count > 0 )
			{
				*char_dest = *char_src;
				char_dest--;
				char_src--;
				count--;
			}
		}
		return dest;
	}



	void *memccpy( void *to, const void *from, int c, unsigned __int64 count )
	{
		char t;
		unsigned __int64 i;
		char *dst = ( char * ) to;
		const char *src = ( const char * ) from;
		for ( i = 0; i < count; i++ )
		{
			dst[i] = t = src[i];
			if ( t == 0 ) break;
			if ( t == c ) return &dst[i + 1];
		}
		return 0;
	}

	void *memchr( const void *s, int c, unsigned __int64 n )
	{
		if ( n )
		{
			const char *p = ( const char * ) s;
			do
			{
				if ( *p++ == c ) return ( void * ) ( p - 1 );
			} while ( --n != 0 );
		}
		return 0;
	}

	int memcmp( const void *s1, const void *s2, unsigned __int64 n )
	{
		if ( n != 0 )
		{
			const unsigned char *p1 = ( unsigned char * ) s1, *p2 = ( unsigned char * ) s2;
			do
			{
				if ( *p1++ != *p2++ ) return ( *--p1 - *--p2 );
			} while ( --n != 0 );
		}
		return 0;
	}

	int memicmp( const void *s1, const void *s2, unsigned __int64 n )
	{
		if ( n != 0 )
		{
			const unsigned char *p1 = ( unsigned char * ) s1, *p2 = ( unsigned char * ) s2;
			do
			{
				if ( toupper( *p1 ) != toupper( *p2 ) ) return ( *p1 - *p2 );
				p1++;
				p2++;
			} while ( --n != 0 );
		}
		return 0;
	}

	void *memmove( void *dest, const void *src, unsigned __int64 count )
	{
		char *char_dest = ( char * ) dest;
		char *char_src = ( char * ) src;
		if ( ( char_dest <= char_src ) || ( char_dest >= ( char_src + count ) ) )
		{
			while ( count > 0 )
			{
				*char_dest = *char_src;
				char_dest++;
				char_src++;
				count--;
			}
		}
		else
		{
			char_dest = ( char * ) dest + count - 1;
			char_src = ( char * ) src + count - 1;
			while ( count > 0 )
			{
				*char_dest = *char_src;
				char_dest--;
				char_src--;
				count--;
			}
		}
		return dest;
	}

	char *_cslwr( char *x )
	{
		char *y = x;
		while ( *y )
		{
			*y = tolower( *y );
			y++;
		}
		return x;
	}

	char *_csupr( char *x )
	{
		char *y = x;
		while ( *y )
		{
			*y = tolower( *y );
			y++;
		}
		return x;
	}

	int strlen( const char *string )
	{
		int cnt = 0;
		if ( string )
		{
			for ( ; *string != 0; ++string ) ++cnt;
		}
		return cnt;
	}

	int wcslen( const wchar_t *string )
	{
		int cnt = 0;
		if ( string )
		{
			for ( ; *string != 0; ++string ) ++cnt;
		}
		return cnt;
	}

	const char *strcpy( char *buffer, const char *string )
	{
		char *ret = buffer;
		while ( *string ) *buffer++ = *string++;
		*buffer = 0;
		return ret;
	}

	int strcmp( const char *cs, const char *ct )
	{
		if ( cs && ct )
		{
			while ( *cs == *ct )
			{
				if ( *cs == 0 && *ct == 0 ) return 0;
				if ( *cs == 0 || *ct == 0 ) break;
				cs++;
				ct++;
			}
			return *cs - *ct;
		}
		return -1;
	}

	int stricmp( const char *cs, const char *ct )
	{
		if ( cs && ct )
		{
			while ( tolower( *cs ) == tolower( *ct ) )
			{
				if ( *cs == 0 && *ct == 0 ) return 0;
				if ( *cs == 0 || *ct == 0 ) break;
				cs++;
				ct++;
			}
			return tolower( *cs ) - tolower( *ct );
		}
		return -1;
	}



	float fabsf( float x )
	{
		{
			if ( x < 0 )
			{
				return -x;
			}
			return x;
		}
	}

	const char *strstr( const char *string, char *substring )
	{
		const char *a, *b;
		b = substring;
		if ( *b == 0 ) {
			return string;
		}
		for ( ; *string != 0; string += 1 ) {
			if ( *string != *b ) {
				continue;
			}
			a = string;
			while ( 1 ) {
				if ( *b == 0 ) {
					return string;
				}
				if ( *a++ != *b++ ) {
					break;
				}
			}
			b = substring;
		}
		return NULL;
	}


	int _wcsicmp( const wchar_t *string1, const wchar_t *string2 )
	{
		wchar_t c1, c2;
		do
		{
			c1 = *string1++;
			c2 = *string2++;
			if ( c2 == L'\0' )
				return c1 - c2;
		} while ( c1 == c2 );
		return c1 < c2 ? -1 : 1;
	}

	size_t strnlen( const char *s, size_t maxlen )
	{
		size_t i;
		for ( i = 0; i < maxlen; ++i )
			if ( s[i] == '\0' )
				break;
		return i;
	}


	double floor( double num )
	{
		if ( num < 0 )
			return ( int ) num - 1;
		else
			return ( int ) num;
	}

	int isspace( int c )
	{
		return ( c == '\t' || c == '\n' ||
			c == '\v' || c == '\f' || c == '\r' || c == ' ' ? 1 : 0 );
	}

	int isdigit( int c )
	{
		return ( c >= '0' && c <= '9' ? 1 : 0 );
	}

	int isalpha( int c )
	{
		return ( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) ? 1 : 0 );
	}

	char isupper( unsigned char c )
	{
		if ( c >= UC( 'A' ) && c <= UC( 'Z' ) )
			return 1;
		return 0;
	}


	inline int wcompare( const wchar_t *X, const wchar_t *Y )
	{
		while ( *X && *Y ) {
			if ( *X != *Y ) {
				return 0;
			}
			X++;
			Y++;
		}

		return ( *Y == L'\0' );
	}

	inline const wchar_t *wcsstr( const wchar_t *X, const wchar_t *Y )
	{
		while ( *X != L'\0' ) {
			if ( ( *X == *Y ) && wcompare( X, Y ) ) {
				return X;
			}
			X++;
		}
		return NULL;
	}


	inline float sqrtf( float number )
	{
		float temp, mysqrt;
		mysqrt = number / 2;
		temp = 0;
		while ( mysqrt != temp ) {
			temp = mysqrt;
			mysqrt = ( number / temp + temp ) / 2;
		}
		return mysqrt;
	}


	float powf( float _X, float _Y ) {
		return _mm_cvtss_f32( _mm_pow_ps( _mm_set_ss( _X ), _mm_set_ss( _Y ) ) );
	}
}
typedef struct _D3DMATRIX2 {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};
} D3DMATRIX2;


class Vector3
{
public:
	float x, y, z;

	__forceinline Vector3( ) {
		x = y = z = 0.0f;
	}

	__forceinline Vector3( float X, float Y, float Z ) {
		x = X; y = Y; z = Z;
	}

	__forceinline float operator[]( int i ) const {
		return ( ( float * ) this )[i];
	}

	__forceinline Vector3 &operator+=( float v ) {
		x += v; y += v; z += v; return *this;
	}

	__forceinline Vector3 &operator-=( float v ) {
		x -= v; y -= v; z -= v; return *this;
	}

	__forceinline Vector3 &operator-=( const Vector3 &v ) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}

	__forceinline Vector3 operator*( float v ) const {
		return Vector3( x * v, y * v, z * v );
	}

	__forceinline double Length() {
		return sqrtf(x * x + y * y + z * z);
		//float dist = UE4::Utilities::SpoofCall(sqrtf, dx * dx + dy * dy + dz * dz) / 100.0f;
	}

	__forceinline Vector3 operator/( float v ) const {
		return Vector3( x / v, y / v, z / v );
	}

	__forceinline Vector3 &operator+=( const Vector3 &v ) {
		x += v.x; y += v.y; z += v.z; return *this;
	}

	__forceinline Vector3 operator-( const Vector3 &v ) const {
		return Vector3( x - v.x, y - v.y, z - v.z );
	}

	__forceinline Vector3 operator+( const Vector3 &v ) const {
		return Vector3( x + v.x, y + v.y, z + v.z );
	}

	__forceinline Vector3 &operator/=( float v ) {
		x /= v; y /= v; z /= v; return *this;
	}

	__forceinline bool Zero( ) const {
		return ( x > -0.1f && x < 0.1f && y > -0.1f && y < 0.1f && z > -0.1f && z < 0.1f );
	}

	__forceinline float Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}
};

class Vector2
{
public:
	float x, y;

	__forceinline Vector2( ) {
		x = y = 0.0f;
	}

	__forceinline Vector2( float X, float Y ) {
		x = X; y = Y;
	}

	__forceinline Vector2 operator+( float v ) const {
		return Vector2( x + v, y + v );
	}

	__forceinline Vector2 operator-( float v ) const {
		return Vector2( x - v, y - v );
	}

	__forceinline Vector2 &operator+=( float v ) {
		x += v; y += v; return *this;
	}

	__forceinline Vector2 &operator*=( float v ) {
		x *= v; y *= v; return *this;
	}

	__forceinline Vector2 &operator/=( float v ) {
		x /= v; y /= v; return *this;
	}

	__forceinline Vector2 operator-( const Vector2 &v ) const {
		return Vector2( x - v.x, y - v.y );
	}

	__forceinline Vector2 operator+( const Vector2 &v ) const {
		return Vector2( x + v.x, y + v.y );
	}

	__forceinline Vector2 &operator+=( const Vector2 &v ) {
		x += v.x; y += v.y; return *this;
	}

	__forceinline Vector2 &operator-=( const Vector2 &v ) {
		x -= v.x; y -= v.y; return *this;
	}

	__forceinline Vector2 operator/( float v ) const {
		return Vector2( x / v, y / v );
	}

	__forceinline Vector2 operator*( float v ) const {
		return Vector2( x * v, y * v );
	}

	__forceinline Vector2 operator/( const Vector2 &v ) const {
		return Vector2( x / v.x, y / v.y );
	}

	__forceinline bool Zero( ) const {
		return ( x > -0.1f && x < 0.1f && y > -0.1f && y < 0.1f );
	}
};


class Vector4
{
public:
	Vector4() : x(0.f), y(0.f), w(0.f), h(0.f)
	{

	}

	Vector4(float _x, float _y, float _w, float _h) : x(_x), y(_y), w(_w), h(_h)
	{

	}
	~Vector4()
	{

	}

	float x;
	float y;
	float w;
	float h;

	inline float Dot(Vector4 v)
	{
		return x * v.x + y * v.y + w * v.w + h * v.h;
	}

	inline float Distance(Vector4 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.w - w, 2.0) + powf(v.h - h, 2.0)));
	}
	inline float Length()
	{
		return sqrtf(x * x + y * y + w * w + h * h);
	}
	Vector4 operator+(Vector4 v)
	{
		return Vector4(x + v.x, y + v.y, w + v.w, h + v.h);
	}

	Vector4 operator-(Vector4 v)
	{
		return Vector4(x - v.x, y - v.y, w - v.w, h - v.h);
	}
};

class Matrix3x4
{
public:
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
		};

		float m[3][4];
		float mm[12];
	};
};

class Matrix4x4
{
public:
	float m[4][4];
};


struct FQuat
{
	float x;
	float y;
	float z;
	float w;
};
_D3DMATRIX2 MatrixMultiplication(_D3DMATRIX2 pM1, _D3DMATRIX2 pM2)
{
	_D3DMATRIX2 pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}

struct FTransform
{
	FQuat rot;
	Vector3 translation;
	char pad[4];
	Vector3 scale;
	char pad1[4];

	_D3DMATRIX2 ToMatrixWithScale()
	{
		_D3DMATRIX2 m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};

namespace Math
{
#define M_PI 3.14159265358979323846f
#define M_RADPI	57.295779513082f
#define M_PI_F ((float)(M_PI))
#define RAD2DEG(x) ((float)(x) * (float)(180.f / M_PI_F))
#define DEG2RAD(x) ((float)(x) * (float)(M_PI_F / 180.f))

	//#define sinf(a) ((float)Module(ntdll, sin, (double)(a)))
#define cosf(a) ((float)Module(ucrtbase, cosf, (float)(a)))
#define atanf(a) ((float)Module(ucrtbase, atanf, (float)(a)))
#define atan2f(a, b) ((float)Module(ucrtbase, atan2f, (float)(a), (float)(b)))

#define powf(a, b) ((a) * (a))
//#define F(A) (A)

/*float acosf(float x) {
	float negate = float(x < 0);
	x = abs(x);
	float ret = -0.0187293;
	ret = ret * x;
	ret = ret + 0.0742610;
	ret = ret * x;
	ret = ret - 0.2121144;
	ret = ret * x;
	ret = ret + 1.5707288;
	ret = ret * sqrt(1.0 - x);
	ret = ret - 2 * negate * ret;
	return negate * 3.14159265358979 + ret;
}*/

	__forceinline float FastSQRT( float x ) {
		union
		{
			int i; float x;
		} u;
		u.x = x; u.i = ( u.i >> 1 ) + 0x1FC00000;
		u.x = u.x + x / u.x;
		return .25f * u.x + x / u.x;
	}

	__forceinline float FastDist2D( const Vector2 &Src, const Vector2 &Dst ) {
		return FastSQRT( powf( Src.x - Dst.x, 2.f ) + powf( Src.y - Dst.y, 2.f ) );
	}

	__forceinline float FastDist3D( const Vector3 &Src, const Vector3 &Dst ) {
		return FastSQRT( powf( Src.x - Dst.x, 2.f ) + powf( Src.y - Dst.y, 2.f ) + powf( Src.z - Dst.z, 2.f ) );
	}

	__forceinline float GameDist( const Vector3 &Src, const Vector3 &Dst ) {
		return FastDist3D( Src, Dst ) * 0.01905f;
	}

	__forceinline float Dist3D( const Vector3 &Src, const Vector3 &Dst ) {
		return custom::sqrtf( powf( Src.x - Dst.x, 2.f ) + powf( Src.y - Dst.y, 2.f ) + powf( Src.z - Dst.z, 2.f ) );
	}

	__forceinline float Vec3Length( const Vector3 &Src ) {
		return custom::sqrtf( powf( Src.x, 2.f ) + powf( Src.y, 2.f ) + powf( Src.z, 2.f ) );
	}

	__forceinline void ClampAngle( Vector3 &Ang ) {
		if ( Ang.x < -89.f ) Ang.x = -89.f;
		if ( Ang.x > 89.f ) Ang.x = 89.f;
		while ( Ang.y < -180.f ) Ang.y += 360.f;
		while ( Ang.y > 180.f ) Ang.y -= 360.f;
		Ang.z = 0.f;
	}

	/*__forceinline float atan2(float y, float x)
	{
		float t0, t1, t3, t4;

		t3 = abs(x);
		t1 = abs(y);
		t0 = max(t3, t1);
		t1 = min(t3, t1);
		t3 = F(1.f) / t0;
		t3 = t1 * t3;

		t4 = t3 * t3;
		t0 = F(-0.013480470f);
		t0 = t0 * t4 + F(0.057477314f);
		t0 = t0 * t4 - F(0.121239071f);
		t0 = t0 * t4 + F(0.195635925f);
		t0 = t0 * t4 - F(0.332994597f);
		t0 = t0 * t4 + F(0.999995630f);
		t3 = t0 * t3;

		t3 = (abs(y) > abs(x)) ? F(1.570796327f) - t3 : t3;
		t3 = (x < 0) ? M_PI - t3 : t3;
		t3 = (y < 0) ? -t3 : t3;

		return t3;
	}

	float atan(float x) {
		return atan2(x, float(1));
	}*/

	Vector3 CalcAngle( const Vector3 &Src, const Vector3 &Dst )
	{
		Vector3 RetVector = { 0,0,0 };
		Vector3 VectorPos = Dst - Src;
		float distance = VectorPos.Length();
		RetVector.x = -(((float)acosf((VectorPos.z / distance)) * (float)(180.0f / M_PI)) - 90.f);
		RetVector.y = (float)atan2f((VectorPos.y), (VectorPos.x)) * (float)(180.0f / M_PI); //atan2 og
		return RetVector;
	}

	__forceinline void VectorNormalize( Vector3 &Src )
	{
		float l = Vec3Length( Src );
		if ( l != 0.0f )
		{
			Src /= l;
		}
		else
		{
			Src.x = Src.y = 0.0f; Src.z = 1.0f;
		}
	}

	/*float acos(float x) {
		float negate = float(x < 0);
		x = abs(x);
		float ret = -0.0187293;
		ret = ret * x;
		ret = ret + 0.0742610;
		ret = ret * x;
		ret = ret - 0.2121144;
		ret = ret * x;
		ret = ret + 1.5707288;
		ret = ret * sqrt(1.0 - x);
		ret = ret - 2 * negate * ret;
		return negate * 3.14159265358979 + ret;
	}

	float sin(float x)
	{
		const float a0 = +1.91059300966915117e-31f;
		const float a1 = +1.00086760103908896f;
		const float a2 = -1.21276126894734565e-2f;
		const float a3 = -1.38078780785773762e-1f;
		const float a4 = -2.67353392911981221e-2f;
		const float a5 = +2.08026600266304389e-2f;
		const float a6 = -3.03996055049204407e-3f;
		const float a7 = +1.38235642404333740e-4f;
		return a0 + x * (a1 + x * (a2 + x * (a3 + x * (a4 + x * (a5 + x * (a6 + x * a7))))));
	}*/

	/*__forceinline float CosAdd(float x) {
		float x2 = x * x;
		const float c1 = 0.99940307f;
		const float c2 = -0.49558072f;
		const float c3 = 0.03679168f;
		return (c1 + x2 * (c2 + c3 * x2));
	}

	float cos(float angle) {
		angle = angle - floorf(angle * 0.15f) * 6.28f;
		angle = angle > 0.f ? angle : -angle;
		if (angle < 1.57f) return CosAdd(angle);
		if (angle < 3.14f) return -CosAdd(3.14f - angle);
		if (angle < 4.71f) return -CosAdd(angle - 3.14f);
		return CosAdd(6.28f - angle);
	}*/

	void MakeVector( Vector3 angle, Vector3 &vector )
	{
		

			float pitch = float( angle[0] * M_PI / 180 );
		float yaw = float( angle[1] * M_PI / 180 );
		float tmp = float( cosf( pitch ) );
		vector.x = float( -tmp * -cosf( yaw ) );
		vector.y = float( sinf( yaw ) * tmp );
		vector.z = float( -sinf( pitch ) );

		
	}

	__forceinline float Dot( const Vector3 &v1, const Vector3 &v2 ) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	float GetFov( const Vector2 &ScrCenter, const Vector2 &ScreenPos/*const Vector3& viewAngle, const Vector3& aimAngle*/ )
	{
		return Math::FastDist2D( ScrCenter, ScreenPos );

		//

		//Vector3 ang, aim; MakeVector(viewAngle, aim); MakeVector(aimAngle, ang);
		//float mag = (float)FastSQRT(powf(aim.x, 2) + powf(aim.y, 2) + powf(aim.z, 2));
		//float u_dot_v = Dot(aim, ang);
		//auto fov = RAD2DEG(acosf(u_dot_v / (pow(mag, 2))));

		//
		//return ((fov < 0) ? 0.f : fov); //fix pls
	}
}

Vector3 SmoothAngles(Vector3 rot1, Vector3 rot2)
{
	Vector3 ret;
	auto currentRotation = rot1;
	ret.x = (rot2.x - rot1.x) / 7.0f + rot1.x;
	ret.y = (rot2.y - rot1.y) / 7.0f + rot1.y;
	return ret;
}

struct FVector2D
{
	float X;
	float Y;

	FVector2D();
	FVector2D(float _X, float _Y);

	float GetX();
	float GetY();

	float Size();
};


struct FVector
{
	float X;
	float Y;
	float Z;

	FVector();
	FVector(float _X, float _Y, float _Z);

	FVector operator+ (const FVector& A);
	FVector operator+ (const float A);
	FVector operator+= (const FVector& A);
	FVector operator+= (const float A);
	FVector operator- (const FVector& A);
	FVector operator- (const float A);
	FVector operator-= (const FVector& A);
	FVector operator-= (const float A);
	FVector operator* (const FVector& A);
	FVector operator* (const float A);
	FVector operator*= (const FVector& A);
	FVector operator*= (const float A);
	FVector operator/ (const FVector& A);
	FVector operator/ (const float A);
	FVector operator/= (const FVector& A);
	FVector operator/= (const float A);

	bool operator== (const FVector& A);
	bool operator!= (const FVector& A);

	float GetX();
	float GetY();
	float GetZ();

	float Size();
	//float Dot (const FVector V1,const FVector V2);

	/*char* toString ()
	{
		static char buffer[ 256 ];
		ZeroMemory( buffer, 256 );
		sprintf( buffer, "vector( %.2f, %.2f, %.2f )", this->X, this->Y, this->Z );
		return buffer;
	};*/
	float		Distance(FVector v);
	float		Dot(FVector pStart, FVector pEnd);
	bool		InAngle(FVector pStart, FVector pEnd, FVector pRotation, int nAngle);
	int			round(float fValue);
	//static	float		GetDistance(centity_t* pEntity, FVector Location);
	//static	bool		WorldToScreen(FVector vec_Location, FVector& vec_Return);

	bool IsValid();
};

float FVector::Dot(FVector pStart, FVector pEnd)
{
	return (pStart.X * pEnd.X) + (pStart.Y * pEnd.Y) + (pStart.Z * pEnd.Z);
}
float FVector::Distance(FVector v)
{
	return float(sqrtf(powf(v.X - X, 2.0) + powf(v.Y - Y, 2.0) + powf(v.Z - Z, 2.0)));
}
bool FVector::InAngle(FVector pStart, FVector pEnd, FVector pRotation, int nAngle)
{
	return (Dot((pStart - pEnd) / (pStart - pEnd).Size(), pRotation) > cos(nAngle * M_PI / 360));
}

int FVector::round(float fValue)
{
	return int(fValue + 0.5f);
}


bool FVector::IsValid()
{
	return *this != FVector(0, 0, 0);
}

FVector2D::FVector2D()
{
	this->X = 0.0f;
	this->Y = 0.0f;
}

FVector2D::FVector2D(float _X, float _Y)
{
	this->X = _X;
	this->Y = _Y;
}

float FVector2D::GetX()
{
	return this->X;
}

float FVector2D::GetY()
{
	return this->Y;
}

float FVector2D::Size()
{
	return sqrt((this->X * this->X) + (this->Y * this->Y));
}


FVector::FVector()
{
	this->X = 0.0f;
	this->Y = 0.0f;
	this->Z = 0.0f;
}

FVector::FVector(float _X, float _Y, float _Z)
{
	this->X = _X;
	this->Y = _Y;
	this->Z = _Z;
}

FVector FVector::operator+ (const FVector& A)
{
	return FVector(this->X + A.X, this->Y + A.Y, this->Z + A.Z);
}

FVector FVector::operator+ (const float A)
{
	return FVector(this->X + A, this->Y + A, this->Z + A);
}

FVector FVector::operator+= (const FVector& A)
{
	this->X += A.X;
	this->Y += A.Y;
	this->Z += A.Z;
	return *this;
}

FVector FVector::operator+= (const float A)
{
	this->X += A;
	this->Y += A;
	this->Z += A;
	return *this;
}

FVector FVector::operator- (const FVector& A)
{
	return FVector(this->X - A.X, this->Y - A.Y, this->Z - A.Z);
}

FVector FVector::operator- (const float A)
{
	return FVector(this->X - A, this->Y - A, this->Z - A);
}

FVector FVector::operator-= (const FVector& A)
{
	this->X -= A.X;
	this->Y -= A.Y;
	this->Z -= A.Z;
	return *this;
}

FVector FVector::operator-= (const float A)
{
	this->X -= A;
	this->Y -= A;
	this->Z -= A;
	return *this;
}

FVector FVector::operator* (const FVector& A)
{
	return FVector(this->X * A.X, this->Y * A.Y, this->Z * A.Z);
}

FVector FVector::operator* (const float A)
{
	return FVector(this->X * A, this->Y * A, this->Z * A);
}

FVector FVector::operator*= (const FVector& A)
{
	this->X *= A.X;
	this->Y *= A.Y;
	this->Z *= A.Z;
	return *this;
}

FVector FVector::operator*= (const float A)
{
	this->X *= A;
	this->Y *= A;
	this->Z *= A;
	return *this;
}

FVector FVector::operator/ (const FVector& A)
{
	return FVector(this->X / A.X, this->Y / A.Y, this->Z / A.Z);
}

FVector FVector::operator/ (const float A)
{
	return FVector(this->X / A, this->Y / A, this->Z / A);
}

FVector FVector::operator/= (const FVector& A)
{
	this->X /= A.X;
	this->Y /= A.Y;
	this->Z /= A.Z;
	return *this;
}

FVector FVector::operator/= (const float A)
{
	this->X /= A;
	this->Y /= A;
	this->Z /= A;
	return *this;
}

bool FVector::operator== (const FVector& A)
{
	if (this->X == A.X
		&& this->Y == A.Y
		&& this->Z == A.Z)
		return true;

	return false;
}

bool FVector::operator!= (const FVector& A)
{
	if (this->X != A.X
		|| this->Y != A.Y
		|| this->Z != A.Z)
		return true;

	return false;
}

float FVector::GetX()
{
	return this->X;
}

float FVector::GetY()
{
	return this->Y;
}

float FVector::GetZ()
{
	return this->Z;
}

float FVector::Size()
{
	return sqrt((this->X * this->X) + (this->Y * this->Y) + (this->Z * this->Z));
}

namespace UE4
{
	struct FMinimalViewInfo
	{
		struct Vector3 Location; // 0x00(0x0C)
		struct Vector3 Rotation; // 0x0С(0x0C)
		float FOV; // 0x18(0x04)
		float DesiredFOV; // 0x1C(0x04)

		float UsableFOV()
		{
			return this->DesiredFOV + 9.f;
		}
	};

	struct FCameraCacheEntry
	{
		float Timestamp; // 0x00(0x04)
		char pad_4[0xc]; // 0x04(0x0c)
		FMinimalViewInfo POV; // 0x10(0x5e0)
	};

	const char* GetObjectFromFName(const int a1, uintptr_t Base)
	{
		DWORD chunkOffset = ((int)(a1) >> 16); // Block
		WORD nameOffset = (WORD)a1;
		auto namePoolChunk = *(uintptr_t*)(Base + 0x55F7FC0 + ((chunkOffset + 2) * 8));
		auto entryOffset = namePoolChunk + (DWORD)(2 * nameOffset);
		INT16 nameLength = *(INT16*)(entryOffset) >> 6;
		if (nameLength > 256)nameLength = 255;
		char cBuf[256] = {};
		RtlCopyMemory(&cBuf, (void*)(entryOffset + 2), nameLength);
		cBuf[nameLength] = 0;
		return cBuf;
	}

	void ReadMemory(const void* address, void* buffer, size_t size)
	{
		DWORD back = NULL;

		DWORD instruction = 0x04;

		if (VirtualProtect((LPVOID)address, size, instruction, &back))
		{
			memcpy(buffer, address, size);

			VirtualProtect((LPVOID)address, size, back, &back);
		}
	}

	std::wstring ReadUnicode(uint64_t address)
	{
		wchar_t buffer[1024 * sizeof(wchar_t)];

		ReadMemory((const void*)(address), &buffer, 1024 * sizeof(wchar_t));

		return std::wstring(buffer);
	}


	namespace UBone
	{
		FTransform GetBoneIndex(uint64_t Mesh, int Bone)
		{
			DWORD_PTR bonearray = *(DWORD_PTR*)(Mesh + 0x4B0);
			if (!bonearray)
				bonearray = *(DWORD_PTR*)(Mesh + 0x4B0 + 0x10);

			if (!bonearray)
				return FTransform();


			return *(FTransform*)(bonearray + (Bone * 0x30));
		}

		Vector3 GetBoneWithRotation(uint64_t Mesh, int id)
		{
			FTransform bone = GetBoneIndex(Mesh, id);
			FTransform ComponentToWorld = *(FTransform*)(Mesh + 0x1c0);
			_D3DMATRIX2 Matrix;
			Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
			return Vector3(Matrix._41, Matrix._42, Matrix._43);
		}

	}
}

namespace FString
{
	auto Convert(const char* ObjectNameA)
	{
		static wchar_t PlayerNameW[128];
		for (int a = 0, u = 0;;)
		{
			if (ObjectNameA[a] == '\\' && ObjectNameA[a + 1] == 'u') {
				PlayerNameW[u++] = DecodeUTF(&ObjectNameA[a + 2]);
				a += 6;
			}

			else {
				PlayerNameW[u++] = ObjectNameA[a];
				if (!ObjectNameA[a]) {
					break;
				} a++;
			}
		}

		return PlayerNameW;
	}
}

struct FRotator {
	float Pitch;
	float Yaw;
	float Roll;
};