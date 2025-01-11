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
#### 1o.9_1 Come 10.9 ma con camera di tipo FPS (mantiene l'osservatore a livello ground), usa camera_ground.h con ground level (xz plane)
[1_10.9_1.cpp](1_10.9_1/1_10.9_1.cpp)
<br><br>
<img src="MEDIA/1_10.9_1.gif" width="400" heigth="200">
#### 1o.9_2 Come negli ultimi esercizi ma viene creata la propria funzione calculate_lookAt_matrix (sempre movimento con WASD tastiera, scroll e posizione mouse)
[1_10.9_2.cpp](1_10.9_2/1_10.9_2.cpp)
<br><br>
<img src="MEDIA/1_10.9.gif" width="400" heigth="200">
