#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\shader_m.h>
#define STB_IMAGE_IMPLEMENTATION //Definisce la macro che permette di includere e utilizzare la libreria stb_image per caricare
//immagini (come texture)
#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\stb_image.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
//calcola matrice di visione utilizzata per determinare la vista della camera nello spazio
glm::mat4 calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp); //prende posizione della camera
//target punto nello spazio in cui è rivolta la telecamera, vettore che rappresenta orientamento verso l'alto del mondo

// setta le dimensioni della finestra
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//dichiarazione variabili per telecamera, posizione, direzione, orientamento
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); //posizione della telecamera nello spazio, 3 unità asse z
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); //direzione di visione, camera guarda lungo asse negativo z
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //vettore che definisce la direzione verso l'alto

bool firstMouse = true; //prima volta che il mouse si muove
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0; //inizializzate posizione attuale (al centro), per calcolo movimenti successivi. 
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// setto impostazioni per uniformare la velocita
float deltaTime = 0.0f; // tempo che passa tra il frame precedente e quello attuale
float lastFrame = 0.0f;

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
    //Creazione di un oggetto della classe Shader
    //carica compila e collega i due shader, un vertex shader e un fragment shader
    Shader shader("1_10.9_camera.vs", "1_10.9_camera.fs");



    //impostare i dati dei vertici (e i buffer) e configurare gli attributi dei vertici 

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };



    // VBO = vertex buffer object, contiene i dati dei vertici
    // VAO = vertex array object, contiene lo stato delle variabili di attributo dei vertici
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



    // imposta gli attributi dei vertici
    // posizione
    // Il primo parametro è l'indice dell'attributo (0 per la posizione, 1 per texture), lunghezza (3 per x, y, z),
    // il tipo(GL_FLOAT) e 5 * sizeof(float) perché ogni vertice ha 5 float
    // Anche texture segue lo stesso pattern
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // coordinate delle texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture: Questo pezzo si occupa di caricare e creare la texture
    unsigned int texture1, texture2;
    // carica e crea la prima texture
    glGenTextures(1, &texture1);  // Crea una texture vuota e la memorizza nell'array texture
    glBindTexture(GL_TEXTURE_2D, texture1); //associa la texture appena creata al target GL_TEXTURE_2D
    // setta i parametri del wrapping della texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Comportamento della texture quando le coordinate
        //S (orizzontali) eccedono i limiti. GL_REPEAT fa sì che la texture venga ripetuta
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// stesso comportamento per le coordinate T(verticali)

    // setta i parametri di filtraggio della tessitura
    //filtro di minificazione, filtro da usare quando la texture viene visualizzata in una dimensione più piccola di quella originale
    // GL_LINEAR -> filtro lineare
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //Imposta il filtro di ingrandimento per quando la texture viene visualizzata con dimensioni maggiori 
    // GL_LINEAR -> filtro lineare
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // carica immagine, crea la texture e genera le mipmaps
    int width, height, nrChannels;
    // riflette l'immagine sul'asse y, inverte verticalmente l'immagine durante il caricamento
    stbi_set_flip_vertically_on_load(true);
    //caricare l'immagine e ottenere i suoi dati numero di canali, 
    // numero di componenti che vogliamo ottenere, se 0 carica il formato più adatto)
    unsigned char* data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {   //se i dati sono stati caricati con successo

        //carica i dati dell'immagine nella texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //Genera le mipmaps per la texture
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    // carica e crea la seconda texture
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // setta i parametri del wrapping della texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // imposta i parametri di filtraggio della texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // carica l'immagine, crea la tessitura e genera le mipmaps
    data = stbi_load("textures/awesomeface.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    // libero la memoria
    stbi_image_free(data);

    // dice a opengl a quale texture unit appartiene ogni sampler 
    shader.use();
    // imposto lo shader prima di impostare l'uniform
    //lo setto attraverso lo shader, texture1 è associata al texture unit 0, texture2 a 1
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);


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
        glClearColor(0.2f, 0.4f, 0.6f, 1.0f); //imposto il colore dello sfondo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // resetta il buffer di profondità (depth), funzione che utilizza lo stato

        // binda le texture sui rispettivi texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // attivazione shader
        shader.use();

        // crea matrice di proiezione prospettica, SCR_WIDTH/SCR_HEIGHT aiuta a mantenere correttamente le 
        // proporzioni della scena, piani vicino e lontano (minimo e massimo)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // passa la projection matrix allo shader (in questo caso cambia ad ogni frame)
        shader.setMat4("projection", projection);

        // funzione lookAt di glm rimpiazzata con "calculate_lookAt_matrix" 
        //glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        //prende posizione, direzione e direzione verso l'alto
        glm::mat4 view = calculate_lookAt_matrix(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setMat4("view", view);


        // render degli oggetti
        glBindVertexArray(VAO);
        //per ogni cubo calcolata una matrice model che definisce la posizione e la rotazione del cubo nello spazio
        for (unsigned int i = 0; i < 10; i++)
        {
            // passa la matrice modello agli shader prima di disegnarlo
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f)); // ruota la matrice traslata
            //intorno a un asse con componenti (1, 0.3, 0.5)
            shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        // GLFW: scambia i buffer e interroga gli eventi IO (tasti premuti, movimento del mouse etc.)o del mouse etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-alloco le risorse non più necessarie:
    glDeleteVertexArrays(1, &VAO);
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

    //cameraSpeed gestione velocità movimento camera
    float cameraSpeed = static_cast<float>(2.5 * deltaTime);//la camera si muoverà indipendentemente da quanto velocemente
    //vengono renderizzati i frame
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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

    if (firstMouse) //mouse si muove per la prima volta
    {
        lastX = xpos; // inizializzate a posizione attuale per calcolare movimenti successivi
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX; //offset rispetto posizione precednete
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; //sensibilità movimento mouse
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset; //aggiornamento angolo rotazione
    pitch += yoffset;

    if (pitch > 89.0f) //Limitazione angolo pitch per rotazione completa camera
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front); //nuovo vettore che indica la direzione in cui la telecamera sta guardando
}

// GLFW: viene chiamata ogni volta che si usa la rotella del mouse
// gestisce zoom della telecamera, il campo visivo FOV viene modificato
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset; //fov aumenta o diminuisce in base allo scroll del mouse
    if (fov < 1.0f) //limitato tra 1 e 45
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

// lookAt funzione generata nuova
glm::mat4 calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
{
    glm::vec3 zaxis = glm::normalize(position - target); //vettore che punta dalla telecamera al punto di destinazione
    //normalizzazione per avere vettore unitario

    glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis)); //direzione asse x vista (destra), ortogonale a z
    //prodotto vettoriale worldUp e zaxis e normalizzazione

    glm::vec3 yaxis = glm::cross(zaxis, xaxis); //direzione su vista, ortogonale a z e x, prodotto vettoriale

    //creazione matrice di traslazione
    glm::mat4 translation = glm::mat4(1.0f);
    translation[3][0] = -position.x;
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;
    //creazione matrice di rotazione
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = xaxis.x;
    rotation[1][0] = xaxis.y;
    rotation[2][0] = xaxis.z;
    rotation[0][1] = yaxis.x;
    rotation[1][1] = yaxis.y;
    rotation[2][1] = yaxis.z;
    rotation[0][2] = zaxis.x;
    rotation[1][2] = zaxis.y;
    rotation[2][2] = zaxis.z;

    return rotation * translation; //matrice finale, definisce la vista della camera (dove è posizionata e dove guarda)
}