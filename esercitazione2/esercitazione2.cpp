#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\shader_es2.h>

#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\stb_image.h>
#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\camera.h>
#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

unsigned int loadTexture(const char* path);
void renderScene(const Shader& shader);
void renderCube();

// setta le dimensioni della finestra
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool shadows = true;
bool shadowsKeyPressed = false;

// setta le impostazioni camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); //creo l'oggetto camera, posizione 3 unità lungo asse z
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// setto impostazioni per uniformare la velocità
float deltaTime = 0.0f; // tempo che passa tra il frame precedente e quello attuale
float lastFrame = 0.0f;

int main()
{

    // GLWF: inizializzazione e configurazione

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //usa solo funzionalità moderne di OpenGl


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

    glEnable(GL_DEPTH_TEST); //Test di profondità: rende oggetti più vicini visibili rispetto a quelli dietro
    glEnable(GL_CULL_FACE); //Culling delle facce: ignora facce non visibili per migliorare le prestazioni

    // shaders
    //Creazione di oggetti della classe Shader
    Shader shader("point_shadow.vs", "point_shadow.fs");
    Shader simpleDepthShader("point_shadow_depth.vs", "point_shadow_depth.fs", "point_shadow_depth.gs");

    //caricamento texture
    unsigned int woodTexture = loadTexture("textures/wood.png");

    /*Il shadow mapping (passaggio delle ombre) permette di calcolare le ombre generate da una luce puntiforme.
     Qui si usa una cube map per catturare la scena da ogni direzione intorno alla luce*/

     //configurazione depth map FBO
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024; //risoluzione shadow map
    unsigned int depthMapFBO; //FBO (Frame Buffer Object): Usato per renderizzare la profondità della scena
    glGenFramebuffers(1, &depthMapFBO); //FBO memorizza la mappa della profondità della scena
    // crea il depth cubemap texture
    unsigned int depthCubemap; //Cube Map: una texture 3D con sei facce (per ogni direzione della luce), cattura la profondità della scena
                               //da sei direzioni 
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (unsigned int i = 0; i < 6; ++i)
        //funzione per inizializzare una faccia, formato GL_DEPTH_COMPONENT per salvare valori di profondità
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    /*Configurazione dei parametri della CubeMap
    filtri NEAREST assicurano precisione per evitare interpolazione delle profondità
    CLAMP_TO_EDGE evita artefatti sui bordi della cube map.*/
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //la cubemap creata viene associata come profondità all'FBO usando GL_DEPTH_ATTACHMENT
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE); //disabilitati perchè ci interessa la profondità e non il colore
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shader.use();
    shader.setInt("diffuseTexture", 0);
    shader.setInt("depthMap", 1);

    //posizione luce
    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

    while (!glfwWindowShouldClose(window))
    {
        // gestione del delta time, per rendere fluida la scena
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //input
        processInput(window);
        // muove la posizione della luce in funzione del tempo, lungo l'asse z
        lightPos.z = static_cast<float>(sin(glfwGetTime() * 0.5) * 3.0);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // creo le depth cubemap transformation matrices
        float near_plane = 1.0f; //distanza dalla fotocamera al piano vicino, distanza minima in cui gli oggetti verranno renderizzati
        float far_plane = 25.0f; //distanza dalla fotocamera al piano lontano, distanza massima


        //Primo passaggio: la scena viene renderizzata dal punto di vista della luce (Proiezione e Trasformazione)
        
        //Proiezione prospettica: Crea un campo visivo di 90° per ogni faccia della cube map
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
        //shadowTransforms: Contiene le matrici di trasformazione per ogni direzione della luce
        //Ogni matrice rappresenta la vista della luce da una delle sei direzioni
        std::vector<glm::mat4> shadowTransforms;
        //glm::lookAt: Crea le sei visuali della luce (una per ogni faccia della cube map)ù
        //lightPos, lightPos + direzione: posizione verso la quale la telecamera è orientata, vettoreorientamento verticale della vista
        // della telecamera
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        // render della scena al depth cubemap
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        //Usa lo shader dedicato point_shadow_depth per calcolare la profondità della scena, la profondità è salvata nella cube map
        simpleDepthShader.use();

        //Per ogni faccia della cubemap, viene impostata una matrice di trasformazione
        //ogni matrice rappresenta la vista della luce da una delle 6 direzioni
        for (unsigned int i = 0; i < 6; ++i) 
            simpleDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
        simpleDepthShader.setFloat("far_plane", far_plane);
        simpleDepthShader.setVec3("lightPos", lightPos);
        renderScene(simpleDepthShader); //disegna tutti gli oggetti della scena
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // render normale della scena

        //secondo passaggio: rendering finale, applicazione delle ombre
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        shader.use(); //Viene utilizzato lo shader principale (point_shadow.vs e point_shadow.fs), 
        //che calcola le ombre e il rendering finale della scena

        // crea matrice di proiezione prospettica, converte zoom della camera in radianti, 
        // SCR_WIDTH/SCR_HEIGHT aiuta a mantenere correttamente le proporzioni della scena, 
        // piani vicino e lontano (minimo e massimo per render)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // creazione matrice view
        //descrive la posizione e l'orientamento della camera nello spazio
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        // set delle light uniform, uniform per la luce
        shader.setVec3("lightPos", lightPos); //lightPos: Posizione della luce
        shader.setVec3("viewPos", camera.Position); //viewPos: Posizione della camera
        shader.setInt("shadows", shadows); // shadows: attiva o disattiva le ombre
        shader.setFloat("far_plane", far_plane); //definisce la distanza alla quale le ombre sono visibili
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture); //carica texture del legno woodTexture
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap); //carica texture cubica contenente la depth map della luce, contiene info ombre
        renderScene(shader); //Renderizza tutti gli oggetti nella scena: una "stanza" (cubo grande scalato) e
        //Cubi più piccoli posizionati e scalati in punti specifici.
        //Le ombre sono applicate all'interno del Fragment Shader (point_shadow.fs)

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
// funzione effettuare il rendering della scena
void renderScene(const Shader& shader)
{
    //La scena contiene un cubo centrale (che funge da stanza) e altri oggetti più piccoli all'interno
    // Ogni oggetto è Trasformato con una matrice modello
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(5.0f)); //scala il cubo per creare una stanza più grande
    shader.setMat4("model", model);
    glDisable(GL_CULL_FACE); //per mostrare anche le facce interne del cubo, se è abilitato le facce nascoste non sono disegnate
    shader.setInt("reverse_normals", 1); // inversione delle normali per illuminazione corretta anche per le facce interne
    renderCube(); //disegna effettivamente il cubo, la stanza

    shader.setInt("reverse_normals", 0); //ripristina impostazioni
    glEnable(GL_CULL_FACE);

    // per tutti i cubi all'interno
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0)); //trasla cubo
    model = glm::scale(model, glm::vec3(0.5f)); //scala cubo più piccolo
    shader.setMat4("model", model);
    renderCube(); //Crea e disegna un cubo con posizioni, normali e coordinate texture e attributi configurati con VAO e VBO
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.75f));
    shader.setMat4("model", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.75f));
    shader.setMat4("model", model);
    renderCube();
}
// funzione effettuare il rendering di un singolo cubo
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    if (cubeVAO == 0)
    {
        float vertices[] = {
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
        };
        glGenVertexArrays(1, &cubeVAO); // crea VAO per il cubo
        glGenBuffers(1, &cubeVBO); // crea VBO 

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO); //associa VBO
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //caricamento vertici nel VBO

        glBindVertexArray(cubeVAO); //associa VAO
        glEnableVertexAttribArray(0); //abilita primo attributo vertici, 0 per posizione
        // Il primo parametro è l'indice dell'attributo (0 posizione, 1 normale e 2 texture), lunghezza (3 per x, y, z),
        // il tipo(GL_FLOAT), GL_FALSE non ci sono normalizzazione dei vettori e 8 * sizeof(float) perché ogni vertice ha 8 float
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0); //disasocia VBO
        glBindVertexArray(0); //disassocia VAO
    }

    glBindVertexArray(cubeVAO); //associa VAO del cubo
    glDrawArrays(GL_TRIANGLES, 0, 36); //disegna 36 triangoli del cubo
    glBindVertexArray(0); //disassocia VAO
}

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

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !shadowsKeyPressed)
    {
        shadows = !shadows;
        shadowsKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) //rilascia barra e resettato a false
    {
        shadowsKeyPressed = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse) //registrare posizione iniziale mouse solo per la prima volta
    {
        lastX = xpos; //posizione salvata qui per calcolarne poi i movimenti
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX; //cambiamento posizione mouse, differenza tra la posizione attuale e quella precedente
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset); //aggiorna posizione camera in base al movimento del mouse
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    //stbi_load() per caricare l'immagine e ottenere i suoi dati 
    //(larghezza, altezza, numero di canali (3 per RGB ecc), numero di componenti che vogliamo ottenere, 
    // se 0 carica il formato più adatto)
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1) //scala di grigi
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID); //lega la texture
        //carica i dati dell'immagine nella texture
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        // Se eccedono limiti S orrizzontali
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        // Se eccedono limiti T verticali
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        //Imposta il filtro di minificazione, filtro da usare quando la texture viene visualizzata in una dimensione 
        //più piccola di quella originale
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //Imposta il filtro di ingrandimento per quando la texture viene visualizzata con dimensioni maggiori 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}