#include "common.h"
#include "matrix.h"

FLOAT operator *(const Vec &a, const Vec &b)
{
	assert(a.n==b.n);
	FLOAT ret=0;
	rep(i,0,a.n-1) ret+=a.a[i]*b.a[i];
	return ret;
}

Vec operator *(const Vec &a, FLOAT b)
{
	Vec ret(a.n);
	rep(i,0,a.n-1) ret[i]=a.a[i]*b;
	return ret;
}

Vec operator +(const Vec &a, const Vec &b)
{
	assert(a.n==b.n);
	Vec ret(a.n);
	rep(i,0,a.n-1) ret[i]=a.a[i]+b.a[i];
	return ret;
}

Vec operator -(const Vec &a, const Vec &b)
{
	assert(a.n==b.n);
	Vec ret(a.n);
	rep(i,0,a.n-1) ret[i]=a.a[i]-b.a[i];
	return ret;
}

Vec operator *(const Mat &a, const Vec &b)
{
	assert(a.m==b.n);
	Vec ret(a.n);
	rept(it,a.values) ret.a[it->x]+=it->z*b.a[it->y];
	return ret;
}

Vec operator *(const Vec &a, const Mat &b)
{
	assert(a.n==b.n);
	Vec ret(b.m);
	rept(it,b.values) ret.a[it->y]+=it->z*a.a[it->x];
	return ret;
}

FLOAT Vec::norm() const
{
	return mysqrt((*this)*(*this));
} 

