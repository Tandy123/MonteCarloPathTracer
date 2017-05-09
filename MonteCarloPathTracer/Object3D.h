#ifndef OBJECT_3D
#define OBJECT_3D

#include "OctTreeAABB.h"

#include <vector>

#include <glm/glm.hpp>
#include "utils.h"

class Object3D
{
private:
	const Material* material_;
public:
	Object3D(Material* material);
	virtual ~Object3D(){};

	virtual bool 	intersect(IntersectionData* id, Ray r) const = 0;
	Material 		material() const;
};

class Mesh : public Object3D
{
private:
	std::vector<glm::vec3> positions_;//顶点坐标
	std::vector<glm::vec2> uvs_;//纹理坐标
	std::vector<glm::vec3> normals_;//法向量
	std::vector<unsigned int> indices_;//索引目录

	glm::mat4 transform_;//变换矩阵
	OctTreeAABB* ot_aabb_;//树结构

	friend class OctNodeAABB;
public:
	Mesh(glm::mat4 transform, 
		std::vector<glm::vec3> &tmp_positions,
		std::vector<glm::vec2> &tmp_uvs,
		std::vector<glm::vec3> &tmp_normals,
		Material * material);
	~Mesh(){ delete ot_aabb_; };

	virtual bool 	intersect(IntersectionData* id, Ray r) const;

	glm::vec3 		getMinPosition() const;
	glm::vec3 		getMaxPosition() const;
	glm::mat4		getTransform() const;
	int				getNumberOfTriangles() const;
};

class Sphere : public Object3D
{
private:
	const glm::vec3 POSITION_;
	const float RADIUS_;
public:
	Sphere(glm::vec3 position, float radius, Material* material);
	~Sphere(){};

	bool intersect(IntersectionData* id, Ray r) const;
	glm::vec3 getPointOnSurface(float u, float v) const;
};

// P0, P1, P2 定义一个平面平行四边形
//		P2——————P0
//		/			 /
//	   /				/
//	  P0——————P1
class Plane : public Object3D
{
private:
	const glm::vec3 P0_, P1_, P2_, V1_, V2_, NORMAL_;//顶点、边、法向量
	const float AREA_;//面积
public:
	Plane(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, Material* material);
	~Plane(){};

	bool 		intersect(IntersectionData* id, Ray r) const;
	glm::vec3 	getPointOnSurface(float u, float v) const;
	float 		getArea() const;
	glm::vec3 	getNormal() const;
	glm::vec3 	getFirstTangent() const;
};

class LightSource
{
private:
	const Plane emitter_;
public:
	LightSource(
		glm::vec3 p0,
		glm::vec3 p1,
		glm::vec3 p2,
		float flux,
		SpectralDistribution color);
	~LightSource(){};

	bool 		intersect(LightSourceIntersectionData* light_id, Ray r);
	glm::vec3 	getPointOnSurface(float u, float v);
	float 		getArea() const;
	glm::vec3 		getNormal() const;

	Ray shootLightRay();

	const SpectralDistribution radiosity; 
};

#endif