# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import sys
import os
from pathlib import Path
from sphinx.cmd.build import get_parser
import sphinx_rtd_theme

args = get_parser().parse_args()

NOTE_C_BASE = Path(__file__).resolve().parents[1]
NOTE_C_BUILD = Path(args.outputdir).resolve()

# Add the '_extensions' directory to sys.path, to enable finding Sphinx
# extensions within.
sys.path.insert(0, str(NOTE_C_BASE / 'docs' / '_extensions'))

# Project information

project = 'note-c'
copyright = '2023, Blues'
author = 'Blues'

# General configuration

extensions = [
    'doxyrunner',
    'breathe',
    'sphinx.ext.autosectionlabel',
    'sphinxcontrib.jquery',
    'sphinx_markdown_builder',
]
suppress_warnings = [
    'autosectionlabel.*',
    'config.cache',  # Suppress locale directory warnings
    'misc.highlighting_failure',  # Suppress highlighting warnings
]
templates_path = ['_templates']
exclude_patterns = ['build/']
master_doc = 'index'

# Options for autosectionlabel

autosectionlabel_prefix_document = True

# Options for doxyrunner

doxyrunner_doxygen = os.environ.get('DOXYGEN_EXECUTABLE', 'doxygen')
doxyrunner_doxyfile = NOTE_C_BASE / 'docs' / 'Doxyfile.in'
doxyrunner_outdir = NOTE_C_BUILD / 'doxygen'
doxyrunner_fmt = True
doxyrunner_fmt_vars = {'NOTE_C_BASE': str(NOTE_C_BASE)}
doxyrunner_outdir_var = 'DOXY_OUT'
doxyrunner_silent = False

# Options for breathe

breathe_projects = {'note-c': str(doxyrunner_outdir / 'xml')}
breathe_default_project = 'note-c'
breathe_domain_by_extension = {
    'h': 'c',
    'c': 'c',
}

# Options for HTML output

html_theme = 'sphinx_rtd_theme'
html_logo = str(NOTE_C_BASE / 'assets' / 'blues_logo_no_text.png')
html_theme_options = {
    'logo_only': True,
    'prev_next_buttons_location': None
}

# Options for Markdown output

markdown_http_base = 'https://blues.github.io/note-c'
markdown_uri_doc_suffix = '.html'

# Custom handling for sphinx-markdown-builder compatibility
def setup(app):
    """Setup custom handlers for node types not supported by sphinx-markdown-builder."""
    from docutils import nodes
    from sphinx import addnodes
    import logging

    # Suppress the parallel reading warning for sphinx_markdown_builder
    logger = logging.getLogger('sphinx')

    # Check if we're using the markdown builder
    if app.config._raw_config.get('_running_markdown_builder', False):
        return

    # Add a flag to detect when markdown builder is running
    def builder_inited(app):
        if app.builder.name == 'markdown':
            app.config._running_markdown_builder = True
            # Register handlers for unsupported node types
            try:
                from sphinx_markdown_builder.translator import MarkdownTranslator

                # Handle desc_signature_line by treating it as a regular paragraph
                def visit_desc_signature_line(self, node):
                    pass

                def depart_desc_signature_line(self, node):
                    pass

                MarkdownTranslator.visit_desc_signature_line = visit_desc_signature_line
                MarkdownTranslator.depart_desc_signature_line = depart_desc_signature_line

            except ImportError:
                pass

    app.connect('builder-inited', builder_inited)
