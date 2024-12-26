#include <glad/glad.h> // includi file header di GLAD e GLFW
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// impostazioni dimensioni finestra
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//shaders
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    // GLFW: inizializzazione e configurazione
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Versione major e minor 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Profilo principale, 
                                                             // sottoinsieme più piccolo di funzionalità

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: creazione finestra
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
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //controllo eventuali errori relativi alla compilazione degli shader
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

     // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // controllo eventuali errori relativi alla compilazione degli shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // collegare gli shader (link)
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
        -0.5f, -0.5f, 0.0f, //sinistra
         0.5f, -0.5f, 0.0f, //destra
         0.0f,  0.5f, 0.0f //alto
    };

    //VBO = vertex buffer object
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // associare (binda) inizialmente l'oggetto Vertex Array (VBO), 
    // poi binda e imposta i buffer dei vertici e configura gli attributi dei vertici
    //(binda e aggiunge valore del VBO)
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // adesso si può fare l'unbind, la chiamata a glVertexAttribPointer ha registrato VBO come oggetto buffer 
    // di vertice associato dell'attributo vertex, quindi possiamo separare tranquillamente
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);



    //------------------------------------------------
    // CICLO DI RENDERING
    // -----------------------------------------------
    // continua finchè non gli viene detto di chiudere

    while (!glfwWindowShouldClose(window))
    {
        // input
        
        processInput(window);

        // GLFW: scambia i buffer e interroga gli eventi IO (tasti premuti, movimento del mouse etc.)

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // funzione che setta lo stato
        glClear(GL_COLOR_BUFFER_BIT); // funzione che utilizza lo stato

        //disegna il triangolo
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); //avendo un solo VAO non è necessario bindarlo,  lo teniamo per una 
        //migliore organizzazione
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // GLFW: scambia i buffer e interroga gli eventi IO (tasti premuti, movimento del mouse etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-alloco le risorse non più necessarie. Non è necessario
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // GLFW: termina e pulisce tutte le risorse allocate da glfw.
    glfwTerminate();
    return 0;
}

// Elabora tutti gli input: interroga GLFW se i tasi vengono premuti e rilasciati in questo frame 
// e reagisce di conseguenza (listener eventi IO che vengono poi gestiti da glfw)

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: ogni volta che la finestra viene modificata (dall'utente o dal SO), viene eseguita 
// questa funzione di callback (listener del resize della finestra)

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // si assicura che il viewport corrisponda alla nuova dimensione della finestra; notare che 
    // su display a retina le dimensioni della finestra sono molto maggiori di quelle specificate
    glViewport(0, 0, width, height);
}