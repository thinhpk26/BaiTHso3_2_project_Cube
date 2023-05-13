//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/
#include "vector"

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

//------------------------------------------- Hình trụ tròn----------------------------------------------------

// số đường muốn vẽ
int edge = 300;
// Số các đỉnh -  bằng với hệ số bên dưới
const int NumPointsCylinder = 300 * 12 * 2;

// tạo độ 2 điểm trung trực
point4 verticesCenterSurfaceCylinder[2] = { point4(0, 0.5, 0, 1), point4(0, -0.5, 0, 1) };

point4 calVertices(point4 centerSurface, GLfloat radius) {
	float x = (centerSurface.x + 0.5) * cos(radius);
	float y = centerSurface.y;
	float z = (centerSurface.z + 0.5) * sin(radius);
	return point4(x, y, z, 1);
}

point4 pointsCylinder[NumPointsCylinder]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colorsCylinder[NumPointsCylinder]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
vec3 normalsCylinder[NumPointsCylinder]; /*Danh sách các vector pháp tuyến ứng với mỗi đỉnh*/

point4 verticesUpCylinder[300];
point4 verticesBottomCylinder[300];
point4 vertex_colorsCylinder[300];

void InitCylinder(int edge) {
	for (int i = 0; i < edge; i++) {
		// Tạo độ các điểm trên mặt trên
		verticesUpCylinder[i] = calVertices(verticesCenterSurfaceCylinder[0], i * (2* M_PI / edge));
		// Tạo độ các điểm trên mặt dưới
		verticesBottomCylinder[i] = calVertices(verticesCenterSurfaceCylinder[1],  i * (2 * M_PI / edge));
	}

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	vertex_colorsCylinder[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	vertex_colorsCylinder[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	vertex_colorsCylinder[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	vertex_colorsCylinder[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	vertex_colorsCylinder[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	vertex_colorsCylinder[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	vertex_colorsCylinder[6] = color4(1.0, 1.0, 1.0, 1.0); // white
	vertex_colorsCylinder[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}
int IndexForCylinder = 0;
void quadCylinder(int a, int b, int c, int d)  /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	vec4 u = verticesBottomCylinder[b] - verticesUpCylinder[a];
	vec4 v = verticesBottomCylinder[c] - verticesBottomCylinder[b];
	vec3 normal = normalize(cross(u, v));

	normalsCylinder[IndexForCylinder] = normal; colorsCylinder[IndexForCylinder] = vertex_colorsCylinder[a]; pointsCylinder[IndexForCylinder] = verticesUpCylinder[a]; IndexForCylinder++;
	normalsCylinder[IndexForCylinder] = normal; colorsCylinder[IndexForCylinder] = vertex_colorsCylinder[b]; pointsCylinder[IndexForCylinder] = verticesBottomCylinder[b]; IndexForCylinder++;
	normalsCylinder[IndexForCylinder] = normal; colorsCylinder[IndexForCylinder] = vertex_colorsCylinder[c]; pointsCylinder[IndexForCylinder] = verticesBottomCylinder[c]; IndexForCylinder++;
	normalsCylinder[IndexForCylinder] = normal; colorsCylinder[IndexForCylinder] = vertex_colorsCylinder[a]; pointsCylinder[IndexForCylinder] = verticesUpCylinder[a]; IndexForCylinder++;
	normalsCylinder[IndexForCylinder] = normal; colorsCylinder[IndexForCylinder] = vertex_colorsCylinder[c]; pointsCylinder[IndexForCylinder] = verticesBottomCylinder[c]; IndexForCylinder++;
	normalsCylinder[IndexForCylinder] = normal; colorsCylinder[IndexForCylinder] = vertex_colorsCylinder[d]; pointsCylinder[IndexForCylinder] = verticesUpCylinder[d]; IndexForCylinder++;
}

void drawSurface(vec4 pointCenter, vec4 point1, vec4 point2) {
	vec4 u = point1 - point2;
	vec4 v = pointCenter - point1;
	vec3 normal = normalize(cross(u, v));

	normalsCylinder[IndexForCylinder] = normal; colorsCylinder[IndexForCylinder] = vertex_colorsCylinder[1]; pointsCylinder[IndexForCylinder] = pointCenter; IndexForCylinder++;
	normalsCylinder[IndexForCylinder] = normal; colorsCylinder[IndexForCylinder] = vertex_colorsCylinder[1]; pointsCylinder[IndexForCylinder] = point1; IndexForCylinder++;
	normalsCylinder[IndexForCylinder] = normal; colorsCylinder[IndexForCylinder] = vertex_colorsCylinder[1]; pointsCylinder[IndexForCylinder] = point2; IndexForCylinder++;
}

void makeColorCylinder(int edge)  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/

{
	for (int i = 1; i < edge; i++) {
		quadCylinder(i, i, i-1, i-1);
		drawSurface(verticesCenterSurfaceCylinder[0], verticesUpCylinder[i - 1], verticesUpCylinder[i]);
		drawSurface(verticesCenterSurfaceCylinder[1], verticesBottomCylinder[i], verticesBottomCylinder[i-1]);
	}
	quadCylinder(0, 0, edge-1, edge-1);
	drawSurface(verticesCenterSurfaceCylinder[0], verticesUpCylinder[edge - 1], verticesUpCylinder[0]);
	drawSurface(verticesCenterSurfaceCylinder[1], verticesBottomCylinder[0], verticesBottomCylinder[edge - 1]);

}

void generateGeometryCylinder(void)
{
	InitCylinder(edge);
	makeColorCylinder(edge);
}

GLuint bufferCylinder;
GLuint bufferCube;

GLuint bufferOfSon;
GLuint bufferOfThinh;
GLuint bufferOfThu;
GLuint bufferOfNhat;
GLuint bufferOfHung;

void initGPUBuffers(void)
{
	// Tạo một VAO - vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Tạo và khởi tạo một buffer chứa cube
	glGenBuffers(1, &bufferCube);
	glBindBuffer(GL_ARRAY_BUFFER, bufferCube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pointsCube) + sizeof(colorsCube) + sizeof(normalsCube), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointsCube), pointsCube);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsCube), sizeof(colorsCube), colorsCube);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsCube) + sizeof(colorsCube), sizeof(normalsCube), normalsCube);

	// Tạo và khởi tạo một buffer chứa Cylinder
	glGenBuffers(1, &bufferCylinder);
	glBindBuffer(GL_ARRAY_BUFFER, bufferCylinder);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pointsCylinder) * 3, NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pointsCylinder), pointsCylinder);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsCylinder), sizeof(colorsCylinder), colorsCylinder);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(pointsCylinder) + sizeof(colorsCylinder), sizeof(normalsCylinder), normalsCylinder);

	// Khởi tạo cho mỗi người 1 buffer làm việc riêng (Khoảng không gian làm việc với đỉnh riêng)
	glGenBuffers(1, &bufferOfSon);
	glBindBuffer(GL_ARRAY_BUFFER, bufferOfSon);

	glGenBuffers(1, &bufferOfThinh);
	glBindBuffer(GL_ARRAY_BUFFER, bufferOfThinh);

	glGenBuffers(1, &bufferOfNhat);
	glBindBuffer(GL_ARRAY_BUFFER, bufferOfNhat);

	glGenBuffers(1, &bufferOfThu);
	glBindBuffer(GL_ARRAY_BUFFER, bufferOfThu);

	glGenBuffers(1, &bufferOfHung);
	glBindBuffer(GL_ARRAY_BUFFER, bufferOfHung);



}

