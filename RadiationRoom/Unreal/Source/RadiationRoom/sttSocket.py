import socket
import struct
import os
import numpy as np
import speech_recognition as sr
import whisper
import torch
from datetime import datetime, timedelta, timezone
from queue import Queue

DEFAULT_PORT = 7070

PORT = DEFAULT_PORT
HOST_IP = "127.0.0.1"

# The last time a recording was retrieved from the queue.

def OpenPort():

    audio_model = whisper.load_model("base.en")
    phrase_timeout = 3 #args.phrase_timeout
    record_timeout = 3

    # Thread safe Queue for passing data from the threaded recording callback.
    data_queue = Queue()

    # We use SpeechRecognizer to record our audio because it has a nice feature where it can detect when speech ends.
    recorder = sr.Recognizer()
    recorder.energy_threshold = 1000 # args.energy_threshold
    # Definitely do this, dynamic energy compensation lowers the energy threshold dramatically to a point where the SpeechRecognizer never stops recording.
    recorder.dynamic_energy_threshold = False

    source = sr.Microphone(sample_rate=16000)

    with source:
        recorder.adjust_for_ambient_noise(source)

    def record_callback(_, audio:sr.AudioData) -> None:
        """
        Threaded callback function to receive audio data when recordings finish.
        audio: An AudioData containing the recorded bytes.
        """
        # Grab the raw bytes and push it into the thread safe queue.
        data = audio.get_raw_data()
        data_queue.put(data)

    # Create a background thread that will pass us raw audio bytes.
    # We could do this manually but SpeechRecognizer provides a nice helper.
    recorder.listen_in_background(source, record_callback, phrase_time_limit=record_timeout)

    # Cue the user that we're ready to go.
    print("Model loaded.\n")

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST_IP, PORT))
        s.listen()
        print("starting to listen")
        print('', end='', flush=True)
        conn, addr = s.accept()
        
        with conn:
            print('Connected by', addr)
            print('', end='', flush=True)
            
            transcription = ['']

            phrase_time = datetime.now(timezone.utc)

            while True:
                now = datetime.now(timezone.utc)

                # Pull raw recorded audio from the queue.
                if not data_queue.empty():
                    # phrase_complete = False
                    
                    # If enough time has passed between recordings, consider the phrase complete.
                    # Clear the current working audio buffer to start over with the new data.
                    # if phrase_time and now - phrase_time > timedelta(seconds=phrase_timeout):
                    #     phrase_complete = True
                    
                    # This is the last time we received new audio data from the queue.
                    phrase_time = now
                    
                    response = bytes()
                    
                    # Combine audio data from queue
                    audio_data = b''.join(data_queue.queue)
                    data_queue.queue.clear()
                    
                    # Convert in-ram buffer to something the model can use directly without needing a temp file.
                    # Convert data from 16 bit wide integers to floating point with a width of 32 bits.
                    # Clamp the audio stream frequency to a PCM wavelength compatible default of 32768hz max.
                    audio_np = np.frombuffer(audio_data, dtype=np.int16).astype(np.float32) / 32768.0

                    # Read the transcription.
                    result = audio_model.transcribe(audio_np, fp16=torch.cuda.is_available())
                    text = result['text'].strip()

                    # If we detected a pause between recordings, add a new item to our transcription.
                    # Otherwise edit the existing one.
                    # if phrase_complete:
                    transcription.append(text)
                    # else:
                    #     transcription[-1] = text

                    # # Clear the console to reprint the updated transcription.
                    os.system('cls' if os.name=='nt' else 'clear')
                    for line in transcription:
                        if line != '':
                            print(line)
                            response = response + bytes(line, 'utf-8')

                    # # Flush stdout.
                    print('', end='', flush=True)

                    pack = struct.pack("I%ds" % (len(response),), len(response), response)

                    conn.sendall(pack)

                elif now - phrase_time > timedelta(seconds=record_timeout*1.3):
                    message_has_content = False
                    for line in transcription:
                        if line != '': 
                            message_has_content = True
                            print("Message has content", transcription)
                            break
                    
                    if message_has_content:
                        message = bytes("1234code", "utf-8")
                        pack = struct.pack("I%ds" % (len(message),), len(message), message)
                        conn.sendall(pack)
                        os.system('cls' if os.name=='nt' else 'clear')
                        print("MESSAGE SENT", transcription)
                    
                    transcription = []
                    phrase_time = now

OpenPort()