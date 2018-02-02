import hashlib
import sys

count0 = 0
file = open("file2.txt", "r")
str = file.read()
print 'Word to be hashed:{}'.format(str)

hash_object2 = []
hash_object1 = hashlib.md5(str)
hash_object2.append(hash_object1.hexdigest())
hash_object4 = []
hash_object3 = hashlib.sha512(str)
hash_object4.append(hash_object3.hexdigest())
hash_object6 = []
hash_object5 = hashlib.new('ripemd160', str)
hash_object6.append(hash_object5.hexdigest())

print 'Hash value of word in MD5:{}'.format(hash_object2)
print 'Hash value of word in SHA512:{}'.format(hash_object4)
print 'Hash value of word in RIPEMD160:{}'.format(hash_object6)
print 'Select algorithm 1-MD5 2-SHA512 3-RIPEMD160'

type = raw_input()
print 'Select no of bytes to match 1 or 2'
bytes = raw_input()

if(int(type) > 3 or int(bytes) > 2):
	print 'Wrong selection'
	sys.exit()

s = open("/usr/share/dict/words", "r")

for line in s:
	if(int(type) ==1):
		ha_obj1 = []
		ha_obj0 = hashlib.md5(line)
		ha_obj1.append(ha_obj0.hexdigest())
	#print ha_obj1
	if (int(bytes) == 1):
	if (ha_obj1[0][0:2] == hash_object2[0][0:2]):
		print 'Match found for 1 byte md5:{}'.format(ha_obj1)
		count0=count0+1
	elif (int(bytes) == 2):
		if (ha_obj1[0][0:4] == hash_object2[0][0:4]):
			print 'Match found for 2 byte md5:{}'.format(ha_obj1)
			count0=count0+1
		elif(int(type) ==2):
			ha_obj1 = []
			ha_obj0 = hashlib.sha512(line)
			ha_obj1.append(ha_obj0.hexdigest())

	#print ha_obj1
	if (int(bytes) == 1):
		if (ha_obj1[0][0:2] == hash_object4[0][0:2]):
			print 'Match found for 1 byte sha512:{}'.format(ha_obj1)
			count0=count0+1
	elif (int(bytes) == 2):
		if (ha_obj1[0][0:4] == hash_object4[0][0:4]):
			print 'Match found for 2 byte sha512:{}'.format(ha_obj1)
			count0=count0+1
	elif(int(type) ==3):
		ha_obj1 = []
		ha_obj0 = hashlib.new('ripemd160', line)
		ha_obj1.append(ha_obj0.hexdigest())
	#print ha_obj1
	if (int(bytes) == 1):
		if (ha_obj1[0][0:2] == hash_object6[0][0:2]):
			print 'Match found for 1 byte rmd160:{}'.format(ha_obj1)
			count0=count0+1
	elif (int(bytes) == 2):
		if (ha_obj1[0][0:4] == hash_object6[0][0:4]):
			print 'Match found for 2 byte rmd160:{}'.format(ha_obj1)
			count0=count0+1
		else:
			print 'Wrong input'
			sys.exit()
print 'Number of matches:{}'.format(count0)