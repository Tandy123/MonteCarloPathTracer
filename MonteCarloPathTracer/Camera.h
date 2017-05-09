#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "utils.h"

class Camera
{
public:
	glm::vec3 eye;//�۾�����λ��
	glm::vec3 center;//�۾�������λ��
	glm::vec3 up;//���Ϸ���
	float fov;//��Ұ�Ļ���
	glm::mat4 VP_inv;

	const int WIDTH;//��Ļ��
	const int HEIGHT;//��Ļ��

	Camera(
		const glm::vec3 eye,
		const glm::vec3 center,
		const glm::vec3 up,
		const float fov,
		const int width,
		const int height);
	~Camera(){};

	Ray castRay(
		const int pixel_x,
		const int pixel_y,
		const float parameter_x,
		const float parameter_y);

	int getWidth();
	int getHeight();
};

#endif // CAMERA_H