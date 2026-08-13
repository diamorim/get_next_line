*This project has been created as part of the 42 curriculum by damorim-.*

# get_next_line

## Description

`get_next_line` is a 42 School project that consists of writing a function
able to read a text file (or any file descriptor, including standard input
or a socket) and return it **one line at a time**, on each call, without
knowing in advance the size of the line or the file.

The goal of the project is to understand and practise:

- **Static variables**, to keep data alive between successive calls to the
  same function.
- **Reading in fixed-size chunks**, since a file cannot be read all at once
  in a real-world / memory-constrained scenario.
- Manual **memory management** in C (`malloc`/`free`), avoiding leaks while
  building and shrinking dynamically-sized buffers.

The project exposes a single public function:

```c
char *get_next_line(int fd);
```

Each call returns the next line read from the file descriptor `fd`,
including the trailing `\n` if there is one, or `NULL` when there is
nothing left to read (end of file) or an error occurs.

## Instructions

### Compilation

The project is built with the provided `Makefile`, which compiles the
sources and archives them into a static library.

```sh
make        # builds get_next_line (static library)
make clean  # removes object files
make fclean # removes object files AND the compiled library
make re     # fclean + all
```

The read buffer size is controlled at compile time through the
`BUFFER_SIZE` macro, defined in the `Makefile` (default: `42`). It can be
overridden at build time, for example:

```sh
make CFLAGS="-Wall -Werror -Wextra -D BUFFER_SIZE=1"
```

### Usage

To use `get_next_line` in your own project:

1. Copy `get_next_line.c`, `get_next_line_utils.c`, and `get_next_line.h`
   into your project.
2. Include the header and call the function in a loop until it returns
   `NULL`:

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    char    *line;

    fd = open("some_file.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 main.c get_next_line.c get_next_line_utils.c -o gnl_test
./gnl_test
```

### Testing

There is no bundled test suite in this repository. Manual testing was done
by reading files of various sizes (empty files, files with and without a
trailing newline, files larger than `BUFFER_SIZE`) with several
`BUFFER_SIZE` values (`1`, `42`, large values), and by reading from
multiple file descriptors in the same run to check that the static state
does not leak between them.

## Technical Choices — Algorithm & Data Structure

The core design decision of `get_next_line` is how to remember, between
two separate calls, the part of the file that was already read from disk
but not yet returned to the caller. The chosen approach is:

- **A `static char *stash` buffer.** Because a static variable keeps its
  value across function calls and is scoped to the function (here shared
  by all calls for all file descriptors, since the subject's mandatory
  part only requires a single `fd` to work correctly at a time), it is
  used to accumulate everything read from `fd` that hasn't been handed
  back to the caller yet. This avoids any global variable while still
  giving the "memory" needed between calls.

- **Chunked reading with `read()` into a fixed `BUFFER_SIZE` buffer.**
  `fill_stash` calls `read` repeatedly, `BUFFER_SIZE` bytes at a time, and
  appends each chunk to the stash with `ft_strjoin`, until either a `\n`
  is found in the stash or `read` returns `0`/`-1` (end of file or error).
  Reading in fixed chunks — rather than the whole file at once — is the
  whole point of the exercise: it keeps memory usage bounded regardless of
  file size and mirrors how `read()` is meant to be used on any file
  descriptor, including non-seekable ones like pipes or sockets.

- **Three cooperating helper functions, one responsibility each:**
  - `fill_stash` — grows the stash until it contains at least one full
    line (or the file is exhausted).
  - `extract_line` — scans the stash and allocates/returns a fresh copy
    of just the first line (up to and including `\n`), leaving the stash
    itself untouched.
  - `next_stash` — rebuilds the stash without the line that was just
    extracted, freeing the old buffer, so the next call starts exactly
    where this one left off.

  Splitting the algorithm this way keeps each function under the 42 norm's
  line-count limit and makes it possible to reason about, and test, each
  step independently.

- **`ft_strjoin` reimplementation.** A local `ft_strjoin` (distinct from
  the `libft` one) is used to concatenate the existing stash with each new
  chunk read from the file, freeing the old stash automatically. This
  keeps `get_next_line.c` self-contained and avoids a `libft` dependency
  for a project that is meant to be usable as a small, standalone unit.

- **Time and space complexity.** Each byte of the file is read exactly
  once and copied a bounded number of times (once into the stash, once
  when a line is extracted, once when the remainder is shifted into the
  next stash), giving overall O(n) time in the size of the file. Memory
  usage at any point in time is proportional to the length of the longest
  line encountered, not the size of the whole file.

## Resources

Classic references used while working on the topic:

- [`read(2)` — Linux man page](https://man7.org/linux/man-pages/man2/read.2.html) — reference for `read()`'s behaviour, return values, and short reads.
- [`open(2)` — Linux man page](https://man7.org/linux/man-pages/man2/open.2.html) — reference for opening files and file descriptor basics.
- [C Standard, static storage duration — cppreference](https://en.cppreference.com/w/c/language/storage_duration) — documentation on how `static` local variables retain their value across calls.
- GeeksforGeeks.
- Youtube.

### AI Usage

An AI assistant (Claude) was used strictly as a documentation and review
aid, **not** to write or debug the C implementation itself:

- Drafting and structuring this `README.md` (Description, Instructions,
  Technical Choices, and Resources sections) from the existing source code
  and Makefile.
- Explaining, in the "Technical Choices" section above, the reasoning
  behind design decisions (the `static` stash buffer, chunked reading,
  the split between `fill_stash`/`extract_line`/`next_stash`) that were
  already implemented in the code, so they could be documented clearly
  and accurately.
- Testing.

All `get_next_line` logic — the static-state management, the buffered
reading loop, and the line-extraction logic — was designed, written, and
debugged manually, in line with the 42 evaluation requirement that the
code itself reflects the student's own understanding.