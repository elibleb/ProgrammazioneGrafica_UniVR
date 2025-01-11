#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\shader_m.h>
#define STB_IMAGE_IMPLEMENTATION //Definisce la macro che permette di includere e utilizzare la libreria stb_image per caricare
//immagini (come texture)
#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\stb_image.h>
#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\camera.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// setta le dimensioni della finestra
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// setta le impostazioni camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); //creo l'oggetto camera, posizione 3 unità lungo asse z
float lastX = SCR_WIDTH / 2.0f; //valori per calcolare movimento drl mouse
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true; //se il mouse è dentro la finestra per la prima volta

// setto impostazioni per uniformare la velocita
float deltaTime = 0.0f; // tempo che passa tra il frame precedente e quello attuale
float lastFrame = 0.0f;

// imposto la posizione della sorgente di luce nello spazio, vettore 3D
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    // GLWF: inizializzazione e configurazione

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLWF: creazione finestra
    // restituisce un oggetto GLFWwidow

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // rendiamo il contesto della nostra finestra il contesto principale
    // del thread corrente
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // chiamiamo questa funzione ad 
    //ogni ridimensionamento della finestra
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // dice a GLFW di catturare il mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLAD: carica tutti i puntatori alle funzioni OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Riga che configura il depth di Opengl (z-index)
    glEnable(GL_DEPTH_TEST);

    //shaders
    //Creazione di oggetti della classe Shader

    //gestisce il rendering del cubo principale con l'illuminazione applicata
    Shader lightingS("2_12.1_colors.vs", "2_12.1_colors.fs"); 
    //gestisce il rendering del cubo piccolo che rappresenta la sorgente di luce
    Shader lightCubeS("2_12.1_light_cube.vs", "2_12.1_light_cube.fs");


    //impostare i dati dei vertici (e i buffer) e configurare gli attributi dei vertici 

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    };



    // VBO = vertex buffer object, contiene i dati dei vertici
    // VAO = vertex array object, contiene lo stato delle variabili di attributo dei vertici
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



    // imposta gli attributi dei vertici
    // posizione
    // Il primo parametro è l'indice dell'attributo, lunghezza (3 per x, y, z),
    // il tipo(GL_FLOAT) e 3 * sizeof(float) perché ogni vertice ha 3 float
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // VBO e VAO per il cubo che rappresenta la sorgente di luce
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO); //genera un VAO per memorizzare lo stato degli attributi dei vertici
    glBindVertexArray(lightCubeVAO); //lega il VAO appena creato, tutte le operazioni successive eseguite su di esso
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //associa il buffer dei vertici, qui memorizzati i dati del cubo
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //imposta gli attributi dei vertici
    glEnableVertexAttribArray(0); //abilita attributo di vertice alla posizione 0, utilizzato per il rendering


    //------------------------------------------------
    // CICLO DI RENDERING
    // -----------------------------------------------
    // continua finchè non gli viene detto di chiudere

    while (!glfwWindowShouldClose(window))
    {

        //gestione del delta time, per rendere fluida la scena
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // input
        processInput(window);


        // render
        glClearColor(0.0f, 0.3f, 0.9f, 1.0f); //imposto il colore dello sfondo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // resetta il buffer di profondità (depth), funzione che utilizza lo stato

        //attivo shader per colori
        lightingS.use();//gestisce scena principale
        lightingS.setVec3("objectColor", 1.0f, 0.5f, 0.31f); //imposta colore cubo grande arancione
        lightingS.setVec3("lightColor", 1.0f, 1.0f, 1.0f); //imposta colore della luce bianco

        // trasformazioni view/projection
        // crea matrice di proiezione prospettica, converte zoom della camera in radianti, 
        // SCR_WIDTH/SCR_HEIGHT aiuta a mantenere correttamente le proporzioni della scena, 
        // piani vicino e lontano (minimo e massimo per render)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        // creazione matrice view
        //descrive la posizione e l'orientamento della camera nello spazio
        glm::mat4 view = camera.GetViewMatrix(); 

        lightingS.setMat4("projection", projection);
        lightingS.setMat4("view", view);

        // creazione matrice modello, trasformazione applicata al cubo (in questo caso nessuna)
        glm::mat4 model = glm::mat4(1.0f);
        lightingS.setMat4("model", model);

        // render degli oggetti
        glBindVertexArray(cubeVAO); //lega il VAO del cubo principale,contiene dati dei vertici e le info sugli 
                                    //attributi dei vertici
        glDrawArrays(GL_TRIANGLES, 0, 36); //disegna il cubo con 36 triangoli
        
        //redering del cubo sorgente di luce
        lightCubeS.use();
        lightCubeS.setMat4("projection", projection);
        lightCubeS.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // scala, più piccolo dell'altro cubo
        lightCubeS.setMat4("model", model);
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // GLFW: scambia i buffer e interroga gli eventi IO (tasti premuti, movimento del mouse etc.)o del mouse etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-alloco le risorse non più necessarie:
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // GLFW: termina e pulisce tutte le risorse allocate da glfw.
    glfwTerminate();

    return 0;

}




// Elabora tutti gli input: interroga GLFW se i tasi vengono premuti e rilasciati in questo frame 
// e reagisce di conseguenza 
// listener eventi IO che vengono poi gestiti da glfw

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}



// GLFW: ogni volta che la finestra viene modificata (dall'utente o dal SO), viene eseguita 
// questa funzione di callback 
// listener del resize della finestra
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // si assicura che il viewport corrisponda alla nuova dimensione della finestra;
    // su display a retina le dimensioni della finestra sono molto maggiori di quelle specificate
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn); //conversioni posizioni 
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) //mouse si muove per la prima volta
    {
        lastX = xpos; // inizializzate a posizione attuale per calcolare movimenti successivi
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX; //offset rispetto posizione precedente
    float yoffset = lastY - ypos; // invertito poiché le coordinate y vanno dal basso verso l'alto

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// GLFW: viene chiamata ogni volta che si usa la rotella del mouse

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}