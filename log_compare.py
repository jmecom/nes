""" Compares NES emulator output log to known correct log """

import sys

NESTEST_LOG = open("test_files/nestest.log", "r").readlines()
OUT_LOG = open("out.log", "r").readlines()

def main():
    """ Compare output log to golden log, line by line """
    for i in xrange(len(OUT_LOG)):
        line1 = NESTEST_LOG[i].rstrip()
        line2 = OUT_LOG[i].rstrip()

        # Compare lines, but skip over the pretty-printing that my logger hasn't implemented yet
        if line1[0:20] != line2[0:20] or line1[49:88] != line2[49:88]:
            print "Logs differ\n    nestest.log: %s\n    out.log:     %s\n" % (line1, line2)
            sys.exit(1)

    print "No differences found so far. Good job!"

if __name__ == '__main__':
    main()
