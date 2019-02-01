import numpy as np
import cv2
import random
import math
    
def generate_random_sample():
    dataclass = random.sample([0, 1], 1)
    objectclass = random.sample(range(0, 5), 1)
    if dataclass == 0:
        index = random.sample(range(0, 1011), 1)
    else:
        index = random.sample(range(0, 471), 1)
    
    return dataclass, objectclass, index

def find_puller(anchor_pose, coarse_list, coarse_poses, objectclass):
    mintheta = 1000.0
    index = 0
    for i in range(0, len(coarse_poses[objectclass])):
        theta =  np.abs(np.inner(anchor_pose, coarse_poses[objectclass][i]))
        if theta > 1:
            theta = 1
        theta = 2 * np.arccos(theta)
        if math.isnan(theta):
            break
        else:
            if mintheta > theta:
                mintheta = theta
                index = i
        
    puller_pose = coarse_poses[objectclass][index]
    puller_list = coarse_list[objectclass][index]
    
    return puller_list, puller_pose


def find_pusher(anchor_pose, coarse_list, coarse_poses, objectclass):
    
    randomobjectclass = random.sample(list(set(range(0, 5)).difference(set([objectclass]))),1)[0]
    randomindex = random.sample(range(0, len(coarse_list[randomobjectclass])), 1)[0]
    pusher_random_pose = coarse_poses[randomobjectclass][randomindex]
    pusher_random_list = coarse_list[randomobjectclass][randomindex]
    
    maxtheta = 0.0
    index = 0
    for i in range(0, len(coarse_poses[objectclass])):
        theta =  np.abs(np.inner(anchor_pose, coarse_poses[objectclass][i]))
        if theta > 1:
            theta = 1
        theta = 2 * np.arccos(theta)
        if math.isnan(theta):
            break
        else:
            if theta > maxtheta:
                maxtheta = theta
                index = i
        
    pusher_same_pose = coarse_poses[objectclass][index]
    pusher_same_list = coarse_list[objectclass][index]
    
    selector = random.sample([0,1], 1)[0]
    if selector == 0:
        pusher_list = pusher_random_list
        pusher_pose = pusher_random_pose
    else:
        
        pusher_list = pusher_same_list
        pusher_pose = pusher_same_pose
    
    return pusher_list, pusher_pose


path = "C:\\Users\\berko\\Desktop\\TDCV\\exercise3\\dataset"
"""
coarse_ape = cv2.imread(path + "\coarse\\ape")
coarse_benchvise = cv2.imread(path + "\coarse\\benchvise")
coarse_cam = cv2.imread(path + "\coarse\cam")
coarse_cat = cv2.imread(path + "\coarse\cat")
coarse_duck = cv2.imread(path + "\coarse\duck")

coarse_ape_poses = cv2.read_poses (path + "\coarse\\ape\poses.txt")
coarse_benchvise_poses = cv2.read_poses(path + "\coarse\\benchvise\poses.txt")
coarse_cam_poses = cv2.read_poses(path + "\coarse\cam\poses.txt")
coarse_cat_poses = cv2.read_poses(path + "\coarse\cat\poses.txt")
coarse_duck_poses = cv2.read_poses(path + "\coarse\duck\poses.txt")
"""
image_names = ["ape", "benchvise", "cam", "cat", "duck"]

coarse_list = []
coarse_poses = []

for i in range(0, len(image_names)):
    loop_list = []
    for j in range(0, 267):
        image = cv2.imread(path + "\\coarse\\" + image_names[i] + "\\coarse" + str(j) + ".png")
        image = image[:63, :63]
        image = image - np.average(image)
        image = image / np.std(image)
        loop_list.append(image)
    coarse_list.append(loop_list)

for i in range(0, len(image_names)):
    file = path + "\\coarse\\" + image_names[i] +"\\poses.txt"
    loop_list = []
    for line in open(file):
        if line.startswith('#'):
            continue
        if line.startswith('\n'):
            break
        else:
            qsub = line.split()
            for i in range(0, len(qsub)):
                qsub[i] = float(qsub[i])
            loop_list.append(qsub)
    coarse_poses.append(loop_list)

