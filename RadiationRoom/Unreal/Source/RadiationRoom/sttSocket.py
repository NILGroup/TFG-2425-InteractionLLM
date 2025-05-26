import socket
import struct
import os
import numpy as np
import speech_recognition as sr
import whisper
import torch
from datetime import datetime, timedelta, timezone
from queue import Queue
import asyncio
import pyaudio

DEFAULT_PORT = 7070

PORT = DEFAULT_PORT
HOST_IP = "127.0.0.1"


async def OpenPort():
    global record_flag
    record_flag = False

    global exit_flag
    exit_flag = False

    # Cola segura para pasar datos desde la grabación asíncrona
    global data_queue
    data_queue = Queue()

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    s.bind((HOST_IP, PORT))
    s.listen()
    s.setblocking(False)

    print("starting to listen")
    print('', end='', flush=True)
    
    loop = asyncio.get_event_loop()

    conn, addr = await loop.sock_accept(s)
    conn.setblocking(False)

    async def handle_recording(conn):
        global exit_flag
        global record_flag
        global data_queue

        print('Connected by', addr)
        print('', end='', flush=True)
        
        transcription = ['']
        phrase_time = datetime.now(timezone.utc)

        # Carga del modelo
        audio_model = whisper.load_model("base.en")
        phrase_timeout = 3
        record_timeout = 3

        # Se utiliza SpeechRecognizer para grabar el audio porque incluye funcionalidad para detectar cuando termina el diálogo
        recorder = sr.Recognizer()
        recorder.energy_threshold = 1000
        recorder.dynamic_energy_threshold = False

        loop = asyncio.get_running_loop()

        pa = pyaudio.PyAudio()

        num_microphones = 0
        for i in range (pa.get_device_count()):
            device_info = pa.get_device_info_by_index(i)
            if device_info['maxInputChannels'] != 0 and device_info['hostApi'] == 0:
                num_microphones += 1

        if num_microphones < 1:
            print("ERROR: No microphones were found. Closing socket connection.")
            
            message = bytes("_socket_down_", "utf-8")
            pack = struct.pack("I%ds" % (len(message),), len(message), message)
            
            await loop.sock_sendall(conn, pack)

            conn.close()
            s.close()

            return None
        
        pa.terminate()

        source = sr.Microphone(sample_rate=16000)
        if source.device_index == None:
            print("Using default microphone designated by OS")

        with source:
            recorder.adjust_for_ambient_noise(source)

        def record_callback(_, audio:sr.AudioData) -> None:
            global record_flag
            """
            Threaded callback function to receive audio data when recordings finish.
            audio: An AudioData containing the recorded bytes.
            """
            data = audio.get_raw_data()
            data_queue.put(data)

        # Se crea un hilo que permite pasar los bytes del audio
        global background_recording_task
        background_recording_task = None
        
        # Informa al usuario de que el modelo está listo para escuchar
        print("Model loaded.\n")

        local_record = False

        while not exit_flag:
            now = datetime.now(timezone.utc)

            if record_flag and not local_record:
                local_record = True
                background_recording_task = recorder.listen_in_background(source, record_callback, record_timeout)
            
            elif not record_flag and local_record:
                local_record = False
                background_recording_task(True)

            if not data_queue.empty(): 
                phrase_time = now
                
                response = bytes()
                
                audio_data = b''.join(data_queue.queue)
                data_queue.queue.clear()
                
                audio_np = np.frombuffer(audio_data, dtype=np.int16).astype(np.float32) / 32768.0

                result = audio_model.transcribe(audio_np, fp16=torch.cuda.is_available())
                text = result['text'].strip()

                transcription.append(text)

                os.system('cls' if os.name=='nt' else 'clear')
                for line in transcription:
                    if line != '':
                        print(line)
                        response = response + bytes(line, 'utf-8')

                print('', end='', flush=True)

                pack = struct.pack("I%ds" % (len(response),), len(response), response)

                await loop.sock_sendall(conn, pack)

            elif now - phrase_time > timedelta(seconds=record_timeout*1.3):
                message_has_content = False
                for line in transcription:
                    if line != '': 
                        message_has_content = True
                        print("Message has content", transcription)
                        break
                
                if message_has_content:
                    message = bytes("_send_message_", "utf-8")
                    pack = struct.pack("I%ds" % (len(message),), len(message), message)
                    await loop.sock_sendall(conn, pack)
                    os.system('cls' if os.name=='nt' else 'clear')
                    print("MESSAGE SENT", transcription)
                
                transcription = []
                phrase_time = now
            
            await asyncio.sleep(0)
        
    async def handle_input_client(client):
        global record_flag
        global exit_flag
        global data_queue
        global background_recording_task

        loop = asyncio.get_running_loop()
        data = None
        while data != 'EXIT':
            try:
                data = await loop.sock_recv(client, 5)
                if not data:
                    print("No data received, closing connection")
                    break  # Sale del bucle si no se recibe información

                data = data.decode('utf8')
                print(f"Received data: {data}")
                if data == "STOP":
                    record_flag = False
                    print("Record: stopped")
                elif data == "START":
                    record_flag = True
                    print("Record: started")
                    data_queue.queue.clear()
                elif data == "EXIT":
                    exit_flag = True 
            except Exception as e:
                print(f"Error receiving data: {e}")
                break
            
            await asyncio.sleep(0)
            

    listen_for_input_task = loop.create_task(handle_input_client(conn))
    
    recording_task = loop.create_task(handle_recording(conn))

    await asyncio.gather(listen_for_input_task, recording_task)


asyncio.run(OpenPort())

print("Ended!!")