#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "utils.h"

class Camera
{
public:
	glm::vec3 eye;//眼睛所在位置
	glm::vec3 center;//眼睛所看的位置
	glm::vec3 up;//朝上方向
	float fov;//视野的弧度
	glm::mat4 VP_inv;

	const int WIDTH;//屏幕宽
	const int HEIGHT;//屏幕高

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