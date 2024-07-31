from PIL import Image
import cv2
import numpy as np
import os
import time
import paho.mqtt.publish as publish     


detector_face = cv2.CascadeClassifier("C:/Users/te-te/OneDrive/Desktop/Projeto Final Embarcados/VISAO_COMPUTACIONAL/haarcascade_frontalface_default.xml")
reconhecedor_face = cv2.face.LBPHFaceRecognizer_create(radius=4, neighbors=14,grid_x=9,grid_y=9)
reconhecedor_face.read("C:/Users/te-te/OneDrive/Desktop/Projeto Final Embarcados/VISAO_COMPUTACIONAL/lbph_classifier.yml")

altura, largura = 220, 220
font = cv2.FONT_HERSHEY_COMPLEX_SMALL
camera = cv2.VideoCapture(0)

# Dicionário para contar detecções por pessoa
detected_counts = {}

while(True):
    ok, frame = camera.read()
    imagem_cinza = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    deteccoes = detector_face.detectMultiScale(imagem_cinza, scaleFactor=1.5, minSize=(30, 30))

    for(x,y,w,h) in deteccoes:
        imagem_face = cv2.resize(imagem_cinza[y:y + w, x:x + h], (largura, altura))
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0,0,255), 2)

        id, confindence = reconhecedor_face.predict(imagem_face)
        nome = ""

        if confindence > 70:  # Ajuste este limite conforme necessário
            if id in detected_counts:
                detected_counts[id] += 1
            else:
                detected_counts[id] = 1

            print(detected_counts)
            
            # Verificar se a pessoa foi detectada várias vezes antes de abrir a porta
            
            if detected_counts[id] >= 100:  # Ajuste o número de detecções necessárias
                if id == 21:
                    nome = "Matheus"
                    publish.single("poli/embarca", "matheus", hostname="broker.hivemq.com")
                else:
                    nome = "Nao identificado"
        else:
            nome = "Nao identificado"

        cv2.putText(frame, nome, (x, y+(w+30)), font, 2, (0,0,255))
        cv2.putText(frame, str(confindence), (x, y+(h+50)), font, 1, (0,0,255))

    cv2.imshow("Face", frame)
    if cv2.waitKey(1) == ord('q'):
        break

camera.release()
cv2.destroyAllWindows()
