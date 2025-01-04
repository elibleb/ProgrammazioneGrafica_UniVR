#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\shader.h>
#define STB_IMAGE_IMPLEMENTATION //Definisce la macro che permette di includere e utilizzare la libreria stb_image per caricare
//immagini, in questo caso la texture
#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\stb_image.h>


#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// impostazioni dimensioni finestra
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // chiamiamo questa funzione ad 
    //ogni ridimensionamento della finestra

    // GLAD: carica tutti i puntatori alle funzioni OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // shaders
    //Creazione di un oggetto della classe Shader
    //carica compila e collega i due shader, un vertex shader e un fragment shader
    Shader shader("1_7.6_shader.vs", "1_7.6_shader.fs");



    //impostare i dati dei vertici (e i buffer) e configurare gli attributi dei vertici 

    float vertices[] = {
        // posizione         // colore           // coordinate texture
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // alto destra
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // basso destra
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // basso sinistra
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // alto sinistra
    };
    unsigned int indices[] = {
        0, 1, 3, // primo
        1, 2, 3  // secondo
    };


    // VBO = vertex buffer object, contiene i dati dei vertici (posizione e colore)
    // VAO = vertex array object, contiene lo stato delle variabili di attributo dei vertici
    // EBO = Element Buffer Object, contiene gli indici per i vertici, 
    // che consentono di ottimizzare il rendering evitando la duplicazione di vertici comuni
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    // associare (binda) inizialmente l'oggetto Vertex Array (VBO), 
    // poi binda e imposta i buffer dei vertici e configura gli attributi dei vertici
    //(binda e aggiunge valore del VBO)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // imposta gli attributi dei vertici
    // Ogni glVertexAttribPointer specifica come i dati devono essere interpretati

    // posizione
    // Il primo parametro di glVertexAttribPointer è l'indice dell'attributo
    // (0 per la posizione, 1 per il colore, 2 per texture). Viene definita la lunghezza (3 per x, y, z),
    // il tipo(GL_FLOAT) e il passo (8 * sizeof(float) perché ogni vertice ha 8 float)
    // Anche colore e texture seguono lo stesso pattern
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // colore
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // coordinate texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // texture: Questo pezzo si occupa di caricare e creare la texture
    unsigned int texture;
    glGenTextures(1, &texture); // Crea una texture vuota e la memorizza nell'array texture. Qui stiamo creando una sola texture
        //quindi il primo parametro è 1
    glBindTexture(GL_TEXTURE_2D, texture); //associa la texture appena creata al target GL_TEXTURE_2D,
                            //tutte le successive operazioni GL_TEXTURE_2D da qui in poi avranno effetto su questo oggetto texture
    
                            // setta i parametri di wrapping della tessitura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// Imposta il comportamento della texture quando le coordinate
                                        //S (orizzontali) eccedono i limiti [0, 1]. GL_REPEAT fa sì che la texture venga ripetuta.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // stesso comportamento per le coordinate T(verticali)

    // setta i parametri di filtraggio della tessitura

    //Imposta il filtro di minificazione, ossia definisce come OpenGL deve gestire la texture quando viene visualizzata con dimensioni più piccole. 
    // GL_LINEAR_MIPMAP_LINEAR -> vengono utilizzate le mipmaps con interpolazione lineare
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //Imposta il filtro di ingrandimento per quando la texture viene visualizzata con dimensioni maggiori 
    // GL_LINEAR -> filtro lineare
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Ora carica l'immagine, crea la tessitura e genera le mipmaps
    int width, height, nrChannels;
    //stbi_load() dalla libreria stb_image per caricare l'immagine e ottenere i suoi dati 
    //(larghezza, altezza, numero di canali, numero di componenti che vogliamo ottenere, se 0 carica il formato più adatto).
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
    //Una volta che la texture è stata caricata possiamo liberare la memoria occupata dall'immagine originale
    stbi_image_free(data);

    //------------------------------------
    // CICLO DI RENDERING
    //------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);


        // render
        glClearColor(0.7f, 0.9f, 0.9f, 1.0f); //colore sfondo
        glClear(GL_COLOR_BUFFER_BIT); //pulisce il buffer dai colori e imposta colore sfondo

        // binda le texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // disegna il triangolo
        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // GLFW: scambia i buffer e interroga gli eventi IO (tasti premuti, movimento del mouse etc.)o del mouse etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-alloco le risorse non più necessarie:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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

