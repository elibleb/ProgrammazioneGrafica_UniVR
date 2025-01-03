#include <glad/glad.h>
#include <GLFW/glfw3.h>

//definisce la classe Shader che gestisce la compilazione, l'uso e la gestione degli shader
#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\shader.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// setta le dimensioni finestra
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
    Shader shader("1_6.8_3_shader.vs", "1_6.8_3_shader.fs");



    //impostare i dati dei vertici (e i buffer) e configurare gli attributi dei vertici 

    float vertices[] = {
        // posizione         // colore rgb
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // basso destra
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // basso sinistra
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // alto 
    };

    // VBO = vertex buffer object, contiene i dati dei vertici (posizione e colore)
    // VAO = vertex array object, contiene lo stato delle variabili di attributo dei vertici
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // associare (binda) inizialmente l'oggetto Vertex Array (VBO), 
    // poi binda e imposta i buffer dei vertici e configura gli attributi dei vertici
    //(binda e aggiunge valore del VBO)
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Carica i dati dei vertici nel buffer, il parametro GL_STATIC_DRAW indica che i dati non cambieranno frequentemente
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // setta gli attributi dei vertici

    // posizione
    //Ogni vertice ha due attributi, posizione e colore
    //Il primo parametro di glVertexAttribPointer è l'indice dell'attributo
    // (0 per la posizione, 1 per il colore). Viene definita la lunghezza (3 per x, y, z),
    // il tipo(GL_FLOAT) e il passo (6 * sizeof(float) perché ogni vertice ha 6 float)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // colore
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    //------------------------------------
    // CICLO DI RENDERING
    //------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        // input
        //come l'ESC per uscire
        processInput(window);


        // render
        glClearColor(1.0f, 0.7f, 0.8f, 1.0f); //colore sfondo
        glClear(GL_COLOR_BUFFER_BIT); //pulisce il buffer dai colori e imposta colore sfondo

        // setta l'uniform
        float offset = 0.5f;
        shader.setFloat("offset", offset);

        // disegna il triangolo
        shader.use(); //utilizza il programma shader
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); //disegna il triangolo utilizzando vertici definiti nel VBO

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