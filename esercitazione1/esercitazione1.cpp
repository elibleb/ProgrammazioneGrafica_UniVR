#include <glad/glad.h>
#include <GLFW/glfw3.h>
//GLM gestione delle trasformazioni
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\shader_m.h> 
//libreria per il caricamento delle immagini
#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\stb_image.h>
#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\camera.h>
//classe utilizzata per il caricamento dei modelli 3D
#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


// setta le dimensioni della finestra
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// setta le impostazioni camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f)); //posizione iniziale della camera, glm::vec3 crea un vettore tridimensionare, con
//spostamento sull'asse x, y, z . La camere rappresenta il punto di vista dell'utente
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// setto impostazioni per uniformare la velocita
float deltaTime = 0.0f;	// tempo che passa tra il frame precedente e quello attuale
float lastFrame = 0.0f;



int main()
{
    // GLWF: inizializzazione e configurazione

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //OpenGl versione 3.?
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //OpenGl versione 3.3
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //chiamiamo questa funzione ad 
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

    // inversione sull'asse y
    stbi_set_flip_vertically_on_load(true);

    // abilita il test di profondità per il rendering 3D, riga che configura il depth di Opengl (z-index)
    glEnable(GL_DEPTH_TEST);


    // Compila e collega gli shaders per gestire luci e modelli
    Shader lightingShader("lighting.vs", "lighting.fs");
    Shader modelShader("model.vs", "model.fs");

    // carica il modello, utilizzando Assimp
    Model ourModel("textures/backpack/backpack.obj");


    // impostare i vertici
    float vertices[] = {
        // posizione
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
        -0.5f,  0.5f, -0.5f
    };

    // Impostare le posizioni point lights, definite in un array di glm::vec3
    // Definisce un array di vettori tridimensionali che rappresentano le posizioni delle luci puntiformi nella scena
    glm::vec3 pointLightPositions[] = { //Ogni elemento dell'array rappresenta la posizione di una
                                        //sorgente di luce pointlight nella scena

        glm::vec3(2.0,  2.0f,  1.0f),   //prima luce: situata in alto a destra rispetto all'origine
        glm::vec3(-2.0f, -2.0f, -1.0f), //seconda luce: situata in basso a sinistra dietro l'origine
        glm::vec3(1.0f,  0.0f, 1.0f),   //Terza luce: situata in avanti e spostata a destra rispetto all'origine
        glm::vec3(0.0f,  0.0f, 0.0f)    //Quarta luce: situata esattamente all'origine.
    };


    //Configurazione VBO = vertex buffer object, contiene i dati dei vertici
    // VAO = vertex array object, contiene lo stato delle variabili di attributo dei vertici
    unsigned int VBO, modelVAO;
    glGenVertexArrays(1, &modelVAO); //Crea un oggetto VAO
    glGenBuffers(1, &VBO); //crea un oggetto VBO e lo associa alla variabile VBO

    // esegue il bind per associare la memoria
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Associa il VBO al buffer di tipo ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // glBufferData per riempire il buffer
    //con i dati dei vertici

    // posizione
    glBindVertexArray(modelVAO); //Associa il VAO appena creato

    // Il primo parametro è l'indice dell'attributo, lunghezza (3 per x, y, z), il tipo(GL_FLOAT), GL_FALSE non ci sono 
    // normalizzazione dei vettori e 3 * sizeof(float) perché ogni vertice ha 3 float
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); //Abilita l'attributo di vertice 0, che rappresenta la posizione dei vertici nel VBO.

    //VBO e VAO sono utilizzati anche per il cubo luminoso (lightCubeVAO) che rappresenta le luci nella scena
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // posizione
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //------------------------------------------------
    // CICLO DI RENDERING
    // -----------------------------------------------
    // continua finchè non gli viene detto di chiudere

    while (!glfwWindowShouldClose(window))
    {
        // gestione del delta time, per rendere fluida la scena
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // input
        processInput(window);

        // render
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f); //imposta il colore dello sfondo
        // resetta il buffer di profondità (depth), funzione che utilizza lo stato
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //inizio fase di rendering,cancella lo schermo, pulisce la finestra

        // attiva gli shader, programmi eseguiti sulla GPU per calcolare come devono apparire gli oggetti nella scena
        // (effetti di luce, colore, texture, ecc.)
        lightingShader.use();
        // view che si basa sulla posizione della camera
        lightingShader.setVec3("viewPos", camera.Position);
        // shiness settato a 32, definisce la lucentezza
        lightingShader.setFloat("material.shininess", 32.0f);

        // setta le proprietà delle luci
        // luce direzionale, da una direzione specifica, stesso effetto su tutti gli oggetti della scena
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1 : emettono luce in tutte le direzioni da una posizione specifica 
        lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[0].constant", 1.0f); // attenuazioni: valore alto, la luce non si attenua in modo costante
        lightingShader.setFloat("pointLights[0].linear", 0.09); //più alto è,più veloce la luce si attenua
        lightingShader.setFloat("pointLights[0].quadratic", 0.032); //attenuazione quadratica: più alto è,più veloce la luce si attenua con la distanza
        // point light 2
        lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.09);
        lightingShader.setFloat("pointLights[1].quadratic", 0.032);
        // point light 3
        lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[2].constant", 1.0f); 
        lightingShader.setFloat("pointLights[2].linear", 0.09); 
        lightingShader.setFloat("pointLights[2].quadratic", 0.032); 
        // point light 4
        // le coordinate y e z cambiano in base al tempo per far ruotare la luce
        float light3_mov_y = pointLightPositions[3].y + cos(glfwGetTime()) * 2.5;
        float light3_mov_z = pointLightPositions[3].z + sin(glfwGetTime()) * 2.0;
        //aggiorna posizione della luce nel lightingShader
        lightingShader.setVec3("pointLights[3].position", pointLightPositions[3].x, light3_mov_y, light3_mov_z);
        lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[3].constant", 1.0f);
        lightingShader.setFloat("pointLights[3].linear", 0.09);
        lightingShader.setFloat("pointLights[3].quadratic", 0.032);
        // spotlight: emette un fascio di luce concentrato da una posizione specifica e si propaga in una specifica direzione
        lightingShader.setVec3("spotLight.position", camera.Position); //posizione uguale a quella della camera, la luce seguirà la 
                                                                       //posizione della vista dell'utente
        lightingShader.setVec3("spotLight.direction", camera.Front); //direzione in cui la luce spotlight è proiettata,è la direzione in cui la
                                                                     //camera è orientata
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09);
        lightingShader.setFloat("spotLight.quadratic", 0.032);
        //angolo del fascio di luce, indicano dove la luce è concentrata e quando inizia a sfumare.
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(8.0f))); //oltre a questo angolo la luce sarà visibile
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(9.0f))); //oltre a questo angolo la luce smetterà di essere visibile e 
                                                                                        //gradualmente inizierà a sfumare


        // crea matrice di proiezione prospettica, converte zoom della camera in radianti, 
        // SCR_WIDTH/SCR_HEIGHT aiuta a mantenere correttamente le proporzioni della scena, 
        // piani vicino e lontano (minimo e massimo per render)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // creazione matrice view, trasormazione
        //descrive la posizione e l'orientamento della camera nello spazio
        glm::mat4 view = camera.GetViewMatrix();

        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        // trasformazione world 
        // creazione matrice modello, trasformazione applicata al cubo, nessuna
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // renderizza il modello del backpack
        // la traslazione posiziona il modello al centro della scena
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        // lo scale riduce la dimensione del modello
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        
        lightingShader.setMat4("model", model);
        ourModel.Draw(modelShader); //disegna il modello

        // renderizza le luci nella scena
        modelShader.use();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);

        // ciclo utilizzato per muovere la quarta point light attorno alla scena e traslare le altre 
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);

            if (i == 3) //movimento quarta luce
            {
                model = glm::translate(model, glm::vec3(pointLightPositions[3].x, light3_mov_y, light3_mov_z));
            }
            else //traslazione delle altre luci
                model = glm::translate(model, pointLightPositions[i]);

            model = glm::scale(model, glm::vec3(0.2f)); // scala per ridurre dimensione del cubo luminoso
            modelShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36); //disegna un cubo per ogni luce
        }

        // GLFW: scambia i buffer (per mostrare la scena appena disegnata)
        // e interroga gli eventi IO (tasti premuti, movimento del mouse etc.)o del mouse etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // GLFW: termina e pulisce tutte le risorse allocate da glfw
    glfwTerminate();
    return 0;
}




// Elabora tutti gli input: interroga GLFW se i tasi vengono premuti e rilasciati in questo frame 
// e reagisce di conseguenza 
// listener eventi IO che vengono poi gestiti da glfw
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
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

//GLFW: movimento del mouse
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
