from hypothesis import given, example, settings
import hypothesis.strategies as st

@given(st.integers(), st.integers())
def test_easy2(x, y):
    assert x != -42 and y != 939399

@given(st.integers(), st.integers())
@settings(max_examples=1000)
def test_easier2(x, y):
    if x == -42:
        assert False
        if y == 939399:
            assert False



if __name__ == '__main__':
    test_easy2()
    test_easier2()


