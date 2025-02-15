# `spbi`

`spbi` is a CLI application written in C for splitting bills easily.

## Installation

Simply clone the project, then build and optionally install the executable using `make`.

```bash
git clone https://github.com/sorousherafat/spbi.git
cd spbi/
make
sudo make install
```

## Usage

> [!IMPORTANT]  
> Before running `spbi`, please ensure that both `cpp` (the C preprocessor) and `column` are installed and available on your `PATH`. Both are usually available by default on a UNIX machine.

`spbi` takes a single argument: the path to an input file. It reads this file, processes its content, and outputs the result to the standard output.

### Input File Format

The input file should contain a series of statements in the following format:

```
<person> += <amount> : <people>
```

- **`<person>`**: The identifier of the individual who has loaned money.
- **`<amount>`**: A number or a more complex expression representing the loaned amount.
- **`<people>`**: A list of recipients. This list is defined by identifiers separated by the `+` and `-` operators. The `+` operator includes a person in the split, while the `-` operator excludes them, if they are included on the list.

For example:

```
alice += 100 * 2 : bob + charlie + dave - dave
```

indicates that `alice` has loaned an amount of 100 which will be split equally among `bob` and `charlie` (with `dave` excluded).

### Builtin Functions and Preprocessing

- **`count`**: A builtin function that takes a list of people as input and returns the number of people. This can be useful for multiplying a single fee by the number of recipients.

Before processing, `spbi` runs the C preprocessor (`cpp`) on the input file. This allows you to write macros for more powerful expressions. For instance, you might define:

```c
#define all a + b + c
```

After which, using `all - c` in your file will represent the list consisting of `a` and `b`.

## License

This project is licensed under the [GPL-3.0 license](https://github.com/sorousherafat/spbi/blob/main/LICENSE). 
