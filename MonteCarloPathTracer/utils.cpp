#include <string>
#include <iostream>

#include "utils.h"

#define M_PI       3.14159265358979323846

const float Photon::RADIUS = 0.1;

SpectralDistribution::SpectralDistribution()
{
	for (int i = 0; i < N_WAVELENGTHS; ++i)
	{
		data[i] = 0;
	}
}

float SpectralDistribution::norm() const
{
	float sum = 0;
	for (int i = 0; i < N_WAVELENGTHS; ++i)
	{
		sum += data[i];
	}
	return sum / N_WAVELENGTHS;
}

std::ostream& operator<<(std::ostream& os, const SpectralDistribution& sd)
{
	os << "[ ";
	for (int i = 0; i < sd.N_WAVELENGTHS - 1; ++i)
	{
		os << sd.data[i] << ", ";
	}
	os << sd.data[sd.N_WAVELENGTHS - 1] << "]";
	return os;
}

SpectralDistribution operator*(float f, const SpectralDistribution& sd)
{
	return sd * f;
}

float& SpectralDistribution::operator[](const int i)
{
	return data[i];
}

SpectralDistribution SpectralDistribution::operator+(const SpectralDistribution& sd) const
{
	SpectralDistribution to_return;
	for (int i = 0; i < N_WAVELENGTHS; ++i)
	{
		to_return.data[i] = data[i] + sd.data[i];
	}
	return to_return;
}

SpectralDistribution SpectralDistribution::operator-(const SpectralDistribution& sd) const
{
	SpectralDistribution to_return;
	for (int i = 0; i < N_WAVELENGTHS; ++i)
	{
		to_return.data[i] = data[i] - sd.data[i];
	}
	return to_return;
}

SpectralDistribution SpectralDistribution::operator^(const float& f) const
{
	SpectralDistribution to_return;
	for (int i = 0; i < N_WAVELENGTHS; ++i)
	{
		to_return.data[i] = pow(data[i], f);
	}
	return to_return;
}

SpectralDistribution SpectralDistribution::operator/(const float& f) const
{
	SpectralDistribution to_return;
	for (int i = 0; i < N_WAVELENGTHS; ++i)
	{
		to_return.data[i] = data[i] / f;
	}
	return to_return;
}

SpectralDistribution SpectralDistribution::operator*(const float& f) const
{
	SpectralDistribution to_return;
	for (int i = 0; i < N_WAVELENGTHS; ++i)
	{
		to_return.data[i] = data[i] * f;
	}
	return to_return;
}

SpectralDistribution SpectralDistribution::operator*(const SpectralDistribution& sd) const
{
	SpectralDistribution to_return;
	for (int i = 0; i < N_WAVELENGTHS; ++i)
	{
		to_return.data[i] = data[i] * sd.data[i];
	}
	return to_return;
}


SpectralDistribution SpectralDistribution::operator+=(const SpectralDistribution& sd)
{
	for (int i = 0; i < N_WAVELENGTHS; ++i)
	{
		data[i] = data[i] + sd.data[i];
	}
	return *this;
}

SpectralDistribution SpectralDistribution::operator-=(const SpectralDistribution& sd)
{
	for (int i = 0; i < N_WAVELENGTHS; ++i)
	{
		data[i] = data[i] - sd.data[i];
	}
	return *this;
}

SpectralDistribution SpectralDistribution::operator*=(const SpectralDistribution& sd)
{
	for (int i = 0; i < N_WAVELENGTHS; ++i)
	{
		data[i] = data[i] * sd.data[i];
	}
	return *this;
}


SpectralDistribution SpectralDistribution::operator/=(const float& f)
{
	for (int i = 0; i < N_WAVELENGTHS; ++i)
	{
		data[i] = data[i] / f;
	}
	return *this;
}

SpectralDistribution SpectralDistribution::operator*=(const float& f)
{
	for (int i = 0; i < N_WAVELENGTHS; ++i)
	{
		data[i] = data[i] * f;
	}
	return *this;
}

Material Material::air()
{
	Material air;
	air.transmissivity = 1;
	air.refraction_index = 1;
	return air;
}

Material Material::mirror()
{
	Material mirror;
	//mirror.color_diffuse[0] =0.2;
	//mirror.color_diffuse[1] =0.2;
	//mirror.color_diffuse[2] =0.8;
	mirror.color_diffuse[0] =1;
	mirror.color_diffuse[1] =1;
	mirror.color_diffuse[2] =1;
	//mirror.color_specular[0] =0.6;
	//mirror.color_specular[1] =0.9;
	//mirror.color_specular[2] =1.0;
	mirror.color_specular[0] =1;
	mirror.color_specular[1] =1;
	mirror.color_specular[2] =1;

	mirror.reflectance =1;
	mirror.specular_reflectance = 1; 
	mirror.transmissivity = 0;
	mirror.refraction_index = 1;
	mirror.diffuse_roughness = 0;
	return mirror;
}

Material Material::glass()
{
	Material glass;
	glass.color_diffuse[0] =1;
	glass.color_diffuse[1] =1;
	glass.color_diffuse[2] =1;
	glass.color_specular[0] =1;
	glass.color_specular[1] =1;
	glass.color_specular[2] =1;

	glass.reflectance =1;
	glass.specular_reflectance = 1; 
	glass.transmissivity = 1;
	glass.refraction_index = 1.6;
	glass.diffuse_roughness = 0;
	return glass;
}

Material Material::colored_glass()
{
	Material colored_glass;
	colored_glass.color_diffuse[0] =0.5;
	colored_glass.color_diffuse[1] =1;
	colored_glass.color_diffuse[2] =0.7;
	colored_glass.color_specular[0] =1;
	colored_glass.color_specular[1] =1;
	colored_glass.color_specular[2] =1;

	colored_glass.reflectance =1;
	colored_glass.specular_reflectance = 1; 
	colored_glass.transmissivity = 1;
	colored_glass.refraction_index = 1.6;
	colored_glass.diffuse_roughness = 0;
	return colored_glass;
}

