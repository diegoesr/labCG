/*Practica 4: Modelado Jerárquico.
Se implementa el uso de matrices adicionales para almacenar información de transformaciones geométricas que se quiere
heredar entre diversas instancias para que estén unidas
Teclas de la F a la K para rotaciones de articulaciones
*/
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z
using std::vector;
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro y cono con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R, int index) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	if (index == 0) {
		//se genera el mesh del cilindro
		Mesh* cilindro0 = new Mesh();
		cilindro0->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
		meshList.push_back(cilindro0);
	}
	else if (index == 1)
	{
		//se genera el mesh del cilindro
		Mesh* cilindro1 = new Mesh();
		cilindro1->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
		meshList.push_back(cilindro1);
	}
	else if (index == 2)
	{
		//se genera el mesh del cilindro
		Mesh* cilindro2 = new Mesh();
		cilindro2->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
		meshList.push_back(cilindro2);
	}


}

//función para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res + 2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}



void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolución (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearCubo();//índice 0 en MeshList
	CrearPiramideTriangular();//índice 1 en MeshList
	CrearCilindro(5, 1.0f, 0);//índice 2 en MeshList		
	CrearCilindro(10, 1.0f, 1);//índice 3 en MeshList	
	CrearCono(25, 2.0f);//índice 4 en MeshList
	CrearPiramideCuadrangular();//índice 5 en MeshList
	CrearCilindro(250, 1.0f, 2);//índice 6 en MeshList	
	CreateShaders();



	/*Cámara se usa el comando: glm::lookAt(vector de posición, vector de orientación, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posición,
	glm::vec3 vector up,
	GlFloat yaw rotación para girar hacia la derecha e izquierda
	GlFloat pitch rotación para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posición inicial está en 0,0,1 y ve hacia 0,0,-1.
	*/
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.2f, 0.2f);
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);

	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4
	glm::mat4 modelaux(1.0);//Inicializar matriz de Modelo 4x4 auxiliar para la grua y perro
	glm::mat4 modelauxbase(1.0);//Inicializar matriz de Modelo 4x4 auxiliar Grua
	glm::mat4 modelauxtronco(1.0);//Inicializar matriz de Modelo 4x4 auxiliar (perro)
	glm::mat4 modelauxcabeza(1.0);//Inicializar matriz de Modelo 4x4 auxiliar (perro)



	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;
	glm::vec3 colorPerro = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar color del perro
	glm::vec3 colorArticulacion = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar color del perro
	glm::vec3 colorSoporte = glm::vec3(0.6f, 0.6f, 0.7f); //inicializar color del perro
	glm::vec3 colorOjos = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar color del perro
	glm::vec3 colorOrejas = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar color del perro
	glm::vec3 colorPatas = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar color del perro

	


	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cámara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		//la línea de proyección solo se manda una vez a menos que en tiempo de ejecución
		//se programe cambio entre proyección ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		//para reiniciar la matriz de modelo con valor de la matriz identidad
		model = glm::mat4(1.0);

		/*
		////////////////////////////////////////EJERCICIO 1/////////////////////////////////////////////////////////////////
		//Cabina
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));

		modelaux = model; 
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(2.0f, 6.0f, 5.0f));
		color = glm::vec3(0.6f, 0.3f, 0.1f); // Cabina de color cafe
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	
		meshList[2]->RenderMeshGeometry(); // Prisma de la cabina
		model = modelaux; 
	

		//Piramide cuadrangular (base de la grua)
		
		model = glm::translate(model, glm::vec3(0.0f, -2.1f, 0.0f));

		modelauxbase = model;
		model = glm::scale(model, glm::vec3(8.0f, 3.0f, 5.0f));
		color = glm::vec3(1.0, 0.5f, 0.0); // Base de color naranja
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[5]->RenderMeshGeometry(); //Base de la grua
		model = modelauxbase; 

		//Llanta 1 (izquierda parte enfrente)
		model = glm::translate(model, glm::vec3(-4.0f, -1.5f, 3.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		color = glm::vec3(0.5f, 0.5f, 0.5f); //Llantas de color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMeshGeometry(); //Llanta 1
		model = modelauxbase; 

		//Llanta 2 (derecha parte enfrente)
		model = glm::translate(model, glm::vec3(-4.0f, -1.5f, -3.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMeshGeometry(); //Llanta 2
		model = modelauxbase; 

		//Llanta 3 (izquierda parte atras)
		model = glm::translate(model, glm::vec3(4.0f, -1.5f, 3.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMeshGeometry(); //Llanta 3
		model = modelauxbase;

		//Llanta 4 (derecha parte atras)
		model = glm::translate(model, glm::vec3(4.0f, -1.5f, -3.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 1.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMeshGeometry(); //Llanta 4


		model = modelaux; 

		//SE EMPIEZA EL DIBUJO DE LOS BRAZOS
		//Articulación 1
		//rotación alrededor de la articulación que une con la cabina
		model = glm::translate(model, glm::vec3(-0.5f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f)); // Para mover la esfera de laprimera articulacion
		
		//Primer brazo que conecta con la cabina
		model = glm::translate(model, glm::vec3(-1.0f, 2.0f, 0.0f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		modelaux = model; // Guardo la ubicacion de la primera articulacion y continuo con las dimensiones del primer brazo
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 1.2f));
		color = glm::vec3(1.0f, 0.0f, 1.0f); // Primer Brazo Magenta
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));		

		meshList[0]->RenderMesh(); //Cubo del primer brazossssss
		model = modelaux;

		//Articulación 2 extremo izquierdo del segundo brazo
		model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));

		modelaux = model; 
		//Primera esfera brazo 1 y 2
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = modelaux; 

		//Segundo brazo
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));

		modelaux = model; // Guardo la ubicacion inicial del segundo brazo y continuo con sus dimensiones
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 0.0f); // Segundo Brazo Verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos

		meshList[0]->RenderMesh(); //Cubo del segundo brazo
		model = modelaux;		

		//Articulación 3 extremo derecho del segundo brazo
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		
		modelaux = model; 

		//Segunda esfera brazo 2 y 3
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = modelaux; 

		//Tercer brazo
		model = glm::translate(model, glm::vec3(-2.0f, -2.0f, 0.0f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		modelaux = model; 
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 0.0f); // Tercer Brazo amarillo
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos

		meshList[0]->RenderMesh(); //Cubo del tercer brazo
		model = modelaux; // Regreso a la posicion inicial del tercer brazo y continuo con las dimensiones de la cuarta articulacion

		//Articulación 4 extremo derecho del segundo brazo
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));

		modelaux = model; 
		
		//Tercera esfera brazo 3 y canasta
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render();
		model = modelaux; 

		////Canasta
		model = glm::translate(model, glm::vec3(0.35f, 0.75f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 2.0f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f); // Canasta de color azul
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); 

		meshList[0]->RenderMesh(); //Prisma Canasta

		////////////////////////////////////////EJERCICIO 1/////////////////////////////////////////////////////////////////
		*/


		

		////////////////////////////////////////EJERCICIO 2/////////////////////////////////////////////////////////////////
		
		
		//----------Colores-----------------//
		colorPerro = glm::vec3(0.65f, 0.65f, 0.75); // Color del perro (gris oscuro)
		colorArticulacion = glm::vec3(1.0f, 1.0f, 0.0f); // Color amarillo para las articulaciones
		colorPatas = glm::vec3(0.5f, 0.0f, 0.5f); // Color morado para las patas
		colorOjos = glm::vec3(1.0f, 0.0f, 0.0f); // Color rojo para los ojos
		colorOrejas = glm::vec3(0.6f, 0.6f, 0.7f); // Color gris para las orejas
		colorSoporte = glm::vec3(0.6f, 0.6f, 0.7f); // Color gris para el soporte


		//--------------Tronco---------------//
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -8.0f));
		modelauxtronco = model; 

		modelaux = model; 
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(4.0f, 10.0f, 6.0f));

		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPerro));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[0]->RenderMeshGeometry(); //Tronco (cubo)
		model = modelaux; 

		
		//---------Pata frontal izquierda---------------//
		//Articulación 1 Delantera Izquierda
		model = glm::translate(model, glm::vec3(-2.5f, -2.0f, 2.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; 

		//Esfera de arriba (pata 1)
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		sp.render();
		model = modelaux; 

		//Pata 1
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		modelaux = model; 

		model = glm::scale(model, glm::vec3(1.5f, 2.5f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatas));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[0]->RenderMesh(); //Primera pata (cubo)
		model = modelaux;			

		//Articulación 5 Delantera Izquierda
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; 

		//Segunda esfera (en medio)
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		sp.render();
		model = modelaux; 

		//Pata 5 
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		modelaux = model; 

		model = glm::scale(model, glm::vec3(1.5f, 2.25f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatas));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[0]->RenderMesh(); //Quinta pata (cubo)
		model = modelaux; 

		//Soporte de la pata
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 0.5f, 1.2f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSoporte));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		sp.render();
		model = modelauxtronco; 

		//---------------------Pata frontal derecha-------------------
		//Articulación 2 Delantera Derecha
		model = glm::translate(model, glm::vec3(-2.5f, -2.0f, -2.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; 

		//Segunda esfera Tronco y pata 2
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		sp.render();
		model = modelaux;

		//Pata 2
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		modelaux = model; 

		model = glm::scale(model, glm::vec3(1.5f, 2.5f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatas));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[0]->RenderMesh(); //Segunda pata (cubo)
		model = modelaux;			

		//Articulación 6 Delantera Derecha
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; 

		//Segunda esfera Pata 2 y 6
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		sp.render();
		model = modelaux; 

		//Pata 6 
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		modelaux = model; 

		model = glm::scale(model, glm::vec3(1.5f, 2.25f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatas));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[0]->RenderMesh(); //Sexta pata (cubo)
		model = modelaux; 

		//Soporte de la pata
		model = glm::translate(model, glm::vec3(-0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 0.5f, 1.2f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSoporte));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		sp.render();
		model = modelauxtronco;

		//--------------------------Pata trasera izquierda-----------------
		
		//Articulación 3 Trasera Izquierda
		model = glm::translate(model, glm::vec3(2.5f, -2.0f, 2.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; 

		//Tercera esfera Tronco y pata 3
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		sp.render();
		model = modelaux;

		//Pata 3
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		modelaux = model; 

		model = glm::scale(model, glm::vec3(1.5f, 2.5f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatas));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[0]->RenderMesh(); //Tercera pata (cubo)
		model = modelaux;			

		//Articulación 7 Trasera Izquierda
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;

		//Tercera esfera Pata 3 y 7
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		sp.render();
		model = modelaux; 

		//Pata 7
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		modelaux = model;

		model = glm::scale(model, glm::vec3(1.5f, 2.25f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatas));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[0]->RenderMesh(); //Septima pata (cubo)
		model = modelaux; 

		//Soporte de la pata
		model = glm::translate(model, glm::vec3(-0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 0.5f, 1.2f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSoporte));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		sp.render();
		model = modelauxtronco; 

		
		//-----------------------Pata trasera derecha-------------------------
		
		//Articulación 4 Trasera Derecha
		model = glm::translate(model, glm::vec3(2.5f, -2.0f, -2.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; 

		//Cuarta esfera Tronco y pata 4
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		sp.render();
		model = modelaux; 

		//Pata 4
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		modelaux = model; 

		model = glm::scale(model, glm::vec3(1.5f, 2.5f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatas));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[0]->RenderMesh(); //Cuarta pata (cubo)
		model = modelaux;			

		//Articulación 8 Trasera Derecha
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; 

		//Cuarta esfera Pata 4 y 8
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		sp.render();
		model = modelaux; 

		//Pata 8
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		modelaux = model; 

		model = glm::scale(model, glm::vec3(1.5f, 2.25f, 1.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPatas));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[0]->RenderMesh(); //Octava pata (cubo)
		model = modelaux; 

		//Soporte de la pata
		model = glm::translate(model, glm::vec3(-0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.2f, 0.5f, 1.2f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorSoporte));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		sp.render();
		model = modelauxtronco; 

	
		//--------------------------Cola---------------------------//
		
		//Articulación 11 
		model = glm::translate(model, glm::vec3(4.7f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model; 

		//Quinta esfera Tronco y Cola
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		sp.render();
		model = modelaux; 

		//Cola (cubo)
		model = glm::translate(model, glm::vec3(1.75f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 0.5f, 0.5f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPerro));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[0]->RenderMesh(); //Cola (cubo)
		model = modelauxtronco; 


		//-------------------Cabeza----------------------//

		model = glm::translate(model, glm::vec3(-3.0f, 2.0f, 0.0f));
		modelauxcabeza = model; 

		model = glm::scale(model, glm::vec3(2.0f, 6.0f, 1.5f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPerro));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[6]->RenderMeshGeometry(); //Cilindro
		model = modelauxcabeza;

		
		//----------------Oreja izquierda-------------------//
		
		//Articulación 9 
		model = glm::translate(model, glm::vec3(0.8f, 2.7f, 1.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model; 

		//Primera esfera (cabeza)
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		sp.render();
		model = modelaux; 

		//Oreja 1
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, 0.25f));
		model = glm::scale(model, glm::vec3(1.5f, 2.5f, 0.1f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorOrejas));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[0]->RenderMesh(); //Primera oreja (cubo)
		model = modelauxcabeza;			

		
		//------------------Oreja derecha---------------------//
		
		//Articulación 10 Delantera Derecha
		model = glm::translate(model, glm::vec3(0.8f, 2.7f, -1.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model; 

		//Segunda esfera (cabeza)
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		sp.render();
		model = modelaux; 

		//Oreja 2
		model = glm::translate(model, glm::vec3(0.0f, -0.75f, -0.25f));
		model = glm::scale(model, glm::vec3(1.5f, 2.5f, 0.1f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorOrejas));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[0]->RenderMesh(); //Segunda oreja (cubo)
		model = modelauxcabeza;		

		
		//--------------------Hocico--------------------//		

		model = glm::translate(model, glm::vec3(-2.2f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.2f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorArticulacion));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[6]->RenderMeshGeometry(); //Hocico (cilindro)
		model = modelauxcabeza;			


		
		//----------------Ojos------------------//

		model = glm::translate(model, glm::vec3(-0.4f, 2.3f, 0.0f));
		model = glm::scale(model, glm::vec3(1.95f, 1.0f, 1.25f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorOjos));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[6]->RenderMeshGeometry(); // Ojos (cilindro) 
		model = modelauxcabeza;			


		
		//--------------------Cabeza-----------------//

		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.95f, 1.0f, 1.55f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(colorPerro));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		sp.render();


		////////////////////////////////////////EJERCICIO 2/////////////////////////////////////////////////////////////////
		
		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}


