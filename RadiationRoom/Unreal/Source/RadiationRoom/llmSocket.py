import ollama
import socket
import struct

EXIT_MESSAGE = "quit_app"
PORT= 8080
HOST_IP ='127.0.0.1'
HISTORY_MAX_MEMORY = 5 #Numero máximo de interacciones, una interacción es una prompt del usuario y una respuesta del LLM
PERFORMANCE_MODE = "Fast" #Este parámetro sería para elegir si queremos que se ejecute un modelo pequeño (y en principio más rápido) o uno más grande con mejor calidad de respuesta
#                   Quality: elige el modelo más grande
#                   Fast: elige el modelo más pequeño
#                   Balanced: elige la mediana de los modelos, este es el modo por defecto si el parámetro que se pasa no coincide con nunguno de los valores posibles
EXECUTION_MODE = "Debug" #La idea de este parámetro es que se pueda alternar entre modos más verbosos o no
#                   Debug: permite escribir por consola mensajes informativos que persistan durante la sesión
#                   Release: no muestra mensajes, es la opción por defecto si no se pone ninguna de las dos

# Listar modelos disponibles
modelList = ollama.list()
modelName = ''
# Filtrar modelos que tienen tamaño definido
filtered_models = [model for model in modelList["models"] if "size" in model]
# Ordenar modelos por tamaño de menor a mayor
sorted_models = sorted(filtered_models, key=lambda x: x["size"])
model_names = [model["model"] for model in sorted_models]
#model_names = [model["model"] for model in modelList["models"]]
listLength = len(model_names)

#Wrapper del print para que solo se llame al print en caso de estar en debug
#Usad este método en vez del print de python
def Print(*args):
    if(EXECUTION_MODE == "Debug"):
        print( "\n\t"+" ".join(map(str,args)))

if(EXECUTION_MODE != "Release" and EXECUTION_MODE != "Debug"): 
    print("Execution mode ", EXECUTION_MODE, ' not found, using Release mode')
    EXECUTION_MODE = "Release"

def PickModel():
    # Seleccionar el modelo automáticamente en base a los presets
    if(PERFORMANCE_MODE == "Fast"):
        return model_names[0] 
    elif(PERFORMANCE_MODE == "Quality"):
        return model_names[listLength-1]
    else: 
        index = listLength//2 #Parece ser que usar "//" sirve para quedarse con el cociente
        return model_names[index]

def OpenPort():
    # Configurar historial para memoria
    history = []
    userMessage = ""
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST_IP, PORT))
        s.listen()
        Print("starting to listen")
        conn, addr = s.accept()
        with conn:
            Print('Connected by', addr)
            while userMessage != EXIT_MESSAGE:
                data = conn.recv(4)  # Leer el tamaño del mensaje
                if not data:
                    break
                msg_length = struct.unpack("I", data)[0]  # Extraer la longitud
                userMessage = conn.recv(msg_length)  # Leer el mensaje completo
                userMessage =  userMessage.decode('utf-8')
                Print("The user message is: " + userMessage)
                #Parte del LLM
                try:
                    #El historial es para que el LLM tenga memoria, de momento es la forma que tenemos que funciona
                    history.append({'role': 'user', 'content': userMessage})
                    stream = ollama.chat(
                        model=modelName,
                        messages=history,
                        stream=False,
                        options={"num_gpu": 1, "gpu_layers": -1}
                    )
            
                    response = stream['message']['content']  # Si el modo de chat "stream" está en False, esta es la forma de acceder a la respuesta
                    # Descomentar la sección para usar el modo "stream" a true
                    #for word in stream:
                    #    Print(word.message.content, end='', flush=True)
                    #    response += word.message.content

                    response = bytes(response, 'utf-8')
                    pack = struct.pack("I%ds" % (len(response),), len(response), response)
                    conn.sendall(pack)
                    history.append({'role': 'assistant', 'content': response})
                    history = history[-HISTORY_MAX_MEMORY:] #limitar el número de interacciones registradas en el historial
                except ollama.ResponseError as e:
                    Print('\033[31mError:', e.error, '\033[0m')
                    userMessage = EXIT_MESSAGE


if(listLength <= 0):
        Print('\033[31mError: no ollama models found\033[0m')
else:
    if(listLength > 1):
        modelName = PickModel()
        Print('Based on the performance mode:', PERFORMANCE_MODE, ' the model ',  modelName, ' was chosen')
    else: 
        Print('Only one model was found: ', model_names[0], '\nRunning that model...')
        modelName = model_names[0]
    OpenPort()