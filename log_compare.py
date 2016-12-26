""" Compares NES emulator output log to known correct log """

import sys

NESTEST_LOG = open("test_files/nestest.log", "r").readlines()
OUT_LOG = open("out.log", "r").readlines()
# Ranges to compare on each line (e.g. ranges for opcode, args, registers, ...).
RANGES = [(0, 4), (6, 8), (10, 12), (12, 14), (50, 52), (55, 57), (60, 62),
          (65, 67), (71, 73), (78, 81), (86, 89)]
# Radices to convert from. All but CYC and SL are base 16.
RADICES = [16, 16, 16, 16, 16, 16, 16, 16, 16, 10, 10]
NUM_TO_COMPARE = len(RANGES)

def check_exists(line, start, end):
    """ Check if a number exists at line[start:end] """
    try:
        int(line[start:end])
        return True
    except ValueError:
        return False

def compare(line1, line2, start, end, radix):
    """ Compare integer values line1 and line2 at range [start, end] for a given radix  """
    if int(line1[start:end], radix) != int(line2[start:end], radix):
        print "Logs differ\n    nestest.log: %s    out.log:     %s" % (line1, line2)
        sys.exit(1)

def main():
    """ Compare output log to golden log, line by line """
    for i in xrange(len(OUT_LOG)):
        for j in xrange(NUM_TO_COMPARE):
            start = RANGES[j][0]
            end = RANGES[j][1]
            if check_exists(NESTEST_LOG[i], start, end):
                compare(NESTEST_LOG[i], OUT_LOG[i], start, end, RADICES[j])
    print "No differences found so far. Good job!"

if __name__ == '__main__':
    main()
