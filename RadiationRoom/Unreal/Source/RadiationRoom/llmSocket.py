import ollama
import socket
import struct

EXIT_MESSAGE = "quit_app"
PORT, HOST_IP = 8080, '127.0.0.1'

# Listar modelos disponibles
modelList = ollama.list()
modelName = ''
model_names = [model["model"] for model in modelList["models"]]
listLength = len(model_names)


def PickModel():
    # Seleccionar el modelo
    found = False
    while not found:
        modelName = input("Pick a model to run: ")
        if modelName in model_names:
            found = True
        else:
            print('\033[31mError: the model', modelName, 'could not be found. Available models:\033[0m')
            for model in model_names:
                print('\t', model, '\n')

def OpenPort():
    # Configurar historial para memoria
    history = []
    userMessage = ""
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST_IP, PORT))
        s.listen()
        print("starting to listen")
        conn, addr = s.accept()
        with conn:
            print('Connected by', addr)
            while userMessage != EXIT_MESSAGE:
                data = conn.recv(4)  # Leer el tamaño del mensaje
                if not data:
                    break
                msg_length = struct.unpack("I", data)[0]  # Extraer la longitud
                userMessage = conn.recv(msg_length)  # Leer el mensaje completo
                userMessage =  userMessage.decode('utf-8')
                print("The user message is: " + userMessage)
                #Parte del LLM
                try:
                    #El historial es para que el LLM tenga memoria, de momento es la forma que tenemos que funciona
                    history.append({'role': 'user', 'content': userMessage})
            
                    stream = ollama.chat(
                        model=modelName,
                        messages=history,
                        stream=True,
                        options={"num_gpu": 1, "gpu_layers": -1}
                    )
            
                    response = ""
                    for word in stream:
                        print(word.message.content, end='', flush=True)
                        response += word.message.content

                    response = bytes(response, 'utf-8')
                    pack = struct.pack("I%ds" % (len(response),), len(response), response)
                    conn.sendall(pack)

                    history.append({'role': 'assistant', 'content': response})
                except ollama.ResponseError as e:
                    print('\033[31mError:', e.error, '\033[0m')
                    userMessage = EXIT_MESSAGE


if(listLength <= 0):
        print('\033[31mError: no ollama models found\033[0m')
else:
    if(listLength > 1):
        PickModel()
    else: 
        print('Only one model was found: ', model_names[0], '\nRunning that model...')
        modelName = model_names[0]
    OpenPort()