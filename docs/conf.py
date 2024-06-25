# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'xo jit documentation'
copyright = '2024, Roland Conybeare'
author = 'Roland Conybeare'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

#extensions = []
extensions = [ "breathe",
               "sphinx.ext.mathjax",     # inline math
               "sphinx.ext.autodoc",     # generate info from docstrings
               "sphinxcontrib.ditaa",    # diagrams-through-ascii-art
               "sphinxcontrib.plantuml"  # text -> uml diagrams
              ]

# note: breathe requires doxygen xml output -> must have GENERATE_XML = YES in Doxyfile.in
#       match project name in Doxyfile.in
breathe_default_project = "xodoxxml"

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

pygments_style = 'sphinx'

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

#html_theme = 'alabaster'
html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']
html_favicon = '_static/img/favicon.ico'
