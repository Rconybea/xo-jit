.. _glossary:

Glossary
--------

.. glossary::
    c.foo
      | llvm typename for automatically-generated closure type for a lambda
      | with name `foo`.

    e.foo
      | llvm typename for automatically-generated local environment for a
      | lambda with name `foo`.

    w.foo
      | llvm typename for automatically-generated wrapper function for a
      | primitive function `foo`.  The wrapper function accepts and ignores
      | an extra initial argument supplying an environment pointer.
      |
      | We apply this practice so that lambdas and primitives support the
      | same ABI,  so that we can support pointers to abstract functions
      | that might at runtime turn out to be either primitives or lambdas

    lambda
      | Common use is for lambda to refer to an anonymous function.
      | In llvm we need all functions to be named, and those names
      | have to be unique.
      |
      | Since all functions have to be named, we cheerfully adopt
      | the oxymoron 'named lambda'
      |
      | Practices:
      | 1. Automatically generate unique names for anonymous lambdas.
      | 2. Incorporate user-provided names for convenience, when provided.
      | 3. Still have to uniqueify names for user-provided nested lambdas.

    localenv
      | Shorthand for local environment.
      | Represents an explicit runtime repsentation for a struct that
      | holds captured function arguments with the ability to be persisted
      | (for example, moved to the the heap).
      |
      | Note that library `xo-expression` also uses the term environment, but differently.
      | In that context describes all function arguments.

    lvtype
      | Shorthand for `llvm::Type`:
      | llvm-owned representation for a datatype

    xsession
      | Shorthand for `llvm::orc::ExecutionSession`.
      | Manages running JIT-generated machine code in the host process

    td
      | Short for `xo::reflect::TypeDescr`.

    XO
      A set of integrated c++ libraries for complex event processing, with browser and python integration.
      `xo documentation here`_

.. _xo documentation here: https://rconybea.github.io/web/sw/xo.html

.. toctree::
