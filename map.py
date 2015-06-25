import random

# map list
arrMap = [-0.3, 0.3, -0.35, 0.35, -0.4, 0.4, -0.45, 0.45]#9

mapId = 1;
x = -200
x1 = 0
x2 = 0
y = 0
dy = 0
file_object = open('mapPoints'+str(mapId) + '.json', 'w')
str1 = '[["x","y"]\n'
str1 += ",["  + str(x) + ", " + str(y) + "]\n"
x += 800
maxy = 0
miny = 0
# map
for i in range(0,240):
    str1 += ",["  + str(x) + ", " + str(y) + "]\n"
    print str1
    x1 = x
    if i < 300:
        x += random.randint(200,500)
        dy = random.uniform(arrMap[0]- 0.02*mapId,arrMap[1] + 0.02*mapId)
    elif i < 160:
        x += random.randint(180,400)
        dy = random.uniform(arrMap[2]- 0.02*mapId,arrMap[3] + 0.02*mapId)
    elif i < 240:
        x += random.randint(160,300)
        dy = random.uniform(arrMap[4]- 0.02*mapId,arrMap[5] + 0.02*mapId)
    else:
        x += random.randint(140,300)
        dy = random.uniform(arrMap[6]- 0.02*mapId,arrMap[7] + 0.02*mapId)
    x2 =x
    y =(x2 - x1) * dy
    if y < 50 and y > 0:
        y += 100 + 10 * mapId + i / 10
    if y > -50 and y < 0:
        y -= 100 - 10 * mapId - i / 10
    y = int(y)
    if y > 250:
        y = 246
    if y < -250:
        y = -246
    if x>80000:
        y = 0
    maxy = max(maxy, y)
    miny = min(miny, y)
    #print i
file_object.write(str1)
file_object.write("]")
file_object.close( )

print maxy
print miny