fine_list = []
fine_poses = []

for i in range(0, len(image_names)):
    loop_list = []
    for j in range(0, 1011):
        image = cv2.imread(path + "\\fine\\" + image_names[i] + "\\fine" + str(j) + ".png")
        image = image[:63, :63]
        image = image - np.average(image)
        image = image / np.std(image)
        loop_list.append(image)
    fine_list.append(loop_list)

for i in range(0, len(image_names)):
    file = path + "\\fine\\" + image_names[i] +"\\poses.txt"
    loop_list = []
    for line in open(file):
        if line.startswith('#'):
            continue
        if line.startswith('\n'):
            break
        else:
            qsub = line.split()
            for i in range(0, len(qsub)):
                qsub[i] = float(qsub[i])
            loop_list.append(qsub)
    fine_poses.append(loop_list)
            
real_list = []
real_poses = []
real_selected_poses = []

training_split = []
file = path + "\\real\\training_split.txt"
with open(file) as f:
    for line in f:
        training_split = line.strip().split(",")
    for i in range(0, len(training_split)):
        training_split[i] = int(training_split[i])
    

for i in range(0, len(image_names)):
    loop_list = []
    for j in training_split:
        image = cv2.imread(path + "\\real\\" + image_names[i] + "\\real" + str(j) + ".png")
        image = image[:63, :63]
        image = image - np.average(image)
        image = image / np.std(image)
        loop_list.append(image)
    real_list.append(loop_list)
    
real_temp_poses = []

for i in range(0, len(image_names)):
    file = path + "\\real\\" + image_names[i] +"\\poses.txt"
    loop_list = []
    for line in open(file):
        if line.startswith('#'):
            continue
        if line.startswith('\n'):
            break
        else:
            qsub = line.split()
            for i in range(0, len(qsub)):
                qsub[i] = float(qsub[i])
            loop_list.append(qsub)
    real_poses.append(loop_list)
    for k in training_split:
        real_temp_poses.append(real_poses[0][k])
    real_selected_poses.append(real_temp_poses)


training_list = [fine_list, real_list]
training_poses = [fine_poses, real_selected_poses]
    


index = range(1177)
test_index = list(set(index).difference(set(training_split)))

test_list = []
test_poses = []
test_temp_poses = []
test_selected_poses = []


for i in range(0, len(image_names)):
    loop_list = []
    for j in test_index:
        image = cv2.imread(path + "\\real\\" + image_names[i] + "\\real" + str(j) + ".png")
        image = image[:63, :63]
        image = image - np.average(image)
        image = image / np.std(image)
        loop_list.append(image)
    test_list.append(loop_list)

for i in range(0, len(image_names)):
    file = path + "\\real\\" + image_names[i] +"\\poses.txt"
    loop_list = []
    for line in open(file):
        if line.startswith('#'):
            continue
        if line.startswith('\n'):
            break
        else:
            qsub = line.split()
            for i in range(0, len(qsub)):
                qsub[i] = float(qsub[i])
            loop_list.append(qsub)
    test_poses.append(loop_list)
    for k in test_index:
        test_temp_poses.append(test_poses[0][k])
    test_selected_poses.append(test_temp_poses)
    
batch = []    
for i in range(0, 9):
    random_data, random_obj, random_index = generate_random_sample()
    anchor_pose = training_poses[random_data[0]][random_obj[0]][random_index[0]]
    anchor_list = training_list[random_data[0]][random_obj[0]][random_index[0]]
    puller_list, puller_pose = find_puller (anchor_pose, coarse_list, coarse_poses, random_obj[0])
    pusher_list, pusher_pose = find_pusher(anchor_pose, coarse_list, coarse_poses, random_obj[0])
    batch.append(anchor_list)
    batch.append(puller_list)
    batch.append(pusher_list)
    batch_array = np.stack(batch)

"""
cv2.imshow('', anchor_list)
cv2.waitKey()
"""
#exercise 2
