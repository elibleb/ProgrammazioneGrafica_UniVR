#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\shader_m.h>
#define STB_IMAGE_IMPLEMENTATION //Definisce la macro che permette di includere e utilizzare la libreria stb_image per caricare
//immagini
#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\stb_image.h>
#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\camera.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// setto le dimensioni della finestra
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// setta le impostazioni camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); //creo l'oggetto camera, posizione 3 unità lungo asse z
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// setto impostazioni per uniformare la velocita
float deltaTime = 0.0f; // tempo che passa tra il frame precedente e quello attuale
float lastFrame = 0.0f;

// imposto la posizione della sorgente di luce nello spazio
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

    //cubo principale
    Shader lightingS("2_14.3_materials.vs", "2_14.3_materials.fs");
    //cubo piccolo sorgente di luce
    Shader lightCubeS("2_14.3_light_cube.vs", "2_14.3_light_cube.fs");


    //impostare i dati dei vertici (e i buffer) e configurare gli attributi dei vertici 

    //normali utilizzate nei calcoli di illuminazione per determinare l'orientamento della superficie del cubo rispetto 
    //alla sorgente luminosa. Passate al fs

    float vertices[] = {
        //posizione             //normale (per ogni faccia del cubo)
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
    // Il primo parametro è l'indice dell'attributo (0 posizione, 1 normale), lunghezza (3 per x, y, z),
    // il tipo(GL_FLOAT), GL_FALSE non ci sono normalizzazione dei vettori e 6 * sizeof(float) perché ogni vertice ha 6 float
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normale
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // VAO per il cubo che rappresenta la sorgente di luce, configurato per usare lo stesso VBO che contiene i vertici
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // imposta gli attributi dei vertici
    // posizione
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //------------------------------------------------
    // CICLO DI RENDERING
    // -----------------------------------------------
    // continua finchè non gli viene detto di chiudere

    while (!glfwWindowShouldClose(window))
    {

        // codice per gestire il delta time
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // input
        processInput(window);


        // render
        glClearColor(0.5f, 0.7f, 0.5f, 1.0f); //imposta colore sfondo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // resetta il buffer di profondità (depth), funzione che utilizza lo stato

        // attiva shader cubo principale e setta uniform
        lightingS.use();
        lightingS.setVec3("light.position", lightPos); //posizione della sorgente di luce nello spazio
        lightingS.setVec3("viewPos", camera.Position); //invia la posizione della camera (osservatore) nello spazio, valore viene usato per
                                        //calcolare l'illuminazione speculare, che dipende dall'angolo tra la vista (camera) e la luce 


        //setta il colore delle varie luci, luminosità impostata al massimo
        lightingS.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
        lightingS.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
        lightingS.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // proprietà del materiale

        //colore ambientale del materiale, illuminazione base che l'oggetto riflette in modo uniforme da tutte le direzioni
        lightingS.setVec3("material.ambient", 0.0f, 0.1f, 0.06f);
        //colore diffuso del materiale, riflessione diffusa della luce, dipende dall'angolo della luce rispetto alla superficie
        lightingS.setVec3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f); //ciano
        // colore speculare del materiale, quanto l'oggetto riflette la luce in modo speculare
        lightingS.setVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
        lightingS.setFloat("material.shininess", 32.0f); //impsosta la lucentezza del materiale

        // crea matrice di proiezione prospettica, converte zoom della camera in radianti, 
        // SCR_WIDTH/SCR_HEIGHT aiuta a mantenere correttamente le proporzioni della scena, 
        // piani vicino e lontano (minimo e massimo per render)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // creazione matrice view
        //descrive la posizione e l'orientamento della camera nello spazio
        glm::mat4 view = camera.GetViewMatrix();

        lightingS.setMat4("projection", projection);
        lightingS.setMat4("view", view);

        // trasformazione world
        // creazione matrice modello, trasformazione applicata al cubo, nessuna
        glm::mat4 model = glm::mat4(1.0f);
        lightingS.setMat4("model", model);

        glBindVertexArray(cubeVAO); //lega il VAO del cubo principale,contiene dati dei vertici e le info sugli 
        //attributi dei vertici
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightCubeS.use();
        lightCubeS.setMat4("projection", projection);
        lightCubeS.setMat4("view", view); //posizione e orientamento telecamera
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
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// GLFW: viene chiamata ogni volta che si usa la rotella del mouse

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
