#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Carga modelos
#include "SOIL2/SOIL2.h"

// Declaración de funciones
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void animacion_bajo();

// Dimensiones de la ventana
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Cámara
Camera  camera(glm::vec3(5.0f, 5.0f, 20.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Atributos de la luz
glm::vec3 lightPos(.0f, .0f, 0.0f);

//Variables de animaciones

//Lámpara//
bool activeL = false; //Booleano que funciona como switch para la animación de la lámpara.
glm::vec3 lampOn;	  //Es un vec3 con los colores que produce la luz. Es la consecuencia del bool anterior.
glm::vec3 pointLightPositions(0.0f, 8.2f, -12.0f); // Posición de la pointlight (en la lámpara)
glm::vec3 Light1 = glm::vec3(0); //Luz de la pointlight

//Puerta del frente
bool activePF = false; //Booleano que funciona como switch para la animación de la puerta.
float rotPF = 0.f;	   //Variable usada en el rotate de la puerta para producir la animación.
bool dirPF = false;	   //Booleano que decide el sentido en que se moverá la puerta.

//Radio
bool activeR = false;	//Booleano que funciona como switch para la animación de la radio.
bool dirRI = false;		//Booleano que decide el sentido en que se moverá la perilla izquierda del radio.
bool dirRD = false;		//Booleano que decide el sentido en que se moverá la perilla derecha del radio.
float rotRI = 0.f;		//Ángulo de rotación de la perilla izquierda del radio.
float rotRD = 0.f;		//Ángulo de rotación de la perilla derecha del radio.
float tiempo;			//Variable independiente en las función seno del archivo anim.vs

//Bajo
glm::vec3 PosIni(-3.95f, .55f, 0.0f); //Posición inicial del objeto para iniciar la animación. Punto de origen para el objeto
float movKitX = 0.0;	//Variable que se sumará a su posición en el eje X.
float movKitY = 0.0;	//Variable que se sumará a su posición en el eje Y.
float movKitZ = 0.0;	//Variable que se sumará a su posición en el eje Z.
float rotKitX = 0.0;	//Variable que hará rotar al objeto en el eje X.
float rotKitY = 0.0;	//Variable que hará rotar al objeto en el eje Y.
float rotKitZ = 0.0;	//Variable que hará rotar al objeto en el eje Z.

bool circuito = false;	//Booleano que funciona como switch para la animación del bajo. Permite que el recorrido se lleve a cabo.
bool recorrido1 = true;		//Paso 1 en el recorrido.
bool recorrido2 = false;	//Paso 2 en el recorrido.
bool recorrido3 = false;	//Paso 3 en el recorrido.
bool recorrido4 = false;	//Paso 4 en el recorrido.
bool recorrido5 = false;	//Paso 5 en el recorrido.
bool recorrido6 = false;	//Paso 6 en el recorrido.
bool recorrido7 = false;	//Paso 7 en el recorrido.
bool recorrido8 = false;	//Paso 8 en el recorrido.
bool recorrido9 = false;	//Paso 9 en el recorrido.
bool recorrido10 = false;	//Paso 10 en el recorrido.
////////////////////////////////////////////////////////////////////////////////////////

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};





// Deltatime
GLfloat deltaTime = 0.0f;	// Tiempo entre el último frame y el actual
GLfloat lastFrame = 0.0f;  	// Tiempo del último frame


int main() {
	// Init GLFW
	glfwInit();

	//Crea un objeto GLFWwindow que se pueda usar para las funciones de GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	//Establece las funciones de callback necesarias
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	//Declaración de funciones de tipo Shader
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader Anim("Shaders/anim.vs", "Shaders/anim.frag");

	//Definición de los modelos

	//Fachada
	Model Casa((char*)"Models/casaMarceline/casa.obj");
	Model VentanaPF((char*)"Models/casaMarceline/vpf.obj");
	Model VentanaPL((char*)"Models/casaMarceline/vpl.obj");
	Model PuertaF((char*)"Models/casaMarceline/puertaFrente.obj");
	Model PuertaL((char*)"Models/casaMarceline/puertaLado.obj");
	Model PuertaP((char*)"Models/casaMarceline/puertaPerro.obj");
	Model VentanaF((char*)"Models/casaMarceline/ventanaFrente.obj");
	Model VentanaL((char*)"Models/casaMarceline/ventanaLado.obj");

	//Objetos
	//Declarados de tipo Model para poder traer los OBJ a OpenGL
	Model Bajo((char*)"Models/bajoMarceline/bajoMarceline.obj");
	Model Amp((char*)"Models/ampMarceline/ampMarceline.obj");
	Model Escalera((char*)"Models/escaleraMarceline/escaleraMarceline.obj");
	Model LamparaMetal((char*)"Models/lamparaMarceline/lamparaMarcelineMetal.obj");
	Model Lampara((char*)"Models/lamparaMarceline/lamparaMarceline.obj");
	Model Radio((char*)"Models/radioMarceline/radioMarceline.obj");
	Model RadioPantalla((char*)"Models/radioMarceline/pantalla.obj");
	Model RadioPI((char*)"Models/radioMarceline/perillaI.obj");
	Model RadioPD((char*)"Models/radioMarceline/perillaD.obj");
	Model Sofa3((char*)"Models/sofaMarceline/sofaMarceline.obj");
	Model Sofa1((char*)"Models/sofaMarceline1/sofaMarceline1.obj");
	Model Mesa((char*)"Models/mesaMarceline/mesaMarceline.obj");
	Model TablaMesa((char*)"Models/tablaMarceline/tablaMarceline.obj");
	Model Refrigerador((char*)"Models/refriMarceline/refriMarceline.obj");
	Model Estufa((char*)"Models/estufaMarceline/estufaMarceline.obj");

	//Configuración del VAO y VBO
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Atributos de posición
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Atributos de la normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Configuración de las unidades de textura
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	//Proyección en perspectiva
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	//Configuración cíclica donde se establecen los atributos de luz y modelos. Se detiene cuando el usuario lo decida.
	while (!glfwWindowShouldClose(window)) {

		//DeltaTime del frame actual
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Revisa si ha ocurrido algún evento (como presionar alguna tecla o movimiento del ratón)
		//Mantiene en loop a las animaciones (animacíon())
		glfwPollEvents();
		DoMovement();
		animacion_bajo();

		//Limpia el colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Habilita opciones de openGL
		glEnable(GL_DEPTH_TEST);

		//Usa el shader correspondiente al configurar objetos uniforms/drawing
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		//Iluminación
		//Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.f, 1.f, 1.f);

		//Point light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions.x, pointLightPositions.y, pointLightPositions.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lampOn.x, lampOn.y, lampOn.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lampOn.x, lampOn.y, lampOn.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), .44f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), .44f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		//Configura las propiedades del material
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		//Crea la transformación de la cámara
		glm::mat4 view;
		view = camera.GetViewMatrix();

		//Obtiene la ubicación de las uniforms
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		//Pasa de matrices a shaders
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//Carga de modelo 
		glm::mat4 model(1);
		view = camera.GetViewMatrix();

		//Fachada
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Casa.Draw(lightingShader);

		//Puerta del frente
		model = glm::mat4(1);
		model = translate(model, glm::vec3(1.05f, 3.5f, 9.f));
		model = glm::rotate(model, glm::radians(rotPF), glm::vec3(.0f, 1.f, 0.f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		PuertaF.Draw(lightingShader);

		//Puerta lateral
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		PuertaL.Draw(lightingShader);

		//Puerta para el perro
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		PuertaP.Draw(lightingShader);

		//Objetos
				//Amplificador 
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Amp.Draw(lightingShader);

		//Bajo
		model = glm::mat4(1);
		model = glm::translate(model, PosIni + glm::vec3(movKitX, movKitY, movKitZ));
		model = glm::rotate(model, glm::radians(rotKitZ), glm::vec3(0.0f, 0.0f, 1.0));
		model = glm::rotate(model, glm::radians(rotKitX), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotKitY), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Bajo.Draw(lightingShader);

		//Escalera
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Escalera.Draw(lightingShader);

		//Parte metálica (no transparente) de la lámpara
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		LamparaMetal.Draw(lightingShader);

		model = glm::mat4(1);
		model = translate(model, glm::vec3(.0f, .0f, -12.f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		LamparaMetal.Draw(lightingShader);

		//RADIO
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Radio.Draw(lightingShader);

		//Perilla izquierda
		model = glm::mat4(1);
		model = translate(model, glm::vec3(-4.42f, 1.1f, 5.6f));
		model = glm::rotate(model, glm::radians(44.f), glm::vec3(0.0f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(rotRI), glm::vec3(1.0f, 0.f, 0.f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		RadioPI.Draw(lightingShader);

		//Perilla derecha
		model = glm::mat4(1);
		model = translate(model, glm::vec3(-4.9f, 1.1f, 5.1f));
		model = glm::rotate(model, glm::radians(44.f), glm::vec3(0.0f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(rotRD), glm::vec3(1.0f, 0.f, 0.f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		RadioPD.Draw(lightingShader);

		//Sofá para 1
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Sofa1.Draw(lightingShader);

		//Sofá para 3
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Sofa3.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Mesa.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		TablaMesa.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Estufa.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Refrigerador.Draw(lightingShader);

		//OBJETOS TRANSPARENTES

		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Ventanas de la puerta del frente
		model = glm::mat4(1);
		model = translate(model, glm::vec3(1.05f, 3.5f, 9.f));
		model = glm::rotate(model, glm::radians(rotPF), glm::vec3(.0f, 1.f, 0.f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 0.0, 0.215, 0.29, 0.99);
		VentanaPF.Draw(lightingShader);

		//Ventanas de la puerta lateral
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 0.0, 0.215, 0.29, 0.99);
		VentanaPL.Draw(lightingShader);

		//Ventana frontal
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 0.0, 0.215, 0.29, 0.99);
		VentanaF.Draw(lightingShader);

		//Ventana lateral
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 0.0, 0.215, 0.29, 0.99);
		VentanaL.Draw(lightingShader);

		//Parte lumínica de la lámpara (cristal)
		model = glm::mat4(1);
		model = translate(model, glm::vec3(.0f, .0f, -12.f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.99);
		Lampara.Draw(lightingShader);

		model = glm::mat4(1);
		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.99);
		Lampara.Draw(lightingShader);

		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 1.0); //Reset para el resto de cosas

		//Pantalla de la radio
		Anim.Use();
		modelLoc = glGetUniformLocation(Anim.Program, "model");
		viewLoc = glGetUniformLocation(Anim.Program, "view");
		projLoc = glGetUniformLocation(Anim.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(Anim.Program, "time"), tiempo);
		RadioPantalla.Draw(Anim);

		glBindVertexArray(0);

		//Fin de la transparencia

				//Se vuelve a dibujar el objeto lamp para el shader apropiado
		lampShader.Use();
		//Obtiene objetos d eubicación para las matrices en el lampshader
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		//Configura las matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//Dibuja el objeto de luz
		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//Intercambia los búfferes de pantalla
		glfwSwapBuffers(window);
	}

	//Termina GLFW. Limpia todos los recursos utilizados por GLFW
	glfwTerminate();

	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement() {
	//Controles de la cámara
	if (keys[GLFW_KEY_W]) {
		camera.ProcessKeyboard(FORWARD, deltaTime * 2);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
		camera.ProcessKeyboard(BACKWARD, deltaTime * 2);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		camera.ProcessKeyboard(LEFT, deltaTime * 2);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		camera.ProcessKeyboard(RIGHT, deltaTime * 2);
	}

	//Abre y cierra la puerta
	if (rotPF >= 85.f) {
		dirPF = false;
	}
	else if (rotPF <= 0.f) {
		dirPF = true;
	}

	if (activePF) {
		if (dirPF)
			rotPF += 1.f;
	}
	else {
		if (!dirPF)
			rotPF -= 1.f;
	}

	//Enciende // apaga la radio
	if (activeR) {
		tiempo = glfwGetTime();

		if (rotRI >= 180.f)
			dirRI = false;
		else if (rotRI <= 0.f)
			dirRI = true;

		if (dirRI)
			rotRI += 0.5f;
		else
			rotRI -= 0.8f;

		if (rotRD >= 85.f)
			dirRD = false;
		else if (rotRD <= 0.f)
			dirRD = true;

		if (dirRD)
			rotRD += 1.5f;
		else
			rotRD -= 1.f;
	}
	else {
		tiempo = 0.f;
	}
}

void animacion_bajo() {
	//Movimiento del bajo
	if (circuito) {
		if (recorrido1) {
			rotKitZ += 0.08 + (rotKitZ / 10);
			if (rotKitZ > 13.6) {
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2) {
			rotKitY -= 2.88 - (rotKitY / 20);
			if (rotKitY < -15.1) {
				recorrido2 = false;
				recorrido3 = true;
			}
		}

		if (recorrido3) {
			if (movKitX < 0.911)
				movKitX += (0.05061111111112 - movKitX / 20);
			rotKitZ += (0.996944444444444445 - rotKitZ / 50);
			if (rotKitZ > 32.5) {
				recorrido3 = false;
				recorrido4 = true;
			}
		}

		if (recorrido4) {
			movKitX += 0.035;
			rotKitZ -= (0.25 + rotKitZ / 130);
			if (rotKitZ < 31.0) {
				recorrido4 = false;
				recorrido5 = true;
			}
		}

		if (recorrido5) {
			movKitX += 0.0083;
			rotKitZ += (0.05 + (rotKitZ / 120));
			if (rotKitZ > 38.8) {
				recorrido5 = false;
				recorrido6 = true;
			}
		}

		if (recorrido6) {
			rotKitZ += (0.05 + (rotKitZ / 120));
			if (rotKitZ > 40.) {
				recorrido6 = false;
				recorrido7 = true;
			}
		}

		if (recorrido7) {
			if (movKitX <= 1.9)
				movKitX += 0.005 + (movKitX / 40);
			rotKitZ += (0.05 + (rotKitZ / 40));
			if (rotKitZ > 72.) {
				recorrido7 = false;
				recorrido8 = true;
			}
		}

		if (recorrido8) {
			rotKitZ = 72.8;
			recorrido8 = false;
			recorrido9 = true;
			circuito = !circuito;
		}

		if (recorrido9) {
			rotKitY = -13.8;
			recorrido9 = false;
			recorrido10 = true;
		}

		if (recorrido10 && circuito) {
			movKitX = 0.0;
			rotKitY = 0.0;
			rotKitZ = 0.0;
			recorrido10 = false;
			recorrido1 = true;
		}
	}
}

//Se llama cuando alguna tecla es presionada vía GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_L]) {
		activeL = !activeL;
		if (activeL) {
			lampOn.x = 1.0f;
			lampOn.y = 1.0f;
			lampOn.z = 0.717647f;
			Light1 = glm::vec3(1.0f, 1.0f, .0f);
		}
		else {
			lampOn = glm::vec3(0.0f);
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}

	if (keys[GLFW_KEY_P]) {
		activePF = !activePF;
	}

	if (keys[GLFW_KEY_R]) {
		activeR = !activeR;
	}

	if (keys[GLFW_KEY_B]) {
		circuito = !circuito;
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}
