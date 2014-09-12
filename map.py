import random
# map1
map1_mindy1_map1 = -0.1
map1_maxdy1_map1 = 0.1
map1_mindy2_map1 = -0.15
map1_maxdy2_map1 = 0.15
map1_mindy3_map1 = -0.2
map1_maxdy3_map1 = 0.2
map1_mindy4_map1 = -0.25
map1_maxdy4_map1 = 0.25

# map2
mindy1_map2 = -0.125
maxdy1_map2 = 0.125
mindy2_map2 = -0.175
maxdy2_map2 = 0.175
mindy3_map2 = -0.225
maxdy3_map2 = 0.225
mindy4_map2 = -0.275
maxdy4_map2 = 0.275
# map3
mindy1_map3 = -0.15
maxdy1_map3 = 0.15
mindy2_map3 = -0.2
maxdy2_map3 = 0.2
mindy3_map3 = -0.25
maxdy3_map3 = 0.25
mindy4_map3 = -0.3
maxdy4_map3 = 0.3

# map4
mindy1_map4 = -0.175
maxdy1_map4 = 0.175
mindy2_map4 = -0.225
maxdy2_map4 = 0.225
mindy3_map4 = -0.275
maxdy3_map4 = 0.275
mindy4_map4 = -0.325
maxdy4_map4 = 0.325

# map5
mindy1_map5 = -0.2
maxdy1_map5 = 0.2
mindy2_map5 = -0.25
maxdy2_map5 = 0.25
mindy3_map5 = -0.375
maxdy3_map5 = 0.375
mindy4_map5 = -0.4
maxdy4_map5 = 0.4

# map6
mindy1_map6 = -0.225
maxdy1_map6 = 0.225
mindy2_map6 = -0.275
maxdy2_map6 = 0.275
mindy3_map6 = -0.4
maxdy3_map6 = 0.4
mindy4_map6 = -0.425
maxdy4_map6 = 0.425

# map7
mindy1_map7 = -0.25
maxdy1_map7 = 0.25
mindy2_map7 = -0.3
maxdy2_map7 = 0.3
mindy3_map7 = -0.425
maxdy3_map7 = 0.425
mindy4_map7 = -0.45
maxdy4_map7 = 0.45

# map8
mindy1_map8 = -0.275
maxdy1_map8 = 0.275
mindy2_map8 = -0.325
maxdy2_map8 = 0.325
mindy3_map8 = -0.45
maxdy3_map8 = 0.45
mindy4_map8 = -0.475
maxdy4_map8 = 0.475

# map9
mindy1_map9 = -0.3
maxdy1_map9 = 0.3
mindy2_map9 = -0.35
maxdy2_map9 = 0.35
mindy3_map9 = -0.475
maxdy3_map9 = 0.475
mindy4_map9 = -0.5
maxdy4_map9 = 0.5

# map10
mindy1_map10 = -0.325
maxdy1_map10 = 0.325
mindy2_map10 = -0.375
maxdy2_map10 = 0.375
mindy3_map10 = -0.5
maxdy3_map10 = 0.5
mindy4_map10 = -0.525
maxdy4_map10 = 0.525


# map list
arrMap = [[-0.1, 0.1, -0.15, 0.15, -0.2, 0.2, -0.25, 0.25],#1
          [-0.125, 0.125, -0.175, 0.175, -0.225, 0.225, -0.275, 0.275],#2
          [-0.15, 0.15, -0.2, 0.2, -0.25, 0.25, -0.3, 0.3],#3
          [-0.175, 0.175, -0.225, 0.225, -0.275, 0.275, -0.325, 0.325],#4
          [-0.2, 0.2, -0.25, 0.25, -0.3, 0.3, -0.35, 0.35],#5
          [-0.225, 0.225, -0.275, 0.275, -0.325, 0.325, -0.375, 0.375],#6
          [-0.25, 0.25, -0.3, 0.3, -0.35, 0.35, -0.4, 0.4], #7
          [-0.275, 0.275, -0.325, 0.325, -0.375, 0.375, -0.425, 0.425],#8
          [-0.3, 0.3, -0.35, 0.35, -0.4, 0.4, -0.45, 0.45],#9
          [-0.325, 0.325, -0.375, 0.375, -0.425, 0.425, -0.475, 0.475]];#10

mapId = 10;
x = -200
dy = 0
x1 = 0
x2 = 0
y = 0
dy = 0
file_object = open('mapPoints'+str(mapId) + '.json', 'w')
str1 = '[["x","y"]\n'
str1 += ",["  + str(x) + ", " + str(y) + "]\n"
x += 800;

for i in range(0,100):
    str1 += ",["  + str(x) + ", " + str(y) + "]\n"
    print str1
    x1 = x
    if i < 25:
        x += random.randint(100,550)
        dy = random.uniform(arrMap[mapId-1][0],arrMap[mapId-1][1])
    elif i < 50:
        x += random.randint(150,500)
        dy = random.uniform(arrMap[mapId-1][2],arrMap[mapId-1][3])
    elif i < 75:
        x += random.randint(200,450)
        dy = random.uniform(arrMap[mapId-1][4],arrMap[mapId-1][5])
    else:
        x += random.randint(250,400)
        dy = random.uniform(arrMap[mapId-1][6],arrMap[mapId-1][7])
    x2 =x
    y = dy + (x2 - x1) * dy
    y = int(y)
    #print i
file_object.write(str1)
file_object.write("]")
file_object.close( )

