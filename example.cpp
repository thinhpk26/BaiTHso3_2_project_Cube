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
		verticesUpCylinder[i] = calVertices(verticesCenterSurfaceCylinder[0], i * (2 * M_PI / edge));
		// Tạo độ các điểm trên mặt dưới
		verticesBottomCylinder[i] = calVertices(verticesCenterSurfaceCylinder[1], i * (2 * M_PI / edge));
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
		quadCylinder(i, i, i - 1, i - 1);
		drawSurface(verticesCenterSurfaceCylinder[0], verticesUpCylinder[i - 1], verticesUpCylinder[i]);
		drawSurface(verticesCenterSurfaceCylinder[1], verticesBottomCylinder[i], verticesBottomCylinder[i - 1]);
	}
	quadCylinder(0, 0, edge - 1, edge - 1);
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

	glGenBuffers(1, &bufferOfSon);
	glBindBuffer(GL_ARRAY_BUFFER, bufferOfThinh);

	glGenBuffers(1, &bufferOfNhat);
	glBindBuffer(GL_ARRAY_BUFFER, bufferOfNhat);

	glGenBuffers(1, &bufferOfSon);
	glBindBuffer(GL_ARRAY_BUFFER, bufferOfThu);

	glGenBuffers(1, &bufferOfSon);
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

	color4 material_ambient(1.0, 0.0, 1.0, 1.0);
	color4 material_diffuse(1.0, 1.0, 1.0, 1.0);
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


// Sơn
class Son {

public:
	void canhcua(GLfloat x) {
		mat4 instance = Translate(x, 0, 0) * Scale(0.8, 1, 0.05);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);

		color4 light_diffuse(1, 1, 1, 1);
		color4 material_diffuse(0.467, 0.616, 0.678, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);

		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	void taycam(GLfloat x) {
		mat4 instance = Translate(x, 0, 0) * Scale(0.16, 0.08, 0.04);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);

		color4 light_diffuse(1, 1, 1, 1);
		color4 material_diffuse(0, 0, 0, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);

		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	void khung() {
		// định nghĩa các phép biến đổi tác động vào model
		mat4 instance = RotateY(40) * Scale(0.2, 0.4, 0.2);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);

		// thay đổi màu vật liệu
		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 0.8, 0.4, 1.0);    // thay đổi tại đây theo dạng màu rgba
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);

		// trường hợp dùng cube để phát triển -> set draw như bên dưới
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	void banh() {
		// định nghĩa các phép biến đổi tác động vào model
		mat4 instance = Translate(0.5, 0, 0) * RotateX(90) * Scale(0.2, 0.2, 0.2);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);

		// thay đổi màu vật liệu
		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 0.4, 0.7, 1.0);    // thay đổi tại đây theo dạng màu rgba
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);

		// trường hợp dùng hình trụ để phát triển -> set draw như bên dưới
		setDrawObject(bufferCylinder, sizeof(pointsCylinder));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCylinder);
	}
	static void run() {
		Son s;
		s.khung();
		s.banh();
		s.canhcua(0.41);
		s.canhcua(-0.41);
		s.taycam(0.031);
		//s.taycam(-0.031);

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

public:
	static void run() {

	}
};


// Hùng
class Hung {

public:
	static void run() {

	}
};


// Nhật
class Nhat {

	static mat4 nhat_model_controller;
	static mat4 fly_right_controller;
	static mat4 fly_left_controller;
	static mat4 flag_controller;
	static float rotate_wing;
	static float rotate_wing_y;
	static float rotate_flag_y;
	static float rotate_root_z;


	static mat4 ShearX(float x, float y)
	{
		mat4 c;
		c[0][1] = x;
		c[0][2] = y;
		return c;
	}

