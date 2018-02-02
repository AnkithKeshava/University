import time
import hashlib
import sys

i = 0
prevtx = "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks"
prevheader = hashlib.sha256(prevtx)
nmax = 100
nm = 1
maxnonce = 2**5

while(nm < nmax):
	start_time = time.time()
	target = int(2**(256-nm))
	difficulty = 2**nm
	nexttx = "I, Ankith, give Rishul 10 BTC-282, on November1, 2016"
	for i in range(0, maxnonce):
		nonce = str(i)
		#print 'Nonce value {}'.format(nonce)
		text = str(prevheader.hexdigest())+str(nexttx)+str(nonce)
		hash_i = hashlib.sha256(text)
		if(hash_i < hex(target)):
			print 'For number {} difficulty {}'.format(nm, difficulty)
			print 'Matching hash found {}'.format(hash_i.hexdigest())
			print 'For nexttx {}'.format(nexttx)
			print 'For nonce value {}'.format(nonce)
			end_time = time.time()
			elapsed_time = end_time-start_time
			print 'Elapsed time {}'.format(elapsed_time)
			print 'CPU hash speed {}'.format(int(nonce)/elapsed_time)
		else:
			print 'Failed after {} tries.'.format(maxnonce)
		nm=nm+1
		print '$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$'
print 'End of computation