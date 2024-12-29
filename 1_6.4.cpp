#include <glad/glad.h> // includi file header di GLAD e GLFW
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// impostazioni dimensioni finestra
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Un uniform è una variabile che viene dichiarata nello shader e che può essere impostata dal programma prima 
//che venga eseguito lo shader. Di solito viene usato per passare valori che devono essere condivisi 
//tra tutti i vertici o frammenti di una singola esecuzione di rendering (colori, trasformazioni, texture o
//parametri di illuminazione)
//In questo esercizio l'uniform viene usato per passare un colore dallo shader al fragment shader 
//Questo permette di aggiornare dinamicamente il colore dell'oggetto renderizzato

//gli uniform quindi forniscono un modo per inviare dati dallo shader program agli shaders senza dover inviare dati 
//per ogni vertice o pixel individualmente


//shaders
//Vertex Shader è un programma che riceve le informazioni sui vertici e calcola la posizione dei vertici nello spazio 3D.
//aPos è un attributo di vertice (posizione), che viene passato al gl_Position (la posizione finale del vertice nello spazio 2D).
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";

//Fragment Shader è un programma che calcola il colore di ogni pixel
// Riceve un valore uniform chiamato ourColor e lo assegna al colore finale del pixel FragColor
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n" //variabile che permette di passare un valore globale (un colore) dallo shader program

"void main()\n"
"{\n"
"   FragColor = ourColor;\n" //L'uniform ourColor non ha un valore definito all'interno dello shader, 
                             //il suo valore sarà impostato dal codice C++ prima che lo shader venga eseguito
"}\n\0";


//Il colore verde del triangolo cambia dinamicamente nel tempo
//grazie alla manipolazione del colore tramite un uniform nel fragment shader

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
    //crea un vertex shader, carica il codice sorgente e lo compila
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
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

    //fragment shader
    //crea un fragment shader, carica il codice sorgente e lo compila
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // controllo se ci sono errori nella compilazione degli shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //collegare gli shader (link)
    //collega i due shader (vertex e fragment) a un programma shader e lo compila

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
    unsigned int VBO, VAO; //Memorizzano rispettivamente i dati dei vertici e gli attributi dei vertici
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
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //funzione che setta lo stato
        glClear(GL_COLOR_BUFFER_BIT); // funzione che usa lo stato


        //attiva il programma shader per l'esecuzione
        glUseProgram(shaderProgram);

        //aggiorna l'uniform dello shader
        //il valore dell'uniform viene passato allo shader prima di eseguire il rendering, utilizzando la funzione glUniform4f

        double timeValue = glfwGetTime(); //Ottiene il tempo corrente tramite glfwGetTime()
        float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5); //Calcola un valore verde oscillante
                                                                    //nel tempo utilizzando una funzione seno (sin(timeValue))

        //cerca la posizione dell'uniform chiamato ourColor nel programma shader attualmente in uso
        //Passa questo valore come colore uniforme (ourColor) allo shader. Il colore del triangolo 
        //sarà verde e cambierà dinamicamente con il tempo
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);//imposta il valore dell'uniform ourColor con un valore di
                                                                       //colore vec4


        glBindVertexArray(VAO); //avendo un solo VAO non è necessario bindarlo,  lo teniamo per una 
        //migliore organizzazione
        glDrawArrays(GL_TRIANGLES, 0, 3); //disegna il triangolo utilizzando i vertici e gli shader configurati


        // GLFW: scambia i buffer e interroga gli eventi IO (tasti premuti, movimento del mouse etc.)
        glfwSwapBuffers(window); //scambia i buffer mostrando il disegno sullo schermo
        glfwPollEvents(); //gestisce gli eventi
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
