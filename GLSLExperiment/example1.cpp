//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/


// remember to prototype
void generateGeometry(void);
void initGPUBuffers(void);
void shaderSetup(void);
void display(void);
void keyboard(unsigned char key, int x, int y);

typedef vec4 point4;
typedef vec4 color4;
using namespace std;


GLuint program;

// --------------------------Vẽ hình lập phương-------------------------------

// Số các đỉnh của các tam giác
const int NumPointsCube = 36;

point4 pointsCube[NumPointsCube]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colorsCube[NumPointsCube]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
vec3 normalsCube[NumPointsCube]; /*Danh sách các vector pháp tuyến ứng với mỗi đỉnh*/

point4 verticesCube[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colorsCube[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/
//

void initCube()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	verticesCube[0] = point4(-0.5, -0.5, 0.5, 1.0);
	verticesCube[1] = point4(-0.5, 0.5, 0.5, 1.0);
	verticesCube[2] = point4(0.5, 0.5, 0.5, 1.0);
	verticesCube[3] = point4(0.5, -0.5, 0.5, 1.0);
	verticesCube[4] = point4(-0.5, -0.5, -0.5, 1.0);
	verticesCube[5] = point4(-0.5, 0.5, -0.5, 1.0);
	verticesCube[6] = point4(0.5, 0.5, -0.5, 1.0);
	verticesCube[7] = point4(0.5, -0.5, -0.5, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	vertex_colorsCube[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	vertex_colorsCube[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	vertex_colorsCube[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	vertex_colorsCube[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	vertex_colorsCube[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	vertex_colorsCube[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	vertex_colorsCube[6] = color4(1.0, 1.0, 1.0, 1.0); // white
	vertex_colorsCube[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}
int Index = 0;
void quad(int a, int b, int c, int d)  /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	vec4 u = verticesCube[b] - verticesCube[a];
	vec4 v = verticesCube[c] - verticesCube[b];
	vec3 normal = normalize(cross(u, v));

	normalsCube[Index] = normal; colorsCube[Index] = vertex_colorsCube[a]; pointsCube[Index] = verticesCube[a]; Index++;
	normalsCube[Index] = normal; colorsCube[Index] = vertex_colorsCube[b]; pointsCube[Index] = verticesCube[b]; Index++;
	normalsCube[Index] = normal; colorsCube[Index] = vertex_colorsCube[c]; pointsCube[Index] = verticesCube[c]; Index++;
	normalsCube[Index] = normal; colorsCube[Index] = vertex_colorsCube[a]; pointsCube[Index] = verticesCube[a]; Index++;
	normalsCube[Index] = normal; colorsCube[Index] = vertex_colorsCube[c]; pointsCube[Index] = verticesCube[c]; Index++;
	normalsCube[Index] = normal; colorsCube[Index] = vertex_colorsCube[d]; pointsCube[Index] = verticesCube[d]; Index++;
}
void makeColorCube(void)  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/

{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}
void generateGeometryCube(void)
{
	initCube();
	makeColorCube();
}

//



void initGPUBuffers(void)
{
	// Tạo một VAO - vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Tạo và khởi tạo một buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pointsCube) + sizeof(colorsCube) + sizeof(normalsCube), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointsCube), pointsCube);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsCube), sizeof(colorsCube), colorsCube);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsCube) + sizeof(colorsCube), sizeof(normalsCube), normalsCube);


}

GLfloat theta[3] = { 0, 0, 0 };
GLfloat dr = 5;

mat4 model;
GLuint model_loc;
mat4 projection;
GLuint projection_loc;
mat4 view;
GLuint view_loc;

void shaderSetup(void)
{
	// Nạp các shader và sử dụng chương trình shader
	program = InitShader("vshader1.glsl", "fshader1.glsl");   // hàm InitShader khai báo trong Angel.h
	glUseProgram(program);

	// Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
	GLuint loc_vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc_vPosition);
	glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsCube)));

	GLuint loc_vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormal);
	glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointsCube) + sizeof(colorsCube)));

	/* Khởi tạo các tham số chiếu sáng - tô bóng*/
	point4 light_position(0.0, 0.0, 1.0, 0.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(1.0, 0.0, 1.0, 1.0);
	color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
	color4 material_specular(1.0, 0.8, 0.0, 1.0);
	float material_shininess = 100.0;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);

	model_loc = glGetUniformLocation(program, "Model");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0, 1.0, 1.0, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
}


// 500 - 1000
// Sơn
class Son {


public:
	static void run() {

	}
};

// 1001 - 1500
// Thư
class Thu {

public:
	static void run() {

	}
};

//1501-2000
// Thịnh
class Thinh {

public:
	static void run() {

	}
};

//2001-2500
// Hùng
class Hung {

public:
	static void run() {

	}
};

//2501-3000
// Nhật
class Nhat {

public:
	static void run() {

	}
};

// Thông số của mắt nhìn
GLfloat XEye = 0;
GLfloat YEye = 0;
GLfloat ZEye = 0;
GLfloat RHead = 0;

// Phương thức tổng hợp các đối tượng đã được vẽ
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Phương thức vẽ

	Nhat::run();
	Hung::run();
	Thinh::run();
	Thu::run();
	Son::run();

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model);

	vec4 eye(0 + XEye, 0 + YEye, 2 + ZEye, 1);
	vec4 at(0 + XEye, 0 + YEye, 0 + ZEye, 1);
	vec4 up(tan(RHead), 1, 0, 1);

	view = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);

	projection = Frustum(-1, 1, -1, 1, 1, 4);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);

	glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);    /*Vẽ các tam giác*/
	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{
	// keyboard handler

	switch (key) {
	case 033:			// 033 is Escape key octal value
		exit(1);		// quit program
		break;
	case 'w':
		ZEye -= 0.5;
		break;
	case 'W':
		ZEye += 0.5;
		break;
	case 'q':
		XEye -= 0.5;
		break;
	case 'Q':
		XEye += 0.5;
		break;
	case 'e':
		YEye += 0.5;
		break;
	case 'E':
		YEye -= 0.5;
		break;
	case 'r':
		RHead += 5;
		break;
	case 'R':
		RHead -= 5;
		break;
	}
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	// main function: program starts here

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Drawing a Cube");


	glewInit();

	// Cube
	generateGeometryCube();

	// 

	initGPUBuffers();
	shaderSetup();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}
