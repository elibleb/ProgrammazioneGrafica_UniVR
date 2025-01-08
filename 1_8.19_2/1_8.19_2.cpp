#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\shader.h>
#define STB_IMAGE_IMPLEMENTATION //Definisce la macro che permette di includere e utilizzare la libreria stb_image per caricare
//immagini, in questo caso la texture
#include <C:\Users\gambarettop\source\repos\Programmazione_grafica\Programmazione_grafica\stb_image.h>


#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// setto le dimensioni finestra
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
    Shader shader("1_8.19_2_shader.vs", "1_8.19_2_shader.fs");



    //impostare i dati dei vertici (e i buffer) e configurare gli attributi dei vertici 

    float vertices[] = {
        // posizione         // coordinate texture
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // alto destra
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // basso destra
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // basso sinistra
       -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // alto sinistra
    };
    unsigned int indices[] = {
        0, 1, 3, // primo triangolo
        1, 2, 3  // secondo triangolo
    };


    // VBO = vertex buffer object, contiene i dati dei vertici (posizione, colore e texture)
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

    // posizione
    // Il primo parametro di glVertexAttribPointer è l'indice dell'attributo
    // (0 per la posizione, 1 per il colore, 2 per texture). Viene definita la lunghezza (3 per x, y, z),
    // il tipo(GL_FLOAT) e il passo (8 * sizeof(float) perché ogni vertice ha 8 float)
    // Anche colore e texture seguono lo stesso pattern
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // colore
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // coordinate texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture: Questo pezzo si occupa di caricare e creare la texture
    unsigned int texture1, texture2;
    // carica e crea la prima texture
    glGenTextures(1, &texture1); // Crea una texture vuota e la memorizza nell'array texture
    glBindTexture(GL_TEXTURE_2D, texture1); //associa la texture appena creata al target GL_TEXTURE_2D,
    //tutte le successive operazioni GL_TEXTURE_2D da qui in poi avranno effetto su questo oggetto texture
    // imposta i parametri di wrapping della tessitura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Imposta il comportamento della texture quando le coordinate
        //S (orizzontali) eccedono i limiti. GL_REPEAT fa sì che la texture venga ripetuta
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // stesso comportamento per le coordinate T(verticali)
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
    unsigned char* data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {   //se i dati sono stati caricati con successo

        //carica i dati dell'immagine nella texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

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
        glClearColor(1.0f, 0.8f, 0.6f, 1.0f); //imposto il colore dello sfondo
        glClear(GL_COLOR_BUFFER_BIT); // funzione che utilizza lo stato

        // binda le texture sui rispettivi texture units
        //legano le due texture precedentemente caricate (texture1 e texture2) alle texture unit 0 e 1 
        //Queste texture saranno poi utilizzate nei shader
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);


        // prima trasformazione (scale/rotate/transform)
        glm::mat4 transform = glm::mat4(1.0f); //creazione matrice 4x4 di tipo glm::mat4 inizializzata come matrice identità, 
        //non modifica un oggetto quando viene applicata a esso
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f)); //applicata una traslazione alla matrice trans, trasformazione
        //glm::vec3(0.5f, -0.5f, 0.0f) vettore di traslazione. 0.5f unità lungo l'asse X, - 0.5f unità lungo l'asse Y. Z invariato
        //oggetto spostato in basso a destra
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f)); //rotazione della matrice trans attorno asse Z
        /*glfwGetTime() tempo trascorso (in secondi) dall'inizio del programma, il rettangolo ruoterà di un angolo crescente nel tempo
        glm::vec3(0.0, 0.0, 1.0) specifica l'asse di rotazione Z*/
        unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform"); //recupera locazione dell'uniform transform 
        //nel programma shader
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // disegna il rettangolo
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //seconda trasformazione (scale/rotate/transform)
        transform = glm::mat4(1.0f); //creazione matrice 4x4 di tipo glm::mat4 inizializzata come matrice identità, 
        //non modifica un oggetto quando viene applicata a esso
        transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f)); //applicata una traslazione alla matrice trans, trasformazione
        //glm::vec3(-0.5f, 0.5f, 0.0f) vettore di traslazione. 0.5f unità lungo l'asse X, - 0.5f unità lungo l'asse Y. Z invariato
        //oggetto spostato in alto a sinistra
        float scaleAmount = sin(glfwGetTime()); //calcola il seno del tempo trascorso, sin restituisce un valore che oscilla tra -1 e +1
        transform = glm::scale(transform, glm::vec3(scaleAmount, scaleAmount, scaleAmount)); //applica una scalatura ciclica dell'oggetto
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]); //invia la matrice di trasformazione aggiornata al programma shader
        
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

