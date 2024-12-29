#include <glad/glad.h> // includi file header di GLAD e GLFW
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// impostazioni dimensioni finestra
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// shaders
// vertexShader: prende la posizione di un vertice e restituisce
// la posizione trasformata (gl_Position) e un colore rosso (vertexColor)
// Calcola la posizione di ogni vertice e altre informazioni come il colore (in questo caso, vertexColor)
// che vengono quindi passate al fragment shader tramite la variabile vertexColor
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n" //definisce una variabile di tipo vec4 che è un valore a 4 componenti (rosso, verde, blu, alfa)
                          //che sarà passato al fragment shader
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n" // viene passato un valore al fragment shader
"}\0";

//fragment shader: Passa il colore del vertice al frammento e lo usa per colorare il pixel
//riceve quindi i dati attraverso la variabile di tipo in (in vec4 vertexColor), utilizza questi dati (per colorare il pixel) 
//nel calcolo finale del colore del frammento (FragColor)
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n" // Questa è la variabile che riceve il valore dal vertex shader
"void main()\n"
"{\n"
"   FragColor = vertexColor;\n" // Qui il valore passato dal vertex shader viene utilizzato, FragColor è la variabile di output
                                // del fragment shader che determina il colore finale del pixel
"}\n\0";

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

    //--------------------------------------
    //COSTRUIRE E COMPILARE PROGRAMMA SHADER
    //--------------------------------------

    //vertex shader
    //Crea un vertex shader: carica il codice sorgente e lo compila
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //controllo eventuali errori relativi alla compilazione del vertex shader
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    // crea un fragment shader: carica il codice sorgente e lo compila
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // controllo se ci sono errori nella compilazione del fragment shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // collegare gli shader (link)
    //Crea un programma di shader, allega il vertex shader e il fragment shader e li collega insieme per formare
    //un programma di rendering eseguibile
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // controllo errori relativi al collegamento
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    //eliminare gli oggetti shader una volta collegati al programma in oggetto (non ne abbiamo più bisogno)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //-------------------------------------------------------------------------------
    //impostare i dati dei vertici (e i buffer) e configurare gli attributi dei vertici
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // sinistra
         0.5f, -0.5f, 0.0f, // destra
         0.0f,  0.5f, 0.0f  // top
    };

    // VBO = vertex buffer object
    // VAO = vertex array object

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // associare (binda) inizialmente l'oggetto Vertex Array (VBO), 
    // poi binda e imposta i buffer dei vertici e configura gli attributi dei vertici
    //(binda e aggiunge valore del VBO)

    //Associa il VAO e il VBO, e copia i dati dei vertici nel VBO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // imposta gli attributi dei vertici
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // adesso si può fare l'unbind, la chiamata a glVertexAttribPointer ha registrato VBO come oggetto buffer 
    // di vertice associato dell'attributo vertex, quindi possiamo separare tranquillamente
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Questa linea serve per disegnare i poligoni in wireframe.
    // Si può decommentare.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);




    //------------------------------------------------
    // CICLO DI RENDERING
    // -----------------------------------------------
    // continua finchè non gli viene detto di chiudere

    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);


        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //imposta colore sfondo e pulisce lo schermo
        glClear(GL_COLOR_BUFFER_BIT); 

        // disegna il triangolo
        //usa il programma shader e disegna il triangolo
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO); //avendo un solo VAO non è necessario bindarlo,  lo teniamo per una 
        //migliore organizzazione
        glDrawArrays(GL_TRIANGLES, 0, 3); //glDrawArrays: OpenGL si occupa automaticamente di passare i dati da vertexColor
                                          //dal vertex shader al fragment shader per ogni frammento generato per il triangolo


        // GLFW: scambia i buffer e interroga gli eventi IO (tasti premuti, movimento del mouse etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-alloco le risorse non più necessarie. 
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

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
    // si assicura che il viewport corrisponda alla nuova dimensione della finestra; notare che 
    // su display a retina le dimensioni della finestra sono molto maggiori di quelle specificate
    glViewport(0, 0, width, height);
}