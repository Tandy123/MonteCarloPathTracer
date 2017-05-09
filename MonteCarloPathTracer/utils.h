#ifndef UTILS_H
#define UTILS_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class SpectralDistribution
{
public:
	SpectralDistribution();
	~SpectralDistribution(){};

	float norm() const;

	friend std::ostream& operator<<(
		std::ostream& os,
		const SpectralDistribution& sd);
	friend SpectralDistribution operator*(
		float f,
		const SpectralDistribution& sd);
	float& operator[](const int i);
	SpectralDistribution operator+(const SpectralDistribution& sd) const;
	SpectralDistribution operator-(const SpectralDistribution& sd) const;
	SpectralDistribution operator^(const float& f) const;
	SpectralDistribution operator/(const float& f) const;
	SpectralDistribution operator*(const float& f) const;
	SpectralDistribution operator*(const SpectralDistribution& sd) const;
	SpectralDistribution operator+=(const SpectralDistribution& sd);
	SpectralDistribution operator-=(const SpectralDistribution& sd);
	SpectralDistribution operator*=(const SpectralDistribution& sd);
	SpectralDistribution operator/=(const float& f);
	SpectralDistribution operator*=(const float& f);

	static const int N_WAVELENGTHS = 3;
	static const int MIN_WAVELENGTH = 300;
	static const int MAX_WAVELENGTH = 700;
	// data包含所有波长中的辐射值
	float data[N_WAVELENGTHS];
};

struct Material
{
	SpectralDistribution color_diffuse;//漫反射描述
	SpectralDistribution color_specular;//反射颜色描述
	float reflectance; //反射比
	float specular_reflectance; //镜面反射描述
	float transmissivity; //透射率
	float refraction_index; //折射值描述
	float diffuse_roughness; //抖动值

	//--预先定义好的材质类型--
	static Material air();
	static Material mirror();
	static Material glass();
	static Material colored_glass();
	static Material diffuse_gray();
	static Material diffuse_red();
	static Material diffuse_green();
	static Material diffuse_blue();
	static Material diffuse_white();
	static Material diffuse_cyan();
};

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;
	Material material; //光线所处的材质
	SpectralDistribution radiance;
	bool has_intersected;
};

struct Photon
{
	glm::vec3 position;
	glm::vec3 direction_in;
	SpectralDistribution delta_flux;
	static const float RADIUS;
};

struct KDTreeNode
{
	typedef double value_type;
	Photon p;
	size_t index;

	value_type operator[](size_t n) const
	{
		return p.position[n];
	}

	double distance( const KDTreeNode &node)
	{
		return glm::length(node.p.position - p.position);
	}
};

struct IntersectionData
{
	Material material; // 与光线相交的物体的材质
	glm::vec3 normal; // 与光线相交的物体表面的法向量
	float t; // 光线相交前经过的距离
};

struct LightSourceIntersectionData
{
	SpectralDistribution radiosity; // 光源的辐射度
	float area; // 光源的面积
	glm::vec3 normal; // 与光线相交的物体表面的法向量
	float t; // 光线相交前经过的距离
};

SpectralDistribution evaluatePerfectBRDF(
	SpectralDistribution albedo);
SpectralDistribution evaluateLambertianBRDF(
	glm::vec3 d1,
	glm::vec3 d2,
	glm::vec3 normal,
	SpectralDistribution albedo);
SpectralDistribution evaluateOrenNayarBRDF(
	glm::vec3 d1,
	glm::vec3 d2,
	glm::vec3 normal,
	SpectralDistribution albedo,
	float roughness);


#endif // UTILS_H