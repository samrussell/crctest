def calc_next_k(last_k):
    if last_k & 1:
        output = 0xedb88320 ^ (last_k >> 1)
    else:
        output = last_k >> 1
    
    return output


def calc_k(t):
    endt = t - 31
    output = 1

    while endt > 0:
        output = calc_next_k(output)
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

    lowest_bits = 9
    k = 1
    for i in range(32, 1024, 1):
        if not (i % (1000 * 1000 * 100)):
            print("%d" % i)
        k = calc_next_k(k)

        #if k.bit_count() < lowest_bits and not (i % 32):
        if k.bit_count() <= 10 and (k & 1):
            #if (0xffff0000 & k).bit_count() == 1 or (0x0000ffff & k).bit_count() == 1:
                #lowest_bits = k.bit_count()
            print("t: %d" % i)
            print("Output: %08X" % k)
            print("Binary: {0:0=32b}".format(k))
            print("Num bits: %d" % k.bit_count())
