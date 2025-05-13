import google.genai as genai
import socket
import struct
import argparse
import google.genai.types as types

#EXIT_MESSAGE Mensaje para cerrar el chat
#PORT Puerto de conexión con el LLM
#HOST_IP Ip de conexión con el LLM (por defecto loopback)
#HISTORY_MAX_MEMORY Numero máximo de interacciones, una interacción es una prompt del usuario y una respuesta del LLM
#PERFORMANCE_MODE  Este parámetro sería para elegir si queremos que se ejecute un modelo pequeño (y en principio más rápido) o uno más grande con mejor calidad de respuesta
#                   Quality: elige el modelo más grande
#                   Fast: elige el modelo más pequeño
#                   Balanced: elige la mediana de los modelos, este es el modo por defecto si el parámetro que se pasa no coincide con nunguno de los valores posibles
#EXECUTION_MODE La idea de este parámetro es que se pueda alternar entre modos más verbosos o no
#                   Debug: permite escribir por consola mensajes informativos que persistan durante la sesión
#                   Release: no muestra mensajes, es la opción por defecto si no se pone ninguna de las dos
#TEMPERATURE Permite ajustar la temperatura del modelo, a menos temperatura el modelo es más coherente y sobre todo, durante la respuesta siempre cogerá la palabra con más probabilidad
#                   Una temperatura de 0.0 siempre dará las mismas respuestas
#                   A mayor sea la temperatura más "aleatoria" será la respuesta del LLM, y será más creativo
#                   El valor mínimo de temperatura es 0, y el parámetro que recibe el script se divide entre 100 para obtener valores decimales

DEFAULT_PORT = 8080
DEFAULT_HIST_MAX_MEM = 0
DEFAULT_TEMP = 0.2

parser = argparse.ArgumentParser()
parser.add_argument('--exit_msg', action='store', dest='exit_msg', default='quit_llm')
parser.add_argument('--port', action='store', dest='port')
parser.add_argument('--host_ip', action='store', dest='host_ip', default='127.0.0.1')
parser.add_argument('--hist_max_mem', action='store', dest='hist_max_mem')
parser.add_argument('--perf_mode', action='store', dest='perf_mode', default='Fast')
parser.add_argument('--exe_mode', action='store', dest='exe_mode', default='Debug')
parser.add_argument('--temperature', action='store', dest='temp')
args = parser.parse_args()

GOOGLE_API_KEY= ["AIzaSyDxdOi9O9-vJycGB8PaY_trfsUKdXx4hm0"]#, "AIzaSyA3G115LsekRUJolFI30WbhMhVeyOu1MEQ"]

EXIT_MESSAGE = args.exit_msg
PORT = DEFAULT_PORT
if((args.port is not None) and args.port.isdigit()):
    PORT = int(args.port)
    #Comprobar que está entre los valores de puertos posibles para IPV4, que el puerto esté disponible dentro de estos rangos es cosa del programador
    if(PORT < 0 and PORT >= 65535):
        PORT = DEFAULT_PORT
HOST_IP = args.host_ip
HISTORY_MAX_MEMORY = DEFAULT_HIST_MAX_MEM
if((args.hist_max_mem is not None) and  args.hist_max_mem.isdigit()):
    HISTORY_MAX_MEMORY = int(args.hist_max_mem)
    if(HISTORY_MAX_MEMORY < 0):
        HISTORY_MAX_MEMORY = 0
PERFORMANCE_MODE = args.perf_mode
EXECUTION_MODE = args.exe_mode
TEMPERATURE = DEFAULT_TEMP
if((args.temp is not None) and  args.temp.isdigit()):
    TEMPERATURE = int(args.temp)/100
    if(TEMPERATURE < 0):
        TEMPERATURE = 0
        

print("Exit MSG ", EXIT_MESSAGE, " PORT ", PORT, " IP ", HOST_IP, " HISTORY ", HISTORY_MAX_MEMORY, " PERFORMANCE ", PERFORMANCE_MODE, " EXECUTION ", EXECUTION_MODE, " TEMPERATURE ", TEMPERATURE)

#Wrapper del print para que solo se llame al print en caso de estar en debug
#Usad este método en vez del print de python
def Print(*args):
    if(EXECUTION_MODE == "Debug"):
        print( "\n\t"+" ".join(map(str,args)))

if(EXECUTION_MODE != "Release" and EXECUTION_MODE != "Debug"): 
    print("Execution mode ", EXECUTION_MODE, ' not found, using Release mode')
    EXECUTION_MODE = "Release"

def OpenPort():
    api_index = 0

    # Configurar historial para memoria
    history = []
    if(HISTORY_MAX_MEMORY <= 0): #Añadimos un elemento al historial donde se va a guardar la prompt del usuario en caso de no haber memoria
         history.append({})
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
                    if(HISTORY_MAX_MEMORY > 0):
                        history.append({'role': 'user', 'content': userMessage})
                    else:
                        history[0]={'role': 'user', 'content': userMessage}

                    
                    new_api_key = GOOGLE_API_KEY[api_index]

                    client = genai.Client(
                        api_key=new_api_key
                    )                    
                    
                    response = client.models.generate_content(
                        model='gemini-2.0-flash',
                        contents=userMessage,
                        config= types.GenerateContentConfig(
                            response_mime_type= 'application/json',
                            temperature= TEMPERATURE
                        )
                    )

                    api_index = (api_index + 1) % len(GOOGLE_API_KEY)

                    Print(response.text)
                    # Descomentar la sección para usar el modo "stream" a true
                    #for word in stream:
                    #    Print(word.message.content, end='', flush=True)
                    #    response += word.message.content

                    response = bytes(response.text, 'utf-8')
                    pack = struct.pack("I%ds" % (len(response),), len(response), response)

                    conn.sendall(pack)
                    if(HISTORY_MAX_MEMORY > 0):
                        history.append({'role': 'assistant', 'content': response})
                        history = history[-HISTORY_MAX_MEMORY*2:] #limitar el número de interacciones registradas en el historial
                        #Se multiplica por 2 porque cada interacción del historial ocupa 2 posiciones de array: la prompt y la respuesta
                except Exception as e:
                    Print('\033[31mError:', e.error, '\033[0m')
                    userMessage = EXIT_MESSAGE


OpenPort()