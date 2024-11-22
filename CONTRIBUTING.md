# Contributing
This is an Open Source project, everybody is welcome to contribute with their knowledge and feedback. Before doing so, please read and make use of **ALL** documnetation on the matter (i.e., this file, the Code of Conduct, the Issue Template and the Pull Request Template).

## Repository management
It's recommended not to commit your changes directly to the main branch. Instead, create a branch with a descriptive name (such as `<my_name>-<my_change>` e.g, `a_salerno-new_parser`) and commit your changes there. Once your changes have been made **AND TESTED**, you can create a Pull Request to merge your code with the official main branch. When writing your Pull Request, please make use of the Pull Request Template.

## Code style
It'd be greatly appreciated if code written by contributors was formatted using `clang-format` with the `.clang-format` file included in this repository. Also, contributors are invited to take note of the following code style guidelines:
- Place declarations in the correct file and location (e.g.., place CLI function handler declarations in `cli.h`)
- Separate interfaces from implementations (i.e., do not write function implementations in header files)
- Use the `include` and `src` directories (and the respective subdirectories) **PROPERLY** (i.e., place header in `include`, common implementations in `src/common` and OS-dependent implementations in `src/<os-name>`)
- Use [Yoda notation](https://en.wikipedia.org/wiki/Yoda_conditions) when writing conditions (e.g., `1 == x` instead of `x == 1`)
- When checking if pointers are `NULL` use explicit statements (i.e., `NULL == ptr` instead of `!ptr`)
- Avoid branchless code unless:
  A. It is intuitive and easy to read
  B. It performs better than other implementations (must be tested)
- Avoid magic numbers, declare constants or macros to disambiguate
- Use guards instead of endless `if-else` statements
- Remember to clean up memory after it's been used (`goto` is not evil, use `goto cleanup` in guards instead of `return`)
- The use of unconventional structures (such as `goto`) is permitted if:
  A. It is intiutive and easy to read (e.g., `goto cleanup` or `goto emit` in guards)
  B. It performs better than other implementations (must be tested)
- Avoid excessive dynamic memory allocations, especially in "hot" areas (e.g., loops, request handler functions, etc.)
- Use common sense
