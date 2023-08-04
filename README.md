# C list

c_list is a library containing one data structure: list.
Types of lists implemented:
  - array list

Purpose of the project is educational. I want to learn how C external libs are being built, installed and used. <br>
Besides Is there a better way to have fun with pointers, than custom data structures? I haven't found it ;)

Ideas for lists implementation are taken from [wikipedia](https://en.wikipedia.org/wiki/List_of_data_structures#Lists). <br>
![image](https://github.com/KubaTaba1uga/c_list/assets/73971628/354cd4b4-d488-498d-955e-7b3f81dbffc4)


For tests I use [criterion](https://github.com/Snaipe/Criterion).
For build I use gcc and make.


## Installation

```
pip install foo
```

## Usage


```python
import foobar

# returns 'words'
foobar.pluralize('word')

# returns 'geese'
foobar.pluralize('goose')

# returns 'phenomenon'
foobar.singularize('phenomena')
```

### Why no negative indexes?
Negative indexes are forbidden by promoting `size_t` usage in functions' arguments.
I find negative indexes helpful, so version including them is in plan. 

## Test

```
make test && ./test
```

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[MIT](https://choosealicense.com/licenses/mit/)
