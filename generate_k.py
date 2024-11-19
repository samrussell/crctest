def calc_k(t):
    endt = t - 31
    output = 1

    while endt > 0:
        if output & 1:
            output = 0xedb88320 ^ (output >> 1)
        else:
            output = output >> 1
        endt -= 1

    return output

def test_double(num):
    for i in range(16):
        divisor = (1 << (16 - i)) + 1
        if not (num % divisor):
            return divisor
    return 0

def test_pairs(num):
    return (not (num % 3)) and ((num.bit_count() // 2) == (num // 3).bit_count())

if __name__ == '__main__':
    import argparse

    #parser = argparse.ArgumentParser()
    #parser.add_argument('t', type=int)
    #args = parser.parse_args()

    lowest_bits = 32
    for i in range(4096 * 8 * 128):
        k = calc_k(i + 32)

        #if k.bit_count() < lowest_bits and not (i % 32):
        if test_pairs(k) and k.bit_count() < lowest_bits:
            #for j in range(16):
            #    if not ((0xffffffff ^ (0xffff << j)) & k):
            lowest_bits = k.bit_count()
            print("t: %d" % i)
            print("Output: %08X" % k)
            print("Binary: {0:0=32b}".format(k))
            print("Num bits: %d" % k.bit_count())
