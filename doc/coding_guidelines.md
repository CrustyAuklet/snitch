# Guidelines for writing C++ code for *snitch*

Unless otherwise stated, follow the [C++ Code Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines). Below are exceptions to these guidelines, or more opinionated choices.


## `noexcept`

Follow the [Lakos Rule](https://quuxplusone.github.io/blog/2018/04/25/the-lakos-rule/) (with some tweaks):
 - Functions with a wide contract (i.e., with no precondition) should be marked as unconditionally `noexcept`.
 - Functions with a narrow contract (i.e., with preconditions), should not be marked as `noexcept`.
 - If a template function is conditionally-wide, (e.g., its purpose is only to be a wrapper or adapter over another function), then it may be marked conditionally `noexcept`.

In particular:
 - Do not mark a function `noexcept` just because it happens not to throw. The decision should be based on the *interface* of the function (which includes its pre-condition), and not its implementation.

Rationale:
 - Easy transition to using contracts when they come to C++.
 - Enable testing for pre-condition violations by conditionally throwing.


## Heap allocations

*snitch* code must not directly allocate heap (or "free store") memory. This means that a number of common C++ STL classes cannot be used (at least not with the default allocator):
 - `std::string`: use `std::string_view` (for constant strings) or `snitch::small_string` (for variable strings) instead.
 - `std::vector`, `std::map`, `std::set`, and their variants: use `std::array` (for fixed size arrays) or `snitch::small_vector` (for variable size arrays) instead.
 - `std::function`, use `snitch::small_function` instead.
 - `std::unique_ptr`, `std::shared_ptr`: use values on the stack, and raw pointers for non-owning references.

Unfortunately, the standard does not generally specify if a function or class allocates heap memory or not. We can make reasonable guesses for simple cases; in particular the following are fine to use:
 - `std::string_view`,
 - `std::array`,
 - `std::span`,
 - `std::variant` with `std::get_if` and `std::visit`,
 - `std::optional`,
 - `std::tuple`,
 - Functions in `<algorithm>` ([except `std::stable_sort`, `std::stable_partition`, and `std::inplace_merge`](https://stackoverflow.com/a/46714875/1565581)).
 - Concepts and type traits.

Any type or function not listed above *should* be assumed to use heap memory unless demonstrated otherwise.


## Heaver headers and compilation time

One of the advantages of *snitch* over competing testing framework is fast compilation of tests. To preserve this advantage, ["heavy" STL headers](https://artificial-mind.net/projects/compile-health/) should not be included in *snitch* headers unless absolutely necessary. However, they can be included in the *snitch* implementation `*.cpp` files. To enable this, as much code as possible should be placed in the `*.cpp` files rather than in headers.
