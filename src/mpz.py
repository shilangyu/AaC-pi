MPZ_INIT_ALLOC_SIZE = 1  # TODO: make it 1024 after testing
UINT_64_MAX = (1 << 64) - 1
UINT_32_MAX = (1 << 32) - 1


class mpz:
    data: list[int]
    used_size: int
    alloc_size: int

    def __init__(self, value: int = 0):
        self.data = [value]
        self.used_size = 1
        self.alloc_size = MPZ_INIT_ALLOC_SIZE

    # TODO: remove after testing
    def validate(self):
        if not all(v <= UINT_32_MAX for v in self.data):
            print('🚨 VALIDATION FAILED')

    def __str__(self):
        self.validate()

        return ''.join(format(v, '032b') for v in reversed(self.data[:self.used_size]))

    def grow(self, index: int):
        if self.used_size == index:
            self.used_size += 1

        if self.used_size > self.alloc_size:
            print(f'GROWING MEMORY: {self.alloc_size} ➡️ {self.alloc_size * 2}')
            self.data += [0] * self.alloc_size
            self.alloc_size *= 2

    def op_ui(self, value: int, index: int, op):
        assert 0 <= value <= UINT_32_MAX

        self.grow(index)

        new_value = op(self.data[index], value)
        assert 0 <= new_value <= UINT_64_MAX

        overflow = new_value // (1 << 32)
        remainder = new_value % (1 << 32)

        self.data[index] = remainder

        if overflow:
            self.add_ui(overflow, index + 1)

    def add_ui(self, value: int, index: int = 0):
        self.op_ui(value, index, int.__add__)

    def mul_ui(self, value: int, index: int = -1):
        if index < 0:
            index = self.used_size - 1

        self.op_ui(value, index, int.__mul__)

        if index:
            self.mul_ui(value, index - 1)


if __name__ == '__main__':
    mpz = mpz()
    mpz.add_ui(UINT_32_MAX)
    mpz.mul_ui(UINT_32_MAX)
    mpz.add_ui(UINT_32_MAX)
    mpz.mul_ui(UINT_32_MAX)
    mpz.mul_ui(UINT_32_MAX // 4)
    mpz.add_ui(255)

    print(mpz)
