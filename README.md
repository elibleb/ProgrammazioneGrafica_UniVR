# ProgrammazioneGrafica_UniVR
## 1. Getting Started
### 4. Hello Window
#### 4.4 Crea una finestra
[1_4.4.cpp](1_4.4.cpp)
<br><br>
<img src="MEDIA/1_4.4.png" width="400" heigth="200">
#### 4.6 Crea una finestra con uno sfondo colorato
[1_4.6.cpp](1_4.6.cpp)
<br><br>
<img src="MEDIA/1_4.6.png" width="400" heigth="200">
### 5. Hello Triangle
#### 5.5.2 Utilizza gli shader per disegnare un triangolo nella finestra
[1_5.5_2.cpp](1_5.5_2.cpp)
<br><br>
<img src="MEDIA/1_5.5_2.png" width="400" heigth="200">
#### 5.6 Creare un rettangolo composto da due triangoli (visuale wireframe)
[1_5.6.cpp](1_5.6.cpp)
<br><br>
<img src="MEDIA/1_5.6.png" width="400" heigth="200">
#### 5.8_1 Disegna due triangoli vicini utilizzando ‘glDrawArrays’ aggiungendo più vertici
[1_5.8_1.cpp](1_5.8_1.cpp)
<br><br>
<img src="MEDIA/1_5.8_1.png" width="400" heigth="200">
#### 5.8_2 Disegna due triangoli vicini utilizzando due VBO e VAO diversi
[1_5.8_2.cpp](1_5.8_2.cpp)
<br><br>
<img src="MEDIA/1_5.8_2.png" width="400" heigth="200">
#### 5.8_3 Disegna due triangoli, uno di colore arancione e uno di colore giallo, usando due programmi shader che utilizzano due diversi fragment shaders
[1_5.8_3.cpp](1_5.8_3.cpp)
<br><br>
<img src="MEDIA/1_5.8_3.png" width="400" heigth="200">
### 6. Shaders
#### 6.3 Invio di valori dal vertex shader al fragment shader
[1_6.3.cpp](1_6.3.cpp)
<br><br>
<img src="MEDIA/1_6.3.png" width="400" heigth="200">
#### 6.4 Utilizzo degli uniforms per impostare attributi in grado di cambiare nel tempo e per scambiare dati tra l'applicazione e gli shaders
[1_6.4.cpp](1_6.4.cpp)
<br><br>
<img src="MEDIA/1_6.4.gif" width="400" heigth="200">
#### 6.5 Specifica tre colori come attributi ai vertici del triangolo e sfrutta l'interpolazione tra frammenti del fragment shader
[1_6.5.cpp](1_6.5.cpp)
<br><br>
<img src="MEDIA/1_6.5.png" width="400" heigth="200">
#### 6.6 Uso della classe Shader e divisione in due file, uno di vertex shader e uno di fragment shader
[1_6.6.cpp](1_6.6/1_6.6.cpp)
<br><br>
<img src="MEDIA/1_6.6.png" width="400" heigth="200">
#### 6.8_1 Modifica del vertex shader per rovesciare il triangolo
[1_6.8_1.cpp](1_6.8_1/1_6.8_1.cpp)
<br><br>
<img src="MEDIA/1_6.8_1.png" width="400" heigth="200">
#### 6.8_2 Uso di un offset passato come parametro uniform per spostare il triangolo a destra (dal vertex shader)
[1_6.8_2.cpp](1_6.8_2/1_6.8_2.cpp)
<br><br>
<img src="MEDIA/1_6.8_2.png" width="400" heigth="200">
#### 6.8_3 Utilizzo della posizione del vertice come parametro RGB per il colore
[1_6.8_3.cpp](1_6.8_3/1_6.8_3.cpp)
<br><br>
<img src="MEDIA/1_6.8_3.png" width="400" heigth="200">
### 7. Textures
#### 7.0 Aggiunta della texture wall.jpg al triangolo
[1_7.0.cpp](1_7.0/1_7.0.cpp)
<br><br>
<img src="MEDIA/1_7.0.png" width="400" heigth="200">
##### 7.6 Aggiunta della texture container.jpg al rettangolo
[1_7.6.cpp](1_7.6/1_7.6.cpp)
<br><br>
<img src="MEDIA/1_7.6.png" width="400" heigth="200">
##### 7.6_1 Aggiunta della texture container.jpg al rettangolo ed interpola il colore con la posizione dei vertici 
[1_7.6_1.cpp](1_7.6_1/1_7.6_1.cpp)
<br><br>
<img src="MEDIA/1_7.6_1.png" width="400" heigth="200">
#### 7.7 Mix di due texture con valore 50%/50%
[1_7.7.cpp](1_7.7/1_7.7.cpp)
<br><br>
<img src="MEDIA/1_7.7.png" width="400" heigth="200">
#### 7.8_1 Inverte la texture awesomeface.png con mix di valore 80%/20%
[1_7.8_1.cpp](1_7.8_1/1_7.8_1.cpp)
<br><br>
<img src="MEDIA/1_7.8_1.png" width="400" heigth="200">
#### 7.8_2 Replica la texture awesomeface.png
[1_7.8_2.cpp](1_7.8_2/1_7.8_2.cpp)
<br><br>
<img src="MEDIA/1_7.8_2.png" width="400" heigth="200">
#### 7.8_3 Utilizzo della posizione centrale della texture come texture
[1_7.8_3.cpp](1_7.8_3/1_7.8_3.cpp)
<br><br>
<img src="MEDIA/1_7.8_3.png" width="400" heigth="200">
#### 7.8_4 Aumenta/riduce la percentuale di mix delle due texture utilizzando le frecce da tastiera, UP per visualizzare awesomeface e DOWN per visualizzare container
[1_7.8_4.cpp](1_7.8_4/1_7.8_4.cpp)
<br><br>
<img src="MEDIA/1_7.8_4.gif" width="400" heigth="200">
### 8. Transformations
#### 8.17 Scala e ruota il rettangolo
[1_8.17.cpp](1_8.17/1_8.17.cpp)
<br><br>
<img src="MEDIA/1_8.17.png" width="400" heigth="200">
#### 8.17_2 Traslare il rettangolo in basso a destra e ruotarlo nel tempo
[1_8.17_2.cpp](1_8.17_2/1_8.17_2.cpp)
<br><br>
<img src="MEDIA/1_8.17_2.gif" width="400" heigth="200">
#### 8.19_1 Ruota il rettangolo nel tempo e traslalo in basso a destra
[1_8.19_1.cpp](1_8.19_1/1_8.19_1.cpp)
<br><br>
<img src="MEDIA/1_8.19_1.gif" width="400" heigth="200">
#### 8.19_2 Aggiungi a "8.17_2" un rettangolo in alto a sinistra e scalalo nel tempo
[1_8.19_2.cpp](1_8.19_2/1_8.19_2.cpp)
<br><br>
<img src="MEDIA/1_8.19_2.gif" width="400" heigth="200">
### 9. Coordinate Systems
#### 9.7 Sposta le coordinate dei vertici attraverso le matrici model, view e projection. Si ottenerrà un oggetto inclinato all'indietro, un po' distante dall'osservatore e visualizzato con prospettiva
[1_9.7.cpp](1_9.7/1_9.7.cpp)
<br><br>
<img src="MEDIA/1_9.7.png" width="400" heigth="200">
#### 9.8 Disegna un cubo che ruota senza abilitare le informazioni di profondità
[1_9.8.cpp](1_9.8/1_9.8.cpp)
<br><br>
<img src="MEDIA/1_9.8.gif" width="400" heigth="200">
#### 9.8_1 Aggiungi a "9.8" le informazioni di profondità abilitate
[1_9.8_1.cpp](1_9.8_1/1_9.8_1.cpp)
<br><br>
<img src="MEDIA/1_9.8_1.gif" width="400" heigth="200">
#### 9.8_2 Disegna più cubi in posizioni casuali con rotazione casuale
[1_9.8_2.cpp](1_9.8_2/1_9.8_2.cpp)
<br><br>
<img src="MEDIA/1_9.8_2.png" width="400" heigth="200">
#### 9.9_3 Modifica "9.8.2" in modo da far ruotare un cubo ogni tre
[1_9.9_3.cpp](1_9.9_3/1_9.9_3.cpp)
<br><br>
<img src="MEDIA/1_9.9_3.gif" width="400" heigth="200">
### 10. Camera
#### 10.2 Rotazione nel tempo della camera attorno alla scena
[1_10.2.cpp](1_10.2/1_10.2.cpp)
<br><br>
<img src="MEDIA/1_10.2.gif" width="400" heigth="200">
#### 10.4 Crea una camera che si muove alla stessa velocità su ogni sistema (deltaTime), movimento con WASD sulla tastiera
[1_10.4.cpp](1_10.4/1_10.4.cpp)
<br><br>
<img src="MEDIA/1_10.4.gif" width="400" heigth="200">
#### 10.8 Crea una camera che consente di muoversi liberamente in un ambiente 3D (movimento con WASD tastiera, scroll e posizione mouse)
[1_10.8.cpp](1_10.8/1_10.8.cpp)
<br><br>
<img src="MEDIA/1_10.8.gif" width="400" heigth="200">
#### 10.9 Crea una camera che consente di muoversi liberamente in un ambiente 3D (movimento con WASD tastiera, scroll e posizione mouse) con una classe camera di tipo fly (camera.h)
[1_10.9.cpp](1_10.9/1_10.9.cpp)
<br><br>
<img src="MEDIA/1_10.9.gif" width="400" heigth="200">
#### 10.9_1 Come 10.9 ma con camera di tipo FPS (mantiene l'osservatore a livello ground), usa camera_ground.h con ground level (xz plane)
[1_10.9_1.cpp](1_10.9_1/1_10.9_1.cpp)
<br><br>
<img src="MEDIA/1_10.9_1.gif" width="400" heigth="200">
#### 10.9_2 Come negli ultimi esercizi ma viene creata la propria funzione calculate_lookAt_matrix (sempre movimento con WASD tastiera, scroll e posizione mouse)
[1_10.9_2.cpp](1_10.9_2/1_10.9_2.cpp)
<br><br>
<img src="MEDIA/1_10.9.gif" width="400" heigth="200">
## 2. Lighting
### 12. Colors
#### 12.1 Crea un oggetto cubo ed un oggetto luce (cubo più piccolo di quello principale)
[2_12.1.cpp](2_12.1/2_12.1.cpp)
<br><br>
<img src="MEDIA/2_12.1.png" width="400" heigth="200">
### 13. Basic Lighting
#### 13.4 Uso dell'illuminazione diffusa. Intensità di illuminazione del cubo è maggiore se l'angolo tra il vettore normale e il vettore direzione della luce è piccolo
[2_13.4.cpp](2_13.4/2_13.4.cpp)
<br><br>
<img src="MEDIA/2_13.4.png" width="400" heigth="200">
#### 13.6 Completa l'implementazione del sistema di illuminazione Phong nello spazio world, (usa camera_13.h per mia preferenza sensibilità mouse)
[2_13.6.cpp](2_13.6/2_13.6.cpp)
<br><br>
<img src="MEDIA/2_13.6.png" width="400" heigth="200">
#### 13.7_1 Usa seno e coseno per muovere la posizione della luce nel tempo
[2_13.7_1.cpp](2_13.7_1/2_13.7_1.cpp)
<br><br>
<img src="MEDIA/2_13.7_1.gif" width="400" heigth="200">
#### 13.7_2 Implementa il Pong shading nello spazio view invece che world (camera_13.h per sensibilità mouse)
[2_13.7_2.cpp](2_13.7_2/2_13.7_2.cpp)
<br><br>
<img src="MEDIA/2_13.7_2.gif" width="400" heigth="200">
#### 13.7_3 Utilizzo del Gouraud shading al posto del Phong (risulta meno preciso)
[2_13.7_3.cpp](2_13.7_3/2_13.7_3.cpp)
<br><br>
<img src="MEDIA/2_13.7_3.png" width="400" heigth="200">
### 14. Materials
#### 14.3 Effetto come quello raggiunto in precedenza ma con controllo completo sulla luce e sul materiale dell'oggetto
[2_14.3.cpp](2_14.3/2_14.3.cpp)
<br><br>
<img src="MEDIA/2_14.3.gif" width="400" heigth="200">
#### 14.4_1 Simula il materiale plastica ciano con intensità luminosa impostata al massimo
[2_14.4_1.cpp](2_14.4_1/2_14.4_1.cpp)
<br><br>
<img src="MEDIA/2_14.4_1.png" width="400" heigth="200">
### 15. Lighting Maps
#### 15.1 Uso di una diffuse map
[2_15.1.cpp](2_15.1/2_15.1.cpp)
<br><br>
<img src="MEDIA/2_15.1.png" width="400" heigth="200">
#### 15.3 Calcola la luce speculare dalla texture fornita
[2_15.3.cpp](2_15.3/2_15.3.cpp)
<br><br>
<img src="MEDIA/2_15.3.png" width="400" heigth="200">
#### 15.4_1 Inverte la luce speculare calcolata dalla texture
[2_15.4_1.cpp](2_15.4_1/2_15.4_1.cpp)
<br><br>
<img src="MEDIA/2_15.4_1.png" width="400" heigth="200">
#### 15.4_2 Utilizza una texture colorata per calcolare la luce speculare
[2_15.4_2.cpp](2_15.4_2/2_15.4_2.cpp)
<br><br>
<img src="MEDIA/2_15.4_2.png" width="400" heigth="200">
#### 15.4_3 Aggiunta di una luce emissiva data da una texture (rappresentata dalle scritte verdi)
[2_15.4_3.cpp](2_15.4_3/2_15.4_3.cpp)
<br><br>
<img src="MEDIA/2_15.4_3.png" width="400" heigth="200">
### 16. Light Casters
#### 16.1 Creazione di una luce direzionale
[2_16.1.cpp](2_16.1/2_16.1.cpp)
<br><br>
<img src="MEDIA/2_16.1.png" width="400" heigth="200">
#### 16.3_2  Creazione di una luce point con attenuazione
[2_16.3_2.cpp](2_16.3_2/2_16.3_2.cpp)
<br><br>
<img src="MEDIA/2_16.3_2.png" width="400" heigth="200">
#### 16.5  Creazione di una flashlight posizionata sulla camera
[2_16.5.cpp](2_16.5/2_16.5.cpp)
<br><br>
<img src="MEDIA/2_16.5.png" width="400" heigth="200">
#### 16.6  Creazione di una flashlight con attenuazione dei bordi
[2_16.6.cpp](2_16.6/2_16.6.cpp)
<br><br>
<img src="MEDIA/2_16.6.png" width="400" heigth="200">
### 17. Multiple lights
#### 17.3  Crea una scena con luci multiple (ambientale, direzionale, spotlight e pointlight)
[2_17.3.cpp](2_17.3/2_17.3.cpp)
<br><br>
<img src="MEDIA/2_17.3.png" width="400" heigth="200">
## 3. Model Loading
### 21. Model
#### 21.3 Importa un modello (formato .obj) completo di texture attraverso Assimp, uso di classi Mesh e Model dedicate
[3_21.3.cpp](3_21.3/3_21.3.cpp)
<br><br>
<img src="MEDIA/3_21.3.png" width="400" heigth="200">
## 5. Advanced Lighting
### 37. Normal Mapping
#### 37.2_2 Utilizza normal mapping per creare dettagli a basso costo attraverso la modifica di vettori delle normali per frammento senza dover modificare l'equazione dell'illuminazione
[5_37.2_2.cpp](5_37.2_2/5_37.2_2.cpp)
<br><br>
<img src="MEDIA/5_37.2_2.png" width="400" heigth="200">
## 6. Esercitazione: caricamento modello animato con Assimp
Riproduce l'animazione scheletale del repository [skeletal_animation_tutorial](https://github.com/dinismf/skeletal_animation_tutorial)
<br><br>
<img src="MEDIA/skeletal_animation.gif" width="400" heigth="200">

