import sys 

def compare(a, b, fro, to, radix):
    if int(a[fro:to], radix) != int(b[fro:to], radix):
        print("Logs differ\n %s\n %s" % (a, b))
        sys.exit(1)

# Compare the two logs, line by line
with open("test_files/nestest.log", "r") as golden_log:
    with open("out.log", "r") as my_log:
        for a in golden_log:
            for b in my_log:
                compare(a, b, 0, 4,   16) # pc
                compare(a, b, 6, 8,   16) # opcode
                compare(a, b, 10, 12, 16) # arg1
                compare(a, b, 12, 14, 16) # arg2
                compare(a, b, 50, 52, 16) # a
                compare(a, b, 55, 57, 16) # x
                compare(a, b, 60, 62, 16) # y
                compare(a, b, 65, 67, 16) # p
                compare(a, b, 71, 73, 16) # sp
                compare(a, b, 78, 81, 10) # cyc
                compare(a, b, 86, 89, 10) # sl

print("No differences found. Good job!")