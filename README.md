## Synopsis

libtools
========================

Library with a lot of useful C routines.

## Code Example

Library contains a lot of routines, just few examples:

Reads entire file into a string:
~~~~~~~~~~~~~{.c}
void *data;
size_t len;

if (!get_file_contents(path, &data, &len)) {
	fprintf(stderr, "failed to get content of file %s\n", path);
}

free(data);
~~~~~~~~~~~~~

Write a string to a file:
~~~~~~~~~~~~~{.c}
const char *s = "Hello world!!";

if (put_file_contents("hello.txt", s, strlen(s))) {
	fprintf(stderr, "failed to write string '%s' to file\n", s);
}
~~~~~~~~~~~~~

## Motivation

I want to have one place for common C routines, which can be used in my C projects.

## Installation

~~~~~~~~~~~~~{.sh}
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr/local ..
make install
~~~~~~~~~~~~~

## API Reference

~~~~~~~~~~~~~{.cmake}
FIND_PACKAGE(libtools)
~~~~~~~~~~~~~

## Tests

## Contributors

- Maksym Kosharniy <kmvvmk@gmail.com>
- Oleh Kravchenko <oleg@kaa.org.ua>
- Ruslan Ternovyi <ruslanternovy@gmail.com>

## License

Copyright (C) 2016  Oleh Kravchenko <oleg@kaa.org.ua>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