## 7. Esercitazione aggiuntiva 1
[esercitazione1.cpp](esercitazione1/esercitazione1.cpp)
<br><br>
In questa esercitazione viene istanziato il modello statico "backpack.obj” che è già stato utilizzato nel capitolo dedicato al caricamento dei modelli (vedere Model Loading, es. 3_21.3). A questo modello statico viene applicata l'illuminazione che è composta da:
- 1 luce direzionale
- 4 point light di cui una ruota attorno al modello
- 1 spot light posizionata sulla camera

Le point light sono implementate come cubi.<br>
L'illuminazione viene gestita attraverso la definizione delle strutture dati necessarie che vengono poi inviate al relativo shader.<br>
Il calcolo dell'illuminazione (lighting) avviene nel tangent space.<br>
Vengono implementate anche le classiche funzioni di gestione dell'input utente utilizzate negli esercizi precedenti, nello specifico, il movimento della camera tramite i tasti WASD, il controllo della direzione attraverso il movimento del mouse e lo zoom tramite lo scroll dello stesso (rotellina).
<br><br>
<img src="MEDIA/esercitazione1.gif" width="400" heigth="200">
## 8. Esercitazione aggiuntiva 2
[esercitazione2.cpp](esercitazione2/esercitazione2.cpp) <br><br>
In questa esercitazione viene implementata una tecnica chiamata "Point Shadows". Si tratta di una tecnica utilizzata per creare ombre dinamiche che si estendono in tutte le direzioni come accade nel mondo reale, dove una sorgente di luce pointlight (ossia che proviene da un punto) produce ombre che si propagono ovunque nell'ambiente circostante. Questa tecnica è molto simile alla mappatura direzionale delle ombre (directional light), in particolare si crea una mappa di profondità dalla prospettiva della luce, la quale viene campionata in base alla posizione attuale del frammento. Successivamente il frammento viene confrontato con il valore di profondità per determinare se è in ombra oppure no. A differenza della mappatura direzionale, qui si necessita di una mappa di profondità chiamata Depth Map che richiede il rendering della scena da tutte le direzioni circostanti di un punto di luce. Per fare questo si utilizza una cubemap che consente di eseguire il rendering dell'intera scena su ciasuna delle sei facce della cubemap stessa. Una volta generata la cubemap di profondità, essa viene passata al fragment shader di illuminazione che campiona la cubemap usando un vettore di direzione per ottenere la profondità più vicina rispetto alla prospettiva della luce per ogni punto. Sebbene la cubemap possa risultare costosa, visto che richiede molte chiamate di rendering per costruirla, in questo esercizio si utilizza lo shader geometrico che consente di creare la cubemap di profondità con un solo passaggio di rendering.
Innanzitutto, bisogna creare una cubemap e associare a ciascuna delle sei facce un'immagine texture 2D contenente i valori di profondità. Di solito, si renderizza una faccia della cubemap alla volta collegando la texture al framebuffer e renderizzando la scena sei volte. Ma visto che si può sfruttare uno shader geometrico, si può eseguire il rendering di tutte le facce in un unico passaggio. Così facendo colleghiamo direttamente la cubemap come allegato di profondità al framebuffer tramite "glFramebufferTexture". Il rendering si svolge in due fasi: prima si genera la cubemap di profondità e poi si usa la mappa di profondità nel classico passaggio di rendering per aggiungere le ombre alla scena. Questo procedimento è simile alla mappatura delle ombre direzionali, ma in questa circostanza si utilizza una cubemap di profondità invece di una texture di profondità 2D.
Una volta configurato il framebuffer e la cubemap, bisogna trasformare tutta la geometria della scena nello spazio della luce in tutte e sei le direzioni della stessa. Occorre così una matrice di trasformazione dello spazio luce per ciascuna faccia della cubemap. Ciascuna matrice di trasformazione include sia una matrice di proiezione che una matrice di vista. Per la matrice di proiezione si utilizza una proiezione prospettica che è la stessa per tutte le facce della cubemap, il parametro del campo visivo di "glm::perspective" viene impostato a 90 gradi. Questo valore garantisce che il campo visivo copra esattamente una singola faccia della cubemap, allineando correttamente le facce tra loro sui bordi. Siccome la matrice di proiezione non cambia per ogni direzione, può essere riutilizzata per tutte e sei le matrici di trasformazione. Per quanto riguarda la matrice di vista, bisogna crearne una diversa per ogni direzione. Usando "glm::lookAt" si possono definire sei direzioni di vista, ciascuna orientata verso una faccia della cubemap. Così si ottengono sei matrici di vista che, moltiplicate con la matrice di proiezione, producono sei diverse matrici di trasformazione dello spazio luce. Queste matrici vengono poi passate agli shader che si occupano di renderizzare la profondità nella cubemap.
Per il rendering dei valori di profondità in una cubemap sono necessari tre shader, uno per i vertici (vertex shader), uno per la geometria (geometry shader) e uno per i frammenti (fragment shader) . Il geometry shader ha il compito di trasformare tutti i vertici dallo spazio del mondo (world space) verso i sei diversi spazi di luce. Di conseguenza lo shader dei vertici si occupa solo di trasformare i vertici nello spazio del mondo e li invia al geometry shader. Il geometry shader riceve in input tre vertici di un triangolo e una matrice uniforme che contiene le matrici di trasformazione dello spazio luce. È quindi responsabile della trasformazione di questi vertici negli spazi di luce corrispondenti. <br><br>
<img src="MEDIA/esercitazione2.gif" width="400" heigth="200">
<br><br>
[esercitazione2_soft.cpp](esercitazione2/esercitazione2_soft.cpp)
<br><br>
Dopo aver applicato la tecnica si nota che le ombre risultano troppo nette, quindi non molto realistiche. Si è trovata una soluzione implementando il "Percentage-Close Filtering" (PCF), una tecnica che rende le ombre più morbide. Questo si ottiene calcolando il valore dell'ombra come la media tra quella attuale e quelle adiacenti. In questo modo, le ombre risultano più simili alla realtà.
<br><br>
<img src="MEDIA/esercitazione_2soft.gif" width="400" heigth="200">



