# xo-alloc/docs/CMakeLists.txt

xo_doxygen_collect_deps()
xo_docdir_doxygen_config()
xo_docdir_sphinx_config(
    index.rst install.rst introduction.rst implementation.rst)

# see xo-reader/doc or xo-unit/doc for working examples
# example.rst install.rst implementation.rst
