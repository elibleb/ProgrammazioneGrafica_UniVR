#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\shader_m.h>
#define STB_IMAGE_IMPLEMENTATION //Definisce la macro che permette di includere e utilizzare la libreria stb_image per caricare
//immagini, in questo caso la texture
#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\stb_image.h>


#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// setta le dimensioni della finestra
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // chiamiamo questa funzione ad 
    //ogni ridimensionamento della finestra

    // GLAD: carica tutti i puntatori alle funzioni OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Riga che configura il depth di Opengl (z-index)
    glEnable(GL_DEPTH_TEST);

    // shaders
    //Creazione di un oggetto della classe Shader
    //carica compila e collega i due shader, un vertex shader e un fragment shader
    Shader shader("1_9.9_3_shader.vs", "1_9.9_3_shader.fs");



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

    // posizioni dei cubi nel world space
    // array di glm::vec3, posizioni dei cubi x,y,z
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



    // posizione
    // Il primo parametro di glVertexAttribPointer è l'indice dell'attributo
    // (0 per la posizione, 1 per texture). Viene definita la lunghezza (3 per x, y, z),
    // il tipo(GL_FLOAT) e il passo (5 * sizeof(float) perché ogni vertice ha 5 float)
    // Anche texture segue lo stesso pattern
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // coordinate texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Questo pezzo si occupa di caricare e creare la texture
    unsigned int texture1, texture2;
    // carica e crea la prima texture
    glGenTextures(1, &texture1); // Crea una texture vuota e la memorizza nell'array texture
    glBindTexture(GL_TEXTURE_2D, texture1); //associa la texture appena creata al target GL_TEXTURE_2D,
    //tutte le successive operazioni GL_TEXTURE_2D da qui in poi avranno effetto su questo oggetto texture
    // imposta i parametri di wrapping della tessitura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Imposta il comportamento della texture quando le coordinate
        //S (orizzontali) eccedono i limiti. GL_REPEAT fa sì che la texture venga ripetuta
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// stesso comportamento per le coordinate T(verticali)
    // imposta i parametri di filtraggio della tessitura
    //Imposta il filtro di minificazione, imposta il filtro da usare quando la texture viene visualizzata in una dimensione 
    //più piccola di quella originale
    // GL_LINEAR -> filtro lineare
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //Imposta il filtro di ingrandimento per quando la texture viene visualizzata con dimensioni maggiori 
    // GL_LINEAR -> filtro lineare
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // carica l'immagine, crea la tessitura e genera le mipmaps
    int width, height, nrChannels;
    // riflette l'immagine sul'asse y, inverte verticalmente l'immagine durante il caricamento
    stbi_set_flip_vertically_on_load(true);
    //stbi_load() dalla libreria stb_image per caricare l'immagine e ottenere i suoi dati 
    //(larghezza, altezza, numero di canali (3 per RGB ecc), numero di componenti che vogliamo ottenere, 
    // se 0 carica il formato più adatto)
    stbi_set_flip_vertically_on_load(true);
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
    // imposta i parametri di wrapping della tessitura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // imposta i parametri di filtraggio della tessitura
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

    // dice a opengl a quale textur unit appartiene ogni sampler 
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
        // input
        processInput(window);


        // render
        glClearColor(0.678f, 0.847f, 1.0f, 1.0f); //imposta il colore dello sfondo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // resetta il buffer di profondità (depth), funzione che utilizza lo stato

        // binda le texture sui rispettivi texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // attivazione shader
        shader.use();


        glm::mat4 view = glm::mat4(1.0f); //view simula la posizione della camera, matrice identità
        glm::mat4 projection = glm::mat4(1.0f); //projection definisce la proiezione 3D
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //crea una matrice di
        //proiezione prospettica che simula la visione di una scena 3D, FOV angolo di campo visivo (angolo di visualizzazione della
        // scena) di 45 gradi,SCR_WIDTH/SCR_HEIGHT aiuta a mantenere correttamente le proporzioni della scena 3D, piani vicino e
        // lontano per il rendering (minimo e massimo, oggetti sforano non verranno renderizzati)
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));//traslazione alla matrice view, la "telecamera" viene spostata
        //lungo l'asse Z di -3.0 unità, è posizionata "dietro" rispetto all'oggetto, così l'oggetto è visibile al centro della scena
        
        // passa le matrici allo shader
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // render degli oggetti
        glBindVertexArray(VAO);
        //per ogni cubo viene calcolata una matrice model che definisce la posizione e la rotazione del cubo nello spazio
        for (unsigned int i = 0; i < 10; i += 1)
        {
            // passa la matrice modello agli shader prima di disegnarlo
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]); //traslazione cubo i
            float angle = 20.0f * i; //angolo aumenta ad ogni iterazione
            if (i % 3 == 0) { //ruota continuamente un cubo ogni tre
                angle = glfwGetTime() * 25.0f;
            }
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f)); // ruota la matrice traslata in precedenza
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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
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

