import paho.mqtt.client as mqtt
import cv2
import mediapipe as mp
import time
import math
import numpy as np
from sympy import false

broker_address="broker.hivemq.com" #use external broker
print("creating new instance")
client = mqtt.Client("Python_P1") #create new instance
print("connecting to broker")
client.connect(broker_address,1883) #connect to broker

print("Subscribing to topic","espData")
client.subscribe("espData")
print("Publishing message to topic","colorData")
client.publish("colorData","Default")

cap = cv2.VideoCapture(1) #Use "http://192.168.<>.<>:8080/video" for streaming from smartphone
res = [1280,720]
# cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter.fourcc('P','I','M','1'))
cap.set(3,res[0])
cap.set(4,res[1])

mpHands = mp.solutions.hands #Start using hands tracking module // using mp.solutions.hands /.Hands /.process(img) /.multi_hand_landmarks /.landmark
hands = mpHands.Hands(static_image_mode=False,
                        max_num_hands=2,
                        model_complexity=1,
                        min_detection_confidence=0.2,
                        min_tracking_confidence=0.85)
mpDraw = mp.solutions.drawing_utils

pTime = 0
cTime = 0
lPosX = [0,0,0,0,0,0,0,0,80,0,0,0,80,0,0,0,0,0,0,0,0]
lPosY = [0,0,0,0,0,0,0,0,80,0,0,0,80,0,0,0,0,0,0,0,0]
rPosX = [0,0,0,0,0,0,0,0,80,0,0,0,80,0,0,0,0,0,0,0,0]
rPosY = [0,0,0,0,0,0,0,0,80,0,0,0,80,0,0,0,0,0,0,0,0]
dX = 0
dY = 0

def get_label(index, hand, results, resolution = [1280, 720]):
    output = None
    coords = []
    for idx, classification in enumerate(results.multi_handedness):
        if classification.classification[0].index == index:
            
            # Process results
            label = classification.classification[0].label
            
            # Extract Coordinates
            for i in range(21):
                coords.append(tuple(np.multiply(np.array((hand.landmark[i].x, hand.landmark[i].y)),resolution).astype(int)))
            
            output = label, coords
            
    return output
flagHeart = flagFriendzone = 0
flagSentData = []
for i in range(21):
    flagSentData.append(False)

while True:
    success, img = cap.read()
    img = cv2.flip(img, 1)

    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = hands.process(img_rgb)
    
    if results.multi_hand_landmarks:
            for num, hand in enumerate(results.multi_hand_landmarks):
                mpDraw.draw_landmarks(img, hand, mpHands.HAND_CONNECTIONS)
                
                # Render left or right detection
                if get_label(num, hand, results, [1280,720]):
                    # print("Two hand")
                    label, coords = get_label(num, hand, results)
                    
                    # cv2.putText(img, label, coords[0], cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2, cv2.LINE_AA)
                    if (label == "Right"):
                        if (coords[4][1]>coords[3][1]>coords[2][1] and coords[8][1]>coords[7][1]>coords[6][1]):
                            flagHeart = 1
                            # print("rf")
                        elif (not flagSentData[0]):
                            flagHeart = 0
                            print("Not send")
                            client.publish("colorData","Default",0)
                            flagSentData[0] = True
                            flagSentData[1] = False
                            flagSentData[2] = False
                    
                    if (label == "Left" and coords[4][1]>coords[3][1]>coords[2][1] and coords[8][1]>coords[7][1]>coords[6][1] and flagHeart):
                        if (not flagSentData[1]):
                            print(coords[4][1],">",coords[3][1],">",coords[2][1]," and ",coords[8][1],">",coords[7][1],">",coords[6][1])
                            print("Heart")
                            client.publish("colorData","Heart",0)
                            flagSentData[1] = True
                            flagSentData[0] = False
                            flagSentData[2] = False
                    if (label == "Left" and coords[4][1]<coords[3][1]<coords[2][1] and coords[4][1]<coords[11][1]<coords[15][1]<coords[20][1] and flagHeart):
                        if (not flagSentData[2]):
                            print("Friendzone")
                            client.publish("colorData","Friendzone",0)
                            print(coords[4][1],"<",coords[3][1],"<",coords[2][1]," and ",coords[4][1],"<",coords[11][1],"<",coords[15][1],"<",coords[20][1])
                            flagSentData[2] = True
                            flagSentData[0] = False
                            flagSentData[1] = False
                    
        
    # cTime = time.time()
    # fps = 1/(cTime-pTime)
    # pTime = cTime
    # cv2.putText(img, str(int(fps)), (10,70), cv2.FONT_HERSHEY_COMPLEX, 3, (0,255,255), 2)
    # cv2.imshow("Hand Tracking", img)
    
    if cv2.waitKey(1) & 0xFF == 27:
        break
cap.release() #close the program