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
	// data�������в����еķ���ֵ
	float data[N_WAVELENGTHS];
};

struct Material
{
	SpectralDistribution color_diffuse;//����������
	SpectralDistribution color_specular;//������ɫ����
	float reflectance; //�����
	float specular_reflectance; //���淴������
	float transmissivity; //͸����
	float refraction_index; //����ֵ����
	float diffuse_roughness; //����ֵ

	//--Ԥ�ȶ���õĲ�������--
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
	Material material; //���������Ĳ���
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
	Material material; // ������ཻ������Ĳ���
	glm::vec3 normal; // ������ཻ���������ķ�����
	float t; // �����ཻǰ�����ľ���
};

struct LightSourceIntersectionData
{
	SpectralDistribution radiosity; // ��Դ�ķ����
	float area; // ��Դ�����
	glm::vec3 normal; // ������ཻ���������ķ�����
	float t; // �����ཻǰ�����ľ���
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