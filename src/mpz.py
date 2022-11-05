from __future__ import annotations

from copy import deepcopy

MPZ_INIT_ALLOC_SIZE = 1  # TODO: make it 1024 after testing
UINT_64_MAX = (1 << 64) - 1
UINT_32_MAX = (1 << 32) - 1


class mpz:
    _data: list[int]
    _size: int
    _alloc_size: int

    def __init__(self, size: int = 1):
        assert size >= 1

        self._data = [0] * MPZ_INIT_ALLOC_SIZE
        self._size = size
        self._alloc_size = MPZ_INIT_ALLOC_SIZE

        # in case size > MPZ_INIT_ALLOC_SIZE
        self._alloc_more_if_necessary()

    def _alloc_more_if_necessary(self):
        while self._size > self._alloc_size:
            print(f'GROWING MEMORY: {self._alloc_size} ➡️ {self._alloc_size * 2}')
            self._data += [0] * self._alloc_size
            self._alloc_size *= 2

    def _trim(self):
        for i in reversed(range(self._size)):
            if self[i]:
                self._size = i + 1
                return

        self._size = 0

    def __str__(self):
        return ''.join(format(v, '032b') for v in reversed(self._data[:self._size]))

    def __getitem__(self, index: int) -> int:
        return self._data[index]

    def __setitem__(self, index: int, value: int) -> None:
        assert 0 <= value <= UINT_32_MAX, 'Only uint32 type-compatible values are allowed'

        if self._size == index:
            self._size += 1
            self._alloc_more_if_necessary()

        self._data[index] = value

    def op_ui(self, value: int, index: int, op):
        assert 0 <= value <= UINT_32_MAX

        new_value = op(self[index], value)
        assert 0 <= new_value <= UINT_64_MAX

        overflow = new_value // (1 << 32)
        remainder = new_value % (1 << 32)

        self[index] = remainder

        if overflow:
            self.add_ui(overflow, index + 1)

    def add_ui(self, value: int, index: int = 0):
        if value == 0:
            return

        self.op_ui(value, index, int.__add__)

    def mul_ui(self, value: int, index: int = -1):
        if value == 1:
            return

        if index < 0:
            index = self._size - 1

        self.op_ui(value, index, int.__mul__)

        if index:
            self.mul_ui(value, index - 1)

    def add(self, other: mpz):
        for i, v in reversed(list(enumerate(other))):
            self.add_ui(v, i)

    def mul(self, other: mpz):
        result = mpz(
            size=
            self._size  # enough to fit original data
            + other._size - 1  # maximum shift (due to multiplication)
            + 1  # last group may overflow (due to addition)
        )

        #  0  1  2  2  2  2 : other_range_max
        #  1  2  3  3  2  1 : other_range_len
        # 1a 2a 3a 4a
        #    1b 2b 3b 4b
        #       1c 2c 3c 4c
        for i in reversed(range(result._size)):
            other_range_max = min(i, other._size - 1)
            other_range_len = min(i, result._size - 1 - i) + 1

            for original_idx, other_idx in zip(
                    range(min(i, self._size - 1), -1, -1),
                    range(other_range_max - (other_range_len - 1), other_range_max + 1)):

                new_value = self[original_idx] * other[other_idx]
                assert 0 <= new_value <= UINT_64_MAX

                overflow = new_value // (1 << 32)
                remainder = new_value % (1 << 32)

                result.add_ui(remainder, i)
                result.add_ui(overflow, i + 1)

        result._trim()

        # copy result to self (or just use a return)
        self._data = result._data
        self._size = result._size
        self._alloc_size = result._alloc_size


if __name__ == '__main__':
    a = mpz(4)
    a._data = [1, 2, 3, 4]

    b = mpz(3)
    b._data = [10, 20, 30]

    a.mul(b)

    print(a)
