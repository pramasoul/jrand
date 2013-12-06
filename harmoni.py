# harmonic series is a fine way to burn CPU
s = 0.0
b = 0
while True:
    s += sum(1.0/n for n in xrange(10000000*b+1,10000000*(b+1)+1))
    print "Sum 1/n for in in [1..%d0000000] = %f" % (b+1, s)
    b += 1

