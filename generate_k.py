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

    parser = argparse.ArgumentParser()
    parser.add_argument('t', type=int)
    args = parser.parse_args()

    k = calc_k(args.t)
    k_binary_reversed = "{0:0=32b}".format(k)[::-1]
    k_reversed = int(k_binary_reversed, 2)

    print("t: %d" % args.t)
    print("Output: %08X" % k)
    print("Binary: {0:0=32b}".format(k))
    print("Output (reversed): %08X" % k_reversed)
    print("Binary (reversed): %s" % k_binary_reversed)
    print("Num bits: %d" % k.bit_count())
