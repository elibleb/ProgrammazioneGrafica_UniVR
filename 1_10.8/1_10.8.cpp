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

// setta le dimensioni della finestra
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// setto le impostazioni della camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); //posizione della camera nello spazio, un po' più lontano dall'origine per vedere la scena
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); //indica dove la camera sta guardando
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //indica quale direzione è in alto rispetto alla camera

bool firstMouse = true; //per sapere se il mouse ha già spostato la finestra
float yaw = -90.0f; //angolo di rotazione della camera sull'asse Y orizzontale
float pitch = 0.0f; //angolo di rotazione della camera sull'asse X verticale
float lastX = 800.0f / 2.0; //coordinate dell'ultima posizione, calcolano offset per aggiornare la direzione della camera
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
    glfwSetCursorPosCallback(window, mouse_callback); //posizione mouse
    glfwSetScrollCallback(window, scroll_callback); //scroll mouse

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
    Shader shader("1_10.8_camera.vs", "1_10.8_camera.fs");



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
    // posizione dei cubi
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
    // Il primo parametro di glVertexAttribPointer è l'indice dell'attributo
    // (0 per la posizione, 1 per texture). Viene definita la lunghezza (3 per x, y, z),
    // il tipo(GL_FLOAT) e il passo (5 * sizeof(float) perché ogni vertice ha 5 float)
    // Anche texture segue lo stesso pattern
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // coordinate delle texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture: Questo pezzo si occupa di caricare e creare la texture
    unsigned int texture1, texture2;
    // carica e crea la prima texture
    glGenTextures(1, &texture1); // Crea una texture vuota e la memorizza nell'array texture
    glBindTexture(GL_TEXTURE_2D, texture1); //associa la texture appena creata al target GL_TEXTURE_2D
    // imposta i parametri di wrapping della tessitura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Imposta il comportamento della texture quando le coordinate
        //S (orizzontali) eccedono i limiti. GL_REPEAT fa sì che la texture venga ripetuta
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// stesso comportamento per le coordinate T(verticali)
    // imposta i parametri di filtraggio della tessitura
    //filtro di minificazione, imposta il filtro da usare quando la texture viene visualizzata in una dimensione 
    //più piccola di quella originale
    // GL_LINEAR -> filtro lineare
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //Imposta il filtro di ingrandimento per quando la texture viene visualizzata con dimensioni maggiori 
    // GL_LINEAR -> filtro lineare
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // carica immagine, crea la texture e genera le mipmaps
    int width, height, nrChannels;
    // riflette l'immagine sul'asse y, inverte verticalmente l'immagine durante il caricamento
    stbi_set_flip_vertically_on_load(true);
    //stbi_load() dalla libreria stb_image per caricare l'immagine e ottenere i suoi dati 
    //(larghezza, altezza, numero di canali (3 per RGB ecc), numero di componenti che vogliamo ottenere, 
    // se 0 carica il formato più adatto)
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
        glClearColor(0.2f, 0.4f, 0.6f, 1.0f); //imposta colore sfondo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // resetta il buffer di profondità (depth), funzione che utilizza lo stato

        // binda le texture sui rispettivi texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // attivazione shader
        shader.use();

        // passa la projection matrix allo shader (in questo caso cambia ad ogni frame, di solito era fuori dal ciclo)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //crea matrice di proiezione prospettica, FOV angolo di campo visivo (angolo di visualizzazione della scena) 
        // SCR_WIDTH/SCR_HEIGHT aiuta a mantenere correttamente le 
        // proporzioni della scena, piani vicino e lontano per il rendering (minimo e massimo, oggetti sforano non verranno renderizzati)
        shader.setMat4("projection", projection);

        // trasformazione camera/view
        //creazione matrice view, definisce la posizione e l'orientamento della camera nella scena
        //cameraPos + cameraFront è il punto che la camera guarda
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setMat4("view", view);


        // render degli oggetti
        glBindVertexArray(VAO);
        //per ogni cubo viene calcolata una matrice model che definisce la posizione e la rotazione del cubo nello spazio
        for (unsigned int i = 0; i < 10; i++)
        {
            // passa la matrice modello agli shader prima di disegnarlo
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i; //angolo aumenta ad ogni iterazione
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

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
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
    float xpos = static_cast<float>(xposIn); //conversioni posizioni 
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) //inizializzazione posizione mouse
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // è invertito perchè le coordinate y vanno dal basso verso l'alto
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // controlla che lo schermo non venga capovolto se si va fuori limite
    //limite di angolo di pitch
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

//GLFW: viene chiamata ogni volta che si usa la rotella del mouse
//utilizzata per cambiare campo visivo FOV

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}