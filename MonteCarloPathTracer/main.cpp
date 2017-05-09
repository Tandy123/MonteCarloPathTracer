#pragma comment( lib, "glut32.lib")

#include <stdio.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <GL/glut.h>

#include <omp.h>

#include "lodepng.h"
#include "glmOBJ.h"

#include "Camera.h"
#include "Scene.h"

#define	WINDOW_WIDTH	640
#define	WINDOW_HEIGHT	640
#define	M_PI       3.14159265358979323846

GLubyte buffer[WINDOW_HEIGHT][WINDOW_WIDTH][3]; //绘制缓冲器


void init(void)
{    
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void display()
{
	glClear( GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);

	glDrawPixels(WINDOW_WIDTH,WINDOW_HEIGHT, GL_RGB, 
		GL_UNSIGNED_BYTE, buffer);
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void savePNG(
	const std::string file_path,
	const int width,
	const int height,
	unsigned char* data) 
{
	std::vector<unsigned char> pixel_buffer;

	int pixel_count = width*height;

	for (int i=0; i<pixel_count; i++) {
		pixel_buffer.push_back(data[i*3+0]);
		pixel_buffer.push_back(data[i*3+1]);
		pixel_buffer.push_back(data[i*3+2]);
		pixel_buffer.push_back(255);
	}

	unsigned error = lodepng::encode(file_path, pixel_buffer, width, height);
	if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;

	pixel_buffer.clear();
}

// 获取当前时间，并以“年_月_日_时_分_秒”的格式存为字符串
const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y_%m_%d_%H_%M_%S", &tstruct);

	return buf;
}

void main(int argc, char *argv[])
{
	

	int SUB_SAMPLING_CAUSTICS = 1;
	int SUB_SAMPLING_MONTE_CARLO = 50;
	int SUB_SAMPLING_DIRECT_SPECULAR = 10;
	int NUMBER_OF_PHOTONS_EMISSION = 200000;


	Camera c(
		glm::vec3(0, 0, 3),	//相机位置
		glm::vec3(0, 0, 0),	//视线指向位置
		glm::vec3(0, 1, 0),	//朝上方向
		M_PI / 4,			//视野的弧度
		WINDOW_WIDTH,			//屏幕宽
		WINDOW_HEIGHT);		//屏幕高
	glm::vec3 camera_plane_normal = glm::normalize(c.center - c.eye);//相机平面法向

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
	glutInitWindowPosition(150, 150);
	glutCreateWindow( "Monte Carlo Path Tracer 汤德衍 21521073" );


	char a1c_fileName[256];

	SceneType scene_type;

	while(1){
		int func_num;
		std::cout<<"******************************"<<std::endl;
		std::cout<<"请输入数字进行对应操作：\n1.导入场景1（scene01.obj）\n2.导入场景2（scene02.obj）\n3.自己创建的场景\n4.重绘场景1\n5.结束程序"<<std::endl;
		std::cin>>func_num;
		if(func_num ==1){
			strcpy(a1c_fileName,"..\\data\\scene01.obj");
			scene_type = SCENE01;
			break;
		}
		else if(func_num ==2){
			strcpy(a1c_fileName,"..\\data\\scene02.obj");
			scene_type = SCENE02;
			break;
		}
		else if(func_num ==3){
			strcpy(a1c_fileName,"..\\data\\bunny.obj");
			scene_type = MYSCENE;
			break;
		}
		else if(func_num ==4){
			strcpy(a1c_fileName,"..\\data\\scene01.obj");
			scene_type = SCENE01_RE;
			break;
		}
		//else if(func_num ==5){
		//	strcpy(a1c_fileName,"..\\data\\scene02.obj");
		//	scene_type = SCENE02_RE;
		//	break;
		//}
		else if(func_num ==5){
			exit(0);
		}
		else{
			std::cout<<"请输入对应数字！" <<std::endl;
		}
	}
	time_t time_start, time_now, rendertime_start;
	time(&time_start);

	std::cout<<"——载入文件:" <<a1c_fileName<<" 请稍等."<<std::endl;
	Scene s(a1c_fileName, scene_type);

	SpectralDistribution* irradiance_values = new SpectralDistribution[c.WIDTH * c.HEIGHT];

	unsigned char* pixel_values =
		new unsigned char[c.WIDTH * c.HEIGHT * 3];

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-0.5, 0.5);//生成-0.5到0.5之间的随机数

	s.buildPhotonMap(NUMBER_OF_PHOTONS_EMISSION);

	std::cout << "——构造树完成,开始绘制!" << std::endl;
	time(&rendertime_start);
	double prerender_time = difftime(rendertime_start, time_start);

#pragma omp parallel for schedule(dynamic, 1)       //利用openMP并行执行for循环
	for (int x = 0; x < c.WIDTH; ++x)
	{
		float rendering_percent_finished = (x+1) * 100 / float(c.WIDTH);
		time(&time_now);
		double rendering_time_elapsed = difftime(time_now, rendertime_start);
		double rendering_time_left = (rendering_time_elapsed / rendering_percent_finished) *
			(100 - rendering_percent_finished);

		double total_time_elapsed = prerender_time + rendering_time_elapsed;
		double total_time_estimate = total_time_elapsed + rendering_time_left;
		double total_time_left = total_time_estimate - total_time_elapsed;

		int hours = total_time_left / (60 * 60);
		int minutes = (int(total_time_left) % (60 * 60)) / 60;
		int seconds = int(total_time_left) % 60;

		fprintf(stderr, "\r——绘制进程 %.2f%%.预计剩余时间为 %d h:%d m:%d s.",rendering_percent_finished,hours,minutes,seconds);                   // progress
		// 利用openMP并行执行for循环
		//#pragma omp parallel for
		for (int y = 0; y < c.HEIGHT; ++y)
		{
			int index = (x + y * c.WIDTH);
			SpectralDistribution sd;
			if (SUB_SAMPLING_DIRECT_SPECULAR)
			{
				for (int i = 0; i < SUB_SAMPLING_DIRECT_SPECULAR; ++i)
				{
					Ray r = c.castRay(
						x, //像素点横坐标
						(c.HEIGHT - y - 1), //像素点纵坐标
						dis(gen), //-0.5和0.5之间的随机值，用于x方向上的采样
						dis(gen)); //-0.5和0.5之间的随机值，用于y方向上的采样
					sd += s.traceRay(r, Scene::WHITTED_SPECULAR) * glm::dot(r.direction, camera_plane_normal);
				}
				irradiance_values[index] += sd / SUB_SAMPLING_DIRECT_SPECULAR * (2 * M_PI);				
			}
			sd = SpectralDistribution();
			if (SUB_SAMPLING_CAUSTICS)
				{
				for (int i = 0; i < SUB_SAMPLING_CAUSTICS; ++i)
				{
					Ray r = c.castRay(
						x, // Pixel x
						(c.HEIGHT - y - 1), // Pixel y 
						dis(gen), // Parameter x (>= -0.5 and < 0.5), for subsampling
						dis(gen)); // Parameter y (>= -0.5 and < 0.5), for subsampling
					sd += s.traceRay(r, Scene::CAUSTICS) * glm::dot(r.direction, camera_plane_normal);
				}
				irradiance_values[index] += sd / SUB_SAMPLING_CAUSTICS * (2 * M_PI);
			}
			sd = SpectralDistribution();
			if (SUB_SAMPLING_MONTE_CARLO)
			{
				for (int i = 0; i < SUB_SAMPLING_MONTE_CARLO; ++i)
				{
					Ray r = c.castRay(
						x, //像素点横坐标
						(c.HEIGHT - y - 1), //像素点纵坐标
						dis(gen), //-0.5和0.5之间的随机值，用于x方向上的采样
						dis(gen)); //-0.5和0.5之间的随机值，用于y方向上的采样
					sd += s.traceRay(r, Scene::MONTE_CARLO) * glm::dot(r.direction, camera_plane_normal);
				}
				irradiance_values[index] += sd / SUB_SAMPLING_MONTE_CARLO * (2 * M_PI);
			}
		}
	}

	time(&time_now);
	double time_elapsed = difftime(time_now, time_start);
	int hours_elapsed = time_elapsed / (60 * 60);
	int minutes_elapsed = (int(time_elapsed) % (60 * 60)) / 60;
	int seconds_elapsed = int(time_elapsed) % 60;

	int hours_prerender = prerender_time / (60 * 60);
	int minutes_prerender = (int(prerender_time) % (60 * 60)) / 60;
	int seconds_prerender = int(prerender_time) % 60;

	std::string rendering_time_string =
		std::to_string(hours_elapsed) + "h:"
		+ std::to_string(minutes_elapsed) + "m:"
		+ std::to_string(seconds_elapsed) + "s";

	std::string prerender_time_string =
		std::to_string(hours_prerender) + "h:"
		+ std::to_string(minutes_prerender) + "m:"
		+ std::to_string(seconds_prerender) + "s";

	std::cout << "\n——绘制完成，用时 : " << rendering_time_string << std::endl;

	float gamma = 1 / 2.2;
	for (int x = 0; x < c.WIDTH; ++x)
	{
		for (int y = 0; y < c.HEIGHT; ++y)
		{
			int index = (x + y * c.WIDTH);
			pixel_values[index * 3 + 0] = char(int(glm::clamp(
				glm::pow(irradiance_values[index][0],gamma), 0.0f, 1.0f) * 255)); // Red
			pixel_values[index * 3 + 1] = char(int(glm::clamp(
				glm::pow(irradiance_values[index][1],gamma), 0.0f, 1.0f) * 255)); // Green
			pixel_values[index * 3 + 2] = char(int(glm::clamp(
				glm::pow(irradiance_values[index][2],gamma), 0.0f, 1.0f) * 255)); // Blue
		}
	}

	std::string date_time = currentDateTime();
	std::string file_name = date_time + ".png";

	//将绘制结果保存为PNG图片格式
	std::string pic = date_time+".png";
	savePNG(pic, WINDOW_WIDTH, WINDOW_HEIGHT, pixel_values);


	for(int h=0; h<WINDOW_HEIGHT; h++)
		for(int w = 0; w<WINDOW_WIDTH; w++){
			buffer[WINDOW_HEIGHT-1-h][w][0] = pixel_values[(h*WINDOW_HEIGHT+w)*3+0];//注意WINDOW_HEIGHT-1!
			buffer[WINDOW_HEIGHT-1-h][w][1] = pixel_values[(h*WINDOW_HEIGHT+w)*3+1];
			buffer[WINDOW_HEIGHT-1-h][w][2] = pixel_values[(h*WINDOW_HEIGHT+w)*3+2];
		}

		std::cout<<"******************************"<<std::endl;
		std::cout<<"绘制信息：\n"
			<<"输出图片名：\t\t" + date_time + ".png\n"
			<<"绘制时间：\t\t" + rendering_time_string +"\n"
			<<"绘制准备时间：\t\t" + prerender_time_string +"\n"
			<<"分辨率：\t\t" + std::to_string(WINDOW_WIDTH) + " x " + std::to_string(WINDOW_HEIGHT) + "\n"
			<<"蒙特卡罗采样数：\t" + std::to_string(SUB_SAMPLING_MONTE_CARLO) + "\n"
			<<"直接镜面反射采样数：\t" + std::to_string(SUB_SAMPLING_DIRECT_SPECULAR) + "\n"
			<<"场景中的物体数：\t" + std::to_string(s.getNumberOfObjects()) + "\n";

		delete [] irradiance_values;
		delete [] pixel_values;

		//绘制结束提示音
		std::cout << '\a';


		init();
		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutMainLoop();

}