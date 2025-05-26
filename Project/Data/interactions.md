# Elementos y sus interacciones

## Objetos

En la experiencia existen objetos con los que el usuario puede interactuar de diversas maneras: cogerlos, soltarlos, usarlos, etc. A continuación se podrá ver el listado de interacciones y sus descripciones para cada una de las acciones que puede realizar el jugador con dichos objetos.

| Nombre | Identificador del objeto | Descripción |
| -- | -- | -- |
||||
||||
||||

|Nombre| Identificador de la acción | Descripción |
| -- | -- | -- |
|Empezar a observar|start_look_at|El usuario ha empezado a mirar un objeto.|
|Dejar de observar|stop_look_at|El usuario ha dejado de mirar un objeto.|
|Interactuar con|interact_with|El usuario ha interactuado con un objeto.|

Existen objetos que poseen comportamientos más complejos y por ello pueden realizar un mayor número de acciones. Estos elementos se pueden subdividir en tres grandes grupos: recogibles, consumibles y usables.


### Objetos recogibles

Los **objetos recogibles** son aquellos que el usuario puede recoger el mundo y llevar consigo. Estos objetos también tienen la posibilidad de ser inspeccionados por el usuario con más detenimiento. Estos objetos no tienen un uso en específico, simplemente son objetos que se pueden llevar encima. Además, es importante destacar que no desaparecen del mundo en ningún momento.

| Nombre | Identificador del objeto | Descripción |
| -- | -- | -- |
||||
||||
||||

|Nombre| Identificador del objeto | Descripción |
| -- | -- | -- |
|Coger|pick_up|El usuario ha cogido un objeto. Este objeto se le ha posicionado en la mano al usuario y lo llevará con él. No podrá coger otro objeto hasta que haya soltado el que lleva consigo.|
|Soltar|drop|El usuario ha soltado un objeto. El objeto se posicionará de nuevo en el mundo.|
|Empezar a inspeccionar|start_inspect|El usuario ha empezado a inspeccionar un objeto. El usuario coloca el objeto en el centro de la pantalla donde puede girarlo o hacer zoom para inspeccionarlo con más detenimiento.|
|Dejar de inspeccionar|stop_inspect|El usuario ha dejado de inspeccionar el objeto. Este se le ha vuelto a posicionar en la mano.|


#### Objetos usables

Los **objetos usables** tienen una función en el mundo, como puede ser una llave para abrir una puerta, un cono para parar el tráfico, o una pieza de un puzzle, entre otros. Estos objetos parten de las interacciones que acabamos de ver y añaden funcionalidades específicas.

| Nombre | Identificador del objeto | Descripción |
| -- | -- | -- |
||||
||||
||||

|Nombre| Identificador del objeto | Descripción |
| -- | -- | -- |
|Usar correctamente|use_correct|El usuario ha usado correctamente el objeto. Es decir, ha realizado la acción que se esperaba con dicho elemento. Por ejemplo, ha usado la llave en la puerta correcta.|
|Usar incorrectamente|use_incorrect|El usuario ha usado incorrectamente el objeto. Es decir, ha realizado una acción innesperada con el objeto. Por ejemplo, ha colocado la pieza del puzzle en el lugar que no era.|


### Objetos consumibles

A diferencia de los recogibles, los **objetos consumibles** desaparecen del mundo cuando el usuario interactúa con ellos y generan una acción. Por ejemplo, el usuario ha interactuado con una pición de vida, esta poción ha desaparecido y la vida del personaje ha aumentado.

| Nombre | Identificador del objeto | Descripción |
| -- | -- | -- |
||||
||||
||||

|Nombre| Identificador del objeto | Descripción |
| -- | -- | -- |
|Consumir|consume|El usuario ha consumido un objeto.|

### _Extras_

Aquí se pueden ver algunos objetos interactuables que tienen funcionalidades específicas y que no se encuentran definidos por ninguno de las familias descritas anteriormente.

#### Dosímetros

Los **dosímetros** son unos de los objetos más importantes de la experiencia. Se usan para medir la radiación del entorno o de cierto elemento. Hay varios tipos divididos tanto por el tipo de radiación que detectan como por la distancia a la que deben ser usados. Por ejemplo, algunos se usan para medir radiación en el ambiente y otros tienen que ser usados muy cerca del objeto en cuestión para poder detectarla.



| Nombre | Identificador del objeto | Descripción |
| -- | -- | -- |
||||
||||
||||

|Nombre| Identificador del objeto | Descripción |
| -- | -- | -- |
|Consumir|consume|El usuario ha consumido un objeto.|


## NPCs

| Nombre | Identificador del NPC | Descripción |
| -- | -- | -- |
|Empezar a observar|start_look_at|El usuario ha empezado a mirar un NPC.|
|Dejar de observar|stop_look_at|El usuario ha dejado de mirar un NPC.|
|Interactuar con|interact_with|El usuario ha interactuado con un NPC. Este evento se lanza cuando el NPC no tiene acciones o no se puede realizar acciones aún, como la acción de hablar.|


### NPCs "hablables"

| Nombre | Identificador del NPC | Descripción |
| -- | -- | -- |
||||
||||
||||

|Nombre| Identificador de la acción | Descripción |
| -- | -- | -- |
|Hablar|talk_with|El usuario ha hablado con un NPC. Cuando el usuario interactua con el NPC y habla con él para iniciar una conversación. Esta acción solo se realiza cuando está permitida.|
|Avanzar diálogo|continue_dialog|El usuario puede avanzar en el diálogo mientras está hablando con el NPC.|

## Elementos _extras_

|Nombre| Identificador del objeto | Descripción | 
| -- | -- | -- |
||||
||||
||||
