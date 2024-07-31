
from PIL import Image
import cv2 #pip install opencv-contrib-python
import numpy as np
import os

def get_image_data():
    paths = [os.path.join("C:/Users/te-te/OneDrive/Desktop/Projeto Final Embarcados/VISAO_COMPUTACIONAL/data", f) for f in os.listdir("C:/Users/te-te/OneDrive/Desktop/Projeto Final Embarcados/VISAO_COMPUTACIONAL/data")]
    #print(paths)
    faces = []
    ids = []

    for path in paths:
        #print(path)
        imagem = Image.open(path).convert('L')
        #print(type(imagem))
        imagem_np = np.array(imagem, 'uint8')


        #print(type(imagem_np))
        #print(os.path.split(path)[1])
        id = int(os.path.split(path)[1].split('.')[0].replace('subject', ''))
        #print(id)
        ids.append(id)
        faces.append(imagem_np)

    return np.array(ids), faces

ids, faces = get_image_data()

lbph_classifier = cv2.face.LBPHFaceRecognizer_create()
lbph_classifier.train(faces, ids)
lbph_classifier.write('lbph_classifier.yml')

lbph_face_classifier = cv2.face.LBPHFaceRecognizer_create()
lbph_face_classifier.read("C:/Users/te-te/OneDrive/Desktop/Projeto Final Embarcados/VISAO_COMPUTACIONAL/haarcascade_frontalface_default.xml")

print(ids)