Material Material::diffuse_gray()
{
	Material diffuse_gray;
	diffuse_gray.color_diffuse[0] =0.5;
	diffuse_gray.color_diffuse[1] =0.5;
	diffuse_gray.color_diffuse[2] =0.5;
	diffuse_gray.color_specular[0] =1;
	diffuse_gray.color_specular[1] =1;
	diffuse_gray.color_specular[2] =1;

	diffuse_gray.reflectance =0.8;
	diffuse_gray.specular_reflectance = 0; 
	diffuse_gray.transmissivity = 0;
	diffuse_gray.refraction_index = 1;
	diffuse_gray.diffuse_roughness = 0;
	return diffuse_gray;
}

Material Material::diffuse_red()
{
	Material diffuse_gray;
	diffuse_gray.color_diffuse[0] =1;
	diffuse_gray.color_diffuse[1] =0.2;
	diffuse_gray.color_diffuse[2] =0.2;
	diffuse_gray.color_specular[0] =1;
	diffuse_gray.color_specular[1] =1;
	diffuse_gray.color_specular[2] =1;

	diffuse_gray.reflectance =0.8;
	diffuse_gray.specular_reflectance = 0; 
	diffuse_gray.transmissivity = 0;
	diffuse_gray.refraction_index = 1;
	diffuse_gray.diffuse_roughness = 0;
	return diffuse_gray;
}

Material Material::diffuse_green()
{
	Material diffuse_gray;
	diffuse_gray.color_diffuse[0] =0.2;
	diffuse_gray.color_diffuse[1] =1;
	diffuse_gray.color_diffuse[2] =0.2;
	diffuse_gray.color_specular[0] =1;
	diffuse_gray.color_specular[1] =1;
	diffuse_gray.color_specular[2] =1;

	diffuse_gray.reflectance =0.8;
	diffuse_gray.specular_reflectance = 0; 
	diffuse_gray.transmissivity = 0;
	diffuse_gray.refraction_index = 1;
	diffuse_gray.diffuse_roughness = 0;
	return diffuse_gray;
}

Material Material::diffuse_blue()
{
	Material diffuse_gray;
	diffuse_gray.color_diffuse[0] =0.6;
	diffuse_gray.color_diffuse[1] =0.6;
	diffuse_gray.color_diffuse[2] =1;
	diffuse_gray.color_specular[0] =1;
	diffuse_gray.color_specular[1] =1;
	diffuse_gray.color_specular[2] =1;

	diffuse_gray.reflectance =0.8;
	diffuse_gray.specular_reflectance = 0; 
	diffuse_gray.transmissivity = 0;
	diffuse_gray.refraction_index = 1;
	diffuse_gray.diffuse_roughness = 0;
	return diffuse_gray;
}

Material Material::diffuse_white()
{
	Material diffuse_gray;
	diffuse_gray.color_diffuse[0] =1;
	diffuse_gray.color_diffuse[1] =1;
	diffuse_gray.color_diffuse[2] =1;
	diffuse_gray.color_specular[0] =1;
	diffuse_gray.color_specular[1] =1;
	diffuse_gray.color_specular[2] =1;

	diffuse_gray.reflectance =0.8;
	diffuse_gray.specular_reflectance = 0; 
	diffuse_gray.transmissivity = 0;
	diffuse_gray.refraction_index = 1;
	diffuse_gray.diffuse_roughness = 0;
	return diffuse_gray;
}

Material Material::diffuse_cyan()//À¶ÂÌÉ«
{
	Material diffuse_gray;
	diffuse_gray.color_diffuse[0] =0.5;
	diffuse_gray.color_diffuse[1] =0.8;
	diffuse_gray.color_diffuse[2] =0.8;
	diffuse_gray.color_specular[0] =1;
	diffuse_gray.color_specular[1] =1;
	diffuse_gray.color_specular[2] =1;

	diffuse_gray.reflectance =0.8;
	diffuse_gray.specular_reflectance = 0; 
	diffuse_gray.transmissivity = 0;
	diffuse_gray.refraction_index = 1;
	diffuse_gray.diffuse_roughness = 0;
	return diffuse_gray;
}

SpectralDistribution evaluateLambertianBRDF(
	glm::vec3 d1,
	glm::vec3 d2,
	glm::vec3 normal,
	SpectralDistribution albedo)
{
	return albedo / M_PI;
}

SpectralDistribution evaluatePerfectBRDF(
	SpectralDistribution albedo)
{
	return albedo;
}

SpectralDistribution evaluateOrenNayarBRDF(
	glm::vec3 d1,
	glm::vec3 d2,
	glm::vec3 normal,
	SpectralDistribution albedo,
	float roughness)
{
	float sigma2 = roughness * roughness;
	float A = 1 - 0.5 * sigma2 / (sigma2 + 0.57);
	float B = 0.45 * sigma2 / (sigma2 + 0.09);
	float cos_theta_d1 = glm::dot(d1, normal);
	float cos_theta_d2 = glm::dot(d2, normal);
	float theta = glm::acos(cos_theta_d2);
	float theta_d1 = glm::acos(cos_theta_d1);
	float alpha = glm::max(theta, theta_d1);
	float beta = glm::min(theta, theta_d1);
	float cos_d1_d2 = glm::dot(d1, d2);

	return
	albedo / M_PI *
	(A + (B * glm::max(0.0f, cos_d1_d2)) * glm::sin(alpha) * glm::tan(beta));
}