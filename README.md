# C list

c_list is a library containing one data structure: list.
Types of lists implemented:
  - array list

Purpose of the project is educational. I want to learn how C external libs are being built, installed and used.

For tests I use [cmocka](https://cmocka.org/).
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