	void drawHeadUp() {
		mat4 instance_Car = ShearX(-0.5, 0) * Scale(0.5, 0.5, 1.5);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(255, 255, 0, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}

	//============ material wheel ==============
	void wheel(float x, float y, float z, float r, float k = 0) {
		mat4 instance = RotateZ(k) *RotateY(r) * Translate(x,y,z)*  RotateX(90)* Scale(0.5, 0.1, 0.5);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance);
		color4 light_diffuse(1.0, 0, 0, 1.0);
		color4 material_diffuse(0, 10, 0.4, 1.0);    // thay đổi tại đây theo dạng màu rgba
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		setDrawObject(bufferCylinder, sizeof(pointsCylinder));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCylinder);
	}
	void connectwheel(float x, float y, float z, float r) {
		mat4 instance = RotateY(r) * Translate(x, y, z) * Translate(0, 0, -0.1)   * Scale(0.1, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance);
		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 0.6, 0.4, 1.0);    // thay đổi tại đây theo dạng màu rgba
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	void baseWheel(float x, float y, float z, float r) {
		mat4 instance = RotateY(r) * Translate(x, y, z) * Translate(0, 0, -0.3) *  Scale(0.3, 0.3, 0.3);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance);
		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 0.6, 0.4, 1.0);    // thay đổi tại đây theo dạng màu rgba
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	void wheelWidget(float x, float y, float z, float r = 0) {
		wheel(x,y,z,r);
		connectwheel(x,y,z, r);
		baseWheel(x,y,z, r);
	}
	// ============ material base behind =========
	void baseBehind(float x, float y, float z) {
		mat4 instance = Translate(x,y,z)* Scale(3, 0.2, 1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance);
		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 0.8, 0.4, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	// ============ light material ===============
	void tailLight(float x , float y, float z, float r = 0) {
		mat4 instance = RotateY(r) * Translate(x, y, z) * RotateZ(90)  * Scale(0.1, 0.1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance);
		color4 light_diffuse(1.0, 0, 0, 1.0);
		color4 material_diffuse(255, 0, 0, 0.8);    // thay đổi tại đây theo dạng màu rgba
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		setDrawObject(bufferCylinder, sizeof(pointsCylinder));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCylinder);
	}
	// ============ truck material ===============
	void truck(float x, float y, float z) {
		mat4 instance = Translate(x,y,z)* Scale(3, 0.7, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance);
		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 0.8, 0.4, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	void truck2(float x, float y, float z) {
		mat4 instance = RotateY(90) * Translate(x, y, z) * Scale(1, 0.3, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance);
		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 0.8, 0.4, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	void truckHead(float x, float y, float z) {
		mat4 instance = Translate(x, y, z) * Scale(0.5, 0.3, 0.5);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance);
		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 0.8, 0.4, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	void drawTruckHead() {
		truckHead(0, 0.3, 0.8);
	}
	//========= header material ==========
void drawHeadBeHind() {
		mat4 instance_Car = Translate(-0.625, 0, 0) * Scale(1, 0.5, 1.5);
		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(255, 255, 0, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance_Car);
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	void subHeader() {
		mat4 instance_Car = Translate(-0.375, -0.625, 0) * Scale(1.5, 0.75, 1.5);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 127, 255, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	void glassHeader() {
		mat4 instance_Car = Translate(0.25, 0, 0) * ShearX(-0.5, 0) * Scale(0.0001, 0.4, 1.3);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 1, 1, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	void glassSide(float side) {
		mat4 instance_Car = Translate(-0.1, 0, side) * ShearX(-0.5, 0) * Scale(0.5, 0.4, 0.0001);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 1, 1, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	void glassSideOther(float side, float tranX) {
		mat4 instance_Car = Translate(tranX, 0, side) * Scale(0.5, 0.4, 0.0001);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 1, 1, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	void drawGlassSideBehind(float side, float tranX) {
		mat4 instance_Car = Translate(tranX, 0, side) * Scale(0.3, 0.4, 0.0001);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 1, 1, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	// Vẽ 2 cạnh bên
	void drawBackSide(float x, float y, float z) {
		mat4 instance_Car = Translate(x, y, z) * Scale(1.5, 0.75, 0.1);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(1, 0, 0, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}
	void drawLightCar(float x, float y, float z) {

		mat4 instance_Car = Translate(x, y, z) * RotateZ(90) * Scale(0.2, 0.1, 0.2);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse((float)218 / 255, (float)165 / 255, (float)32 / 255, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance_Car);

		// Phải dùng 2 dòng lệnh để vẽ hình trụ hoặc hình lập phương
		setDrawObject(bufferCylinder, sizeof(pointsCylinder));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCylinder);

	}
	void drawHead() {
		drawHeadUp();
		drawHeadBeHind();
		subHeader();
		glassHeader();
		glassSide(-0.75);
		glassSideOther(0.75, -0.2);
		glassSideOther(-0.75, -0.2);
		glassSide(0.75);
		drawLightCar(0.36, -0.35, -0.45);
		drawLightCar(0.36, -0.35, 0.45);
		for (int i = 0; i < 10; i++) {
			drawLightCar(0.36, -0.65- i*0.009, -0.45+ i*0.05);
			drawLightCar(0.36, -0.65 - i * 0.009, 0.45 - i * 0.05);
		}
	}
	// ================== flag vietnam ==============
	void buttress(float x, float y, float z) {
		mat4 instance =Translate(x, y, z) * Scale(0.1, 2, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller* flag_controller * instance);
		color4 light_diffuse(1.0, 0, 0, 1.0);
		color4 material_diffuse(0, 40, 40, 1.0);    // thay đổi tại đây theo dạng màu rgba
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		setDrawObject(bufferCylinder, sizeof(pointsCylinder));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCylinder);
	}
	void flag(float x, float y, float z) {
		mat4 instance = Translate(x, y, z) * Translate(-0.6,0.5,0) * Scale(1.2, 1, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller* flag_controller * instance);
		color4 light_diffuse(1.0, 0, 0, 1.0);
		color4 material_diffuse(255, 255, 0, 1.0);    // thay đổi tại đây theo dạng màu rgba
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		setDrawObject(bufferCylinder, sizeof(pointsCylinder));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCylinder);
	}
	void star(float x, float y, float z) {
		vec4 vertices[] = {
		vec4(0.0,  0.5, 0.0, 1.0),
		vec4(0.112, 0.154, 0.0, 1.0),
		vec4(0.475, 0.154, 0.0, 1.0),
		vec4(0.181, -0.059, 0.0, 1.0),
		vec4(0.293, -0.422, 0.0, 1.0),
		vec4(0.0, -0.206, 0.0, 1.0),
		vec4(-0.293, -0.422, 0.0, 1.0),
		vec4(-0.181, -0.059, 0.0, 1.0),
		vec4(-0.475, 0.154, 0.0, 1.0),
		vec4(-0.112, 0.154, 0.0, 1.0),
		};
		GLuint indices[] = {
			0, 1, 2,
			0, 2, 3,
			0, 3, 4,
			0, 4, 5,
			0, 5, 6,
			0, 6, 7,
			0, 7, 8,
			0, 8, 9,
			0, 9, 1,
		};

		GLuint buffer[2];
		glGenBuffers(2, buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		mat4 instance = Translate(x, y, z) * Scale(0.5, 0.5, 1.0);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance);

		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(255, 215, 10, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);

		setDrawObject(buffer[0], sizeof(vertices));
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	}

	void materialWing(float x, float y, float z, float r , mat4 input_controller) {
		mat4 instance = Translate(x, y, z) *RotateX(r) * Scale(3, 0.6, 0.1);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller* input_controller * instance);
		color4 light_diffuse(1.0, 0, 0, 1.0);
		color4 material_diffuse(255, 255, 255, 1.0);    // thay đổi tại đây theo dạng màu rgba
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}

	void baseBehind1(float x, float y, float z, float r = 0) {
		mat4 instance =Translate(x, y, z) * RotateZ(r) * Scale(0.7, 0.2, 0.8);
		glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * nhat_model_controller * instance);
		color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
		color4 material_diffuse(0, 0, 139, 1.0);
		color4 diffuse_product = light_diffuse * material_diffuse;
		glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
		setDrawObject(bufferCube, sizeof(pointsCube));
		glDrawArrays(GL_TRIANGLES, 0, NumPointsCube);
	}

	//========== systemlayout ========
	void systemWheelBehind() {
		wheelWidget(-0.09, -1.1, 0.9);
		wheelWidget(-0, -1.1, 0.9,180);
		wheelWidget(-3, -1.1, 0.65);
		wheelWidget(-3.5, -1.1, 0.65);
		wheelWidget(3.5, -1.1, 0.65,180);
		wheelWidget(3, -1.1, 0.65, 180);
		truck(-2.5, -0.6, 0.44);
		truck(-2.5, -0.6, -0.44);
		truck2(0, -0.6, -4);
		wheel(-0.8, -0.5, -0.75,0);
		wheel(-0.8, -0.5, 0.75, 0);
	}
	//========== draw system layout ===========
	void drawGraphic() {
		this->systemWheelBehind();
		baseBehind(-2.5,-0.9,0);
		drawHead();
		buttress(0,0.7,0.6);
		flag(0, 0.7, 0.6);
		buttress(0, 0.7, -0.6);
		flag(0, 0.7, -0.6);

		baseBehind1(-1.55, -0.2, 0, rotate_root_z);
		baseBehind1(-2.25, -0.2, 0, rotate_root_z);
		baseBehind1(-2.95, -0.2, 0, rotate_root_z);

		materialWing(-2.5, -0.6, -0.44, rotate_wing, fly_left_controller);
		materialWing(-2.5, -0.6, 0.44, rotate_wing, fly_right_controller);

		baseBehind(-2.5, -0.9, 0);

		
	}
	
	
public:
	

	static void modeOn(float x = 1) {
		if (rotate_root_z > -50) {
			rotate_root_z = rotate_root_z - 1;
			return;
		}
		if (rotate_wing == 90) {
			if (rotate_wing_y <= 34) {
				rotate_wing_y = rotate_wing_y + x;
				fly_right_controller = fly_right_controller * RotateY(-x);
				fly_left_controller = fly_left_controller * RotateY(x);
			}
		}
		else  {
			rotate_wing = rotate_wing + 1;
		}
		if (rotate_wing_y > 34) {
			if (rotate_flag_y <= 45) {
				flag_controller = flag_controller * RotateZ(1);
				rotate_flag_y = rotate_flag_y + 1;
			}

		}
	}
	static void modeOff(float x = 1) {

		if (rotate_root_z != 0) {
			rotate_root_z = rotate_root_z + 1;
			return;
		}

		if (rotate_wing_y != 0) {
			rotate_wing_y = rotate_wing_y - x;
			fly_right_controller = fly_right_controller * RotateY(x);
			fly_left_controller = fly_left_controller * RotateY(-x);
		}
		if (rotate_wing_y == 0 && rotate_flag_y != 0) {
			flag_controller = flag_controller * RotateZ(-1);
			rotate_flag_y = rotate_flag_y - 1;

		}
		if (rotate_wing_y == 0 && rotate_flag_y == 0 && rotate_wing != 0) {

				rotate_wing = rotate_wing - 1;
			

		}
		
	}
	// === method scale, rotate  object

	static void rotate_model_nhat(float x = 0, float y = 0, float z = 0) {
		nhat_model_controller = nhat_model_controller * RotateX(x) * RotateY(y) * RotateZ(z);
	}

	static void translate_model_nhat(float x = 0, float y = 0, float z = 0) {
		nhat_model_controller = nhat_model_controller * Translate(x,y,z);
	}

	static void scale_model_nhat(float x = 0, float y = 0, float z = 0) {
		nhat_model_controller = nhat_model_controller * Translate(x, y, z);
	}


	static void run() {
		mat4 nhat_model_controller = Translate(0, 0, 0) * RotateY(0) * Scale(0.5, 0.5, 0.5);
		Nhat().drawGraphic();
	}
};

mat4 Nhat::nhat_model_controller = RotateX(0);
mat4 Nhat::fly_right_controller = RotateX(0);
mat4 Nhat::fly_left_controller = RotateX(0);
mat4 Nhat::flag_controller = RotateZ(0);

float Nhat::rotate_wing = 0;
float Nhat::rotate_wing_y = 0;
float Nhat::rotate_flag_y = 0;
float Nhat::rotate_root_z = 0;


// Thông số của mắt nhìn
GLfloat XEye = 0;
GLfloat YEye = 0;
GLfloat ZEye = 0;

// Phương thức tổng hợp các đối tượng đã được vẽ
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Phương thức vẽ

	Nhat::run();
	Hung::run();
	Thinh::run();
	Thu::run();
	//Son::run();

	//glUniformMatrix4fv(model_loc, 1, GL_TRUE, model);

	vec4 eye(0 + XEye, 0 + YEye, 2 + ZEye, 1);
	vec4 at(0 + XEye, 0 + YEye, 0 + ZEye, 1);
	vec4 up(0, 1, 0, 1);

	view = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);

	projection = Frustum(-1, 1, -1, 1, 1, 4);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);

	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{
	// keyboard handler

	switch (key) {
	case 033:			// 033 is Escape key octal value
		exit(1);		// quit program
		break;

	case 'r':
		XEye = XEye  * cos(30);
		
		ZEye = ZEye * sin(30);

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
	case 'j':
		Nhat::modeOn();
		break;
	case 't':
		Nhat::modeOff();
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
