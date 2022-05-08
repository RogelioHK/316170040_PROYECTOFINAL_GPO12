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

//Load Models
#include "SOIL2/SOIL2.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void animacion();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(5.0f, 5.0f, 20.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(.0f, .0f, 0.0f);

//Variables de animaciones
bool activeL = false;
bool activePF = false;
bool activeR = false;
bool dirPF = false;
bool dirRI = false;
bool dirRD = false;
glm::vec3 lampOn;
float tiempo;
float rot = 0.0f;
float rotPF = 0.f;
float rotRI = 0.f;
float rotRD = 0.f;

//Animación del bajo
glm::vec3 PosIni(-3.95f, .55f, 0.0f);
float movKitX = 0.0;
float movKitY = 0.0;
float movKitZ = 0.0;
float rotKitX = 0.0;
float rotKitY = 0.0;
float rotKitZ = 0.0;

bool circuito = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;
bool recorrido6 = false;
bool recorrido7 = false;
bool recorrido8 = false;
bool recorrido9 = false;
bool recorrido10 = false;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,8.2f, 0.0f)
};

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



glm::vec3 Light1 = glm::vec3(0);
//glm::vec3 Light2 = glm::vec3(0);
//glm::vec3 Light3 = glm::vec3(0);
//glm::vec3 Light4 = glm::vec3(0);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
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



	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader Anim("Shaders/anim.vs", "Shaders/anim.frag");

	Model Casa((char*)"Models/casaMarceline/casa.obj");
	Model VentanaPF((char*)"Models/casaMarceline/vpf.obj");
	Model VentanaPL((char*)"Models/casaMarceline/vpl.obj");
	Model PuertaF((char*)"Models/casaMarceline/puertaFrente.obj");
	Model PuertaL((char*)"Models/casaMarceline/puertaLado.obj");
	Model PuertaP((char*)"Models/casaMarceline/puertaPerro.obj");
	Model VentanaF((char*)"Models/casaMarceline/ventanaFrente.obj");
	Model VentanaL((char*)"Models/casaMarceline/ventanaLado.obj");

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


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);



		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.f, 1.f, 1.f);


		// Point light 1

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lampOn.x,lampOn.y,lampOn.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lampOn.x,lampOn.y,lampOn.z);
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

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(0.f,1.f,0.f));
		//model = glm::rotate(model, glm::radians(rot), glm::vec3(0.f, 1.f, 0.f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Casa.Draw(lightingShader); //Casa

		////////////////////////////////////Elementos de la fachada/////////////////////////////
		model = glm::mat4(1);
		model = translate(model,glm::vec3(1.05f,3.5f,9.f));
		model = glm::rotate(model, glm::radians(rotPF), glm::vec3(.0f, 1.f, 0.f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		PuertaF.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		PuertaL.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		PuertaP.Draw(lightingShader);
		////////////////////////////////////////////////////////////////////////////////////

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

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		LamparaMetal.Draw(lightingShader);

		///////////////////////////////RADIO////////////////////////////////////////////////
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Radio.Draw(lightingShader);

		model = glm::mat4(1);
		model = translate(model, glm::vec3(-4.42f, 1.1f, 5.6f));
		model = glm::rotate(model, glm::radians(44.f), glm::vec3(0.0f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(rotRI), glm::vec3(1.0f, 0.f, 0.f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		RadioPI.Draw(lightingShader);

		model = glm::mat4(1);
		model = translate(model, glm::vec3(-4.9f, 1.1f, 5.1f));
		model = glm::rotate(model, glm::radians(44.f), glm::vec3(0.0f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(rotRD), glm::vec3(1.0f, 0.f, 0.f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		RadioPD.Draw(lightingShader);
		/////////////////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Sofa1.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Sofa3.Draw(lightingShader);
		
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

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 0.0, 0.215, 0.29, 0.99);
		VentanaPL.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 0.0, 0.215, 0.29, 0.99);
		VentanaF.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 0.0, 0.215, 0.29, 0.99);
		VentanaL.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 0.99);
		Lampara.Draw(lightingShader);

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
		///////////////////////////////////////////////////////////////////////////////////////////
		
		//Fin de la transparencia
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlfa"), 1.0, 1.0, 1.0, 1.0); //Reset para el resto de cosas

		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		for (GLuint i = 0; i < 1; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
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
			rotPF += .5f;
	}
	else {
		if(!dirPF)
		rotPF -= .5f;
	}

	//Enciende // apaga la radio
	if (activeR) {
		tiempo = glfwGetTime();
	
		if (rotRI >= 180.f)
			dirRI = false;
		else if (rotRI <= 0.f)
			dirRI = true;

		if (dirRI)
			rotRI += 3.f;
		else
			rotRI -= 4.f;

		if (rotRD >= 85.f)
			dirRD = false;
		else if (rotRD <= 0.f)
			dirRD = true;

		if (dirRD)
			rotRD += 5.f;
		else
			rotRD -= 2.f;
	
	}
	else {
		tiempo = 0.f;
	
	
	}
}

void animacion(){
	//Movimiento del bajo
	if (circuito)
	{
		if (recorrido1){
			rotKitZ += 0.08+(rotKitZ/10);
			if (rotKitZ > 13.6){
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2){
			rotKitY -= 2.88 - (rotKitY/20);
			if (rotKitY < -15.1){
				recorrido2 = false;
				recorrido3 = true;
			}
		}

		if (recorrido3){
			if(movKitX < 0.911)
				movKitX += (0.05061111111112 -movKitX/20);
			rotKitZ += (0.996944444444444445 - rotKitZ/50);
			if (rotKitZ > 32.5) {
				recorrido3 = false;
				recorrido4 = true;
			}
		}

		if (recorrido4) {
			movKitX += 0.035;
			rotKitZ -= (0.25 + rotKitZ/130);
			if (rotKitZ < 31.0) {
				recorrido4 = false;
				recorrido5 = true;
			}
		}

		if (recorrido5) {
			movKitX += 0.0083;
			rotKitZ += (0.05 + (rotKitZ/120));
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
			if(movKitX <= 1.9)
				movKitX += 0.005 + (movKitX/40);
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

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_L]){
		activeL = !activeL;
		if (activeL)
		{
			lampOn.x = 1.0f;
			lampOn.y = 1.0f;
			lampOn.z = 0.717647f;
			Light1 = glm::vec3(1.0f, 1.0f, .0f);
		}
		else
		{
			lampOn = glm::vec3(0.0f);
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}

	if (keys[GLFW_KEY_P]){
		activePF = !activePF;
	}

	if (keys[GLFW_KEY_R]) {
		activeR = !activeR;
	}

	if (keys[GLFW_KEY_B]) {
		circuito = !circuito;
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
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