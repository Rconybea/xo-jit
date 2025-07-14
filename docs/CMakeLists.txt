# xo-indentlog/docs/CMakeLists.txt

xo_doxygen_collect_deps()
xo_docdir_doxygen_config()
xo_docdir_sphinx_config(
    index.rst install.rst
    logging_impl.rst
    pretty_impl.rst ppconfig-reference.rst ppstr-reference.rst
)