GLfloat theta[3] = { 0, 0, 0 };
GLfloat dr = 5;

mat4 model;
GLuint model_loc;
mat4 projection;
GLuint projection_loc;
mat4 view;
GLuint view_loc;

// Set shader nhận diện đang vẽ hình khối nào
void setDrawObject(GLuint buffer, int size) {
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// Khởi tạo thuộc tính vị trí đỉnh của cube từ vertex shader
	GLuint loc_vPositionCube = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc_vPositionCube);
	glVertexAttribPointer(loc_vPositionCube, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint loc_vColorCube = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColorCube);
	glVertexAttribPointer(loc_vColorCube, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(size));

	GLuint loc_vNormalCube = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormalCube);
	glVertexAttribPointer(loc_vNormalCube, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(size * 2));
}

void shaderSetup(void)
{
	// Nạp các shader và sử dụng chương trình shader
	program = InitShader("vshader1.glsl", "fshader1.glsl");   // hàm InitShader khai báo trong Angel.h
	glUseProgram(program);

	/* Khởi tạo các tham số chiếu sáng - tô bóng*/
	point4 light_position(0.0, 0.0, 1.0, 0.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(1.0, 1.0, 1.0, 1.0);
	color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
	color4 material_specular(0.2, 0.2, 0.2, 1.0); /*Chinh lai specular cho màu  sắc chính xác*/
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


// Sơn
class Son {


public:
	static void run() {

	}
};


// Thư
class Thu {

public:
	static void run() {

	}
};


// Thịnh
class Thinh {
	static mat4 model_Car;
	static mat4 instance_Car;
	// Vẽ bánh xe
	static void drawWheel(float x, float y, float z) {
		instance_Car = Translate(x, y, z) * Scale(0.5, 0.3, 0.5);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0.2, 0.2, 0.2, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);

		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCylinder, sizeof(pointsCylinder));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCylinder);
	}

	static void drawHub() {
		instance_Car = Scale(0.03, 2.2, 0.03);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 1, 1, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCylinder, sizeof(pointsCylinder));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCylinder);
	}

	static void drawHubAndTwoWheeFront() {
		drawHub();
		drawWheel(0, 0.92, 0);
		drawWheel(0, -0.92, 0);
	}

	static mat4 ShearX(float x, float y)
	{
		mat4 c;
		c[0][1] = x;
		c[0][2] = y;
		return c;
	}

	static mat4 ShearY(float x, float y)
	{
		mat4 c;
		c[1][0] = x;
		c[1][2] = y;
		return c;
	}

	static mat4 ShearZ(float x, float y)
	{
		mat4 c;
		c[2][0] = x;
		c[2][1] = y;
		return c;
	}

	// Vẽ đầu xe phía trên
	static void drawHeadUp() {
		instance_Car = ShearX(-0.5, 0) * Scale(0.5, 0.5, 1.5);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(1, 0, 0, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}

	// Vẽ đầu xe phía đuôi
	static void drawHeadBeHind() {
		instance_Car = Translate(-0.625, 0, 0) * Scale(1, 0.5, 1.5);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(1, 0, 0, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}

	// Vẽ đầu xe phía dưới
	static void drawHeadBeBottom() {
		instance_Car = Translate(-0.375, -0.625, 0) * Scale(1.5, 0.75, 1.5);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(1, 0, 0, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}

	// Vẽ đèn xe
	static void drawLightCar(float x, float y, float z) {

		instance_Car = Translate(x, y, z) * RotateZ(90) * Scale(0.2, 0.1, 0.2);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse((float)218/255, (float)165/255, (float)32/255, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCylinder, sizeof(pointsCylinder));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCylinder);

	}

	// Vẽ kính
	static void drawGlassHead() {
		instance_Car = Translate(0.25, 0, 0) * ShearX(-0.5, 0) * Scale(0.0001, 0.4, 1.3);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 1, 1, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	static void drawGlassSide(float side) {
		instance_Car = Translate(-0.1, 0, side) * ShearX(-0.5, 0) * Scale(0.5, 0.4, 0.0001);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 1, 1, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}

	static void drawGlassSide1(float side, float tranX) {
		instance_Car = Translate(tranX, 0, side) * Scale(0.5, 0.4, 0.0001);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 1, 1, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}

	static void drawGlassSideBehind(float side, float tranX) {
		instance_Car = Translate(tranX, 0, side) * Scale(0.3, 0.4, 0.0001);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 1, 1, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	
	// Vẽ 2 cạnh bên
	static void drawBackSide(float x, float y , float z) {
		instance_Car = Translate(x, y, z) * Scale(1.5, 0.75, 0.1);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(1, 0, 0, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	// Vẽ 1 cạnh đáy
	static void drawBackBottom() {
		instance_Car = Translate(-1.878, -0.95, 0) * Scale(1.5, 0.1, 1.5);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(1, 0, 0, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}

	static void drawOutriggers(float x, float y, float z) {
		instance_Car = Translate(x, y, z) * ShearX(-0.5, 0) * Scale(0.2, 1, 0.1);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse((float)0, (float)250 / 255, (float)154 / 255, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}

	static void drawElbow() {
		instance_Car = Translate(-2.7, -0.05, 0) * RotateX(90) * Scale(0.1, 0.75, 0.1);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse((float)105 / 255, (float)105 / 255, (float)105 / 255, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCylinder, sizeof(pointsCylinder));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCylinder);
	}

	// biến thay đổi phép biến đổi để vẽ thang
	static mat4 instance_Stair;

	// chân chống thang
	static void drawBodyLadder(float x, float y, float z) {
		instance_Car = instance_Stair * Translate(x, y, z) * Scale(0.05, 2.5, 0.05);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse((float)169 / 255, (float)169 / 255, (float)169 / 255, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}

	// thanh ngang của thang
	static void drawRungLadder(float x, float y, float z, float shear/*vẽ thanh ngang chéo*/) {
		instance_Car = instance_Stair * ShearY(shear, 0) * Translate(x, y, z) * RotateZ(90) * Scale(0.05, 0.4, 0.05);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse((float)105 / 255, (float)105 / 255, (float)105 / 255, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCylinder, sizeof(pointsCylinder));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCylinder);
	}

	// Vẽ mặt thang nhỏ
	static void drawSideLadder() {
		drawBodyLadder(0.2, 0, 0);
		drawBodyLadder(-0.2, 0, 0);

		float i = 0; 
		while (i <= 1) {
			drawRungLadder(0, -i, 0, 1);
			drawRungLadder(0, -i, 0, -1);
			drawRungLadder(0, i, 0, 1);
			drawRungLadder(0, i, 0, -1);
			i += 0.5;
		}
	}

	// Vẽ thang nhỏ bên trong
	static void drawInSideLadder() {
		instance_Stair = Translate(0, 0, 0.2);
		drawSideLadder();
		instance_Stair = Translate(0, 0, -0.2);
		drawSideLadder();
		instance_Stair = Translate(-0.2, 0, 0) * RotateY(90);
		drawSideLadder();
		instance_Stair = Translate(0.2, 0, 0) * RotateY(90);
		drawSideLadder();
	}

	// Vẽ mặt thang lớn
	static void drawSurfaceOfOutSideLadder() {
		instance_Car = instance_Stair * Scale(0.6975, 2.5, 0.0975);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse((float)169 / 255, (float)169 / 255, (float)169 / 255, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}


	// vẽ bao bọc của thang
	static void drawOutSideLadder() {
		instance_Stair = Translate(0, 0, 0.3);
		drawSurfaceOfOutSideLadder();
		instance_Stair = Translate(0, 0, -0.3);
		drawSurfaceOfOutSideLadder();
		instance_Stair = Translate(-0.3, 0, 0) * RotateY(90);
		drawSurfaceOfOutSideLadder();
		instance_Stair = Translate(0.3, 0, 0) * RotateY(90);
		drawSurfaceOfOutSideLadder();
	}

	// Vẽ thùng đựng người cứu hỏa
	static void drawContainer() {
		// Xét màu
		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse((float)147/255, (float)112/255, (float)219/255, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);

		instance_Car = Translate(0, 0, 0.325) * Scale(0.7, 0.5, 0.05);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);

		instance_Car = Translate(0, 0, -0.325) * Scale(0.7, 0.5, 0.05);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);

		instance_Car = Translate(0.325, 0, 0) * RotateY(90) * Scale(0.7, 0.5, 0.05);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);

		instance_Car = Translate(-0.325, 0, 0) * RotateY(90) * Scale(0.7, 0.5, 0.05);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);

		instance_Car = Translate(0, -0.225, 0) * Scale(0.7, 0.05, 0.7);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * model_Car * instance_Car);
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	// Vẽ đầu xe
	static void drawHead() {
		drawHeadUp();
		drawHeadBeHind();
		drawHeadBeBottom();
		// Vẽ kính
		drawGlassHead();
		drawGlassSide(-0.75);
		drawGlassSide1(0.75, -0.2);
		drawGlassSide1(-0.75, -0.2);
		drawGlassSide(0.75);
		drawGlassSideBehind(-0.75, -0.8);
		drawGlassSideBehind(0.75, -0.8);
		// Vẽ đèn xe
		drawLightCar(0.36, -0.75, -0.45);
		drawLightCar(0.36, -0.75, 0.45);
	}

	// Vẽ đuôi xe
	static void drawBackCar() {
		drawBackBottom();
		drawBackSide(-1.878, -0.6, 0.7);
		drawBackSide(-1.878, -0.6, -0.7);
		drawOutriggers(-2.5, -0.45, 0.3);
		drawOutriggers(-2.5, -0.45, -0.3);
		drawElbow();
	}

	// Vẽ toàn bộ xe
	static void drawbodyCar() {
		drawHead();
		drawBackCar(); 
	}

public:
	static float thetaY;
	static float rotateOutSideStair;
	static float translateInsideStair;
	static float SpinWheels;
	static float moveCar;

	static void spinWheel(void)
	{
		SpinWheels += 10;
		if (SpinWheels > 360.0) {
			SpinWheels -= 360.0;
		}
		glutPostRedisplay();
	}

	static void run() {
		mat4 model_All_car = Translate(moveCar, 0, 0) * RotateY(thetaY) * Scale(0.5, 0.5, 0.5);
		model_Car = model_All_car;
		drawbodyCar();
		// Thiết kế đuôi xe 
		model_Car = model_Car * Translate(-2.8, -0.1, 0) * RotateZ(rotateOutSideStair) * Translate(0, 1.1, 0) * Scale(0.95, 0.95, 0.95);
		drawOutSideLadder();
		model_Car = model_Car * Translate(0, translateInsideStair, 0);
		drawInSideLadder();
		model_Car = model_Car * Translate(0, 1.5, 0) * RotateZ(90 - (rotateOutSideStair + 60 + 20)) * Scale(1, 1.5, 1);
		drawContainer();
		model_Car = model_Car * RotateX(90);

		// Thiết kế bánh xe
		model_Car = model_All_car * Translate(-0.2, -1, 0) * RotateZ(SpinWheels) * RotateX(90);
		drawHubAndTwoWheeFront();
		model_Car = model_All_car * Translate(-2, -1, 0) * RotateZ(SpinWheels) * RotateX(90);
		drawHubAndTwoWheeFront();

		// Xoay bánh xe
		//glutIdleFunc(spinWheel);

	}
};

mat4 Thinh::instance_Car = RotateX(0);
mat4 Thinh::model_Car = RotateX(0);
mat4 Thinh::instance_Stair;
float Thinh::thetaY = 0;
float Thinh::rotateOutSideStair = -65;
float Thinh::translateInsideStair = 0.4;
float Thinh::SpinWheels = 0;
float Thinh::moveCar = 0;

// Hùng
class Hung {

public:
	static void run() {

	}
};


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

// Phương thức tổng hợp các đối tượng đã được vẽ
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	vec4 eye(0 + XEye, 0 + YEye, 2 + ZEye, 1);
	vec4 at(0 + XEye, 0 + YEye, 0 + ZEye, 1);
	vec4 up(0, 1, 0, 1);

	view = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);

	projection = Frustum(-1, 1, -1, 1, 1, 4);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);

	// Phương thức vẽ
	Nhat::run();
	Hung::run();
	Thinh::run();
	Thu::run();
	Son::run();

	  /*Vẽ các tam giác*/
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
	case 'm':
		if (Thinh::translateInsideStair <= 2) {
			Thinh::translateInsideStair += 0.1;
		}
		break;
	case 'M':
		if (Thinh::translateInsideStair > 0.4) {
			Thinh::translateInsideStair -= 0.1;
		}
		break;
	case 'n':
		if (Thinh::rotateOutSideStair > -65) {
			Thinh::rotateOutSideStair -= 5;
		}
		break;
	case 'N':
		if (Thinh::rotateOutSideStair < 0) {
			Thinh::rotateOutSideStair += 5;
		}
		break;
	case 'b':
		if (Thinh::moveCar < 5) {
			Thinh::moveCar += 0.05;
		}
		break;
	case 'B':
		if (Thinh::moveCar > -5) {
			Thinh::moveCar -= 0.05;
		}
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

	// Cylinder
	generateGeometryCylinder();

	initGPUBuffers();
	shaderSetup();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}
