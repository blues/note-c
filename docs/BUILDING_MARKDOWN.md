# Building Markdown Documentation

The note-c documentation can now be generated in Markdown format in addition to HTML. This is useful for reusing the documentation in other contexts.

## Prerequisites

### Local Build

To build the documentation locally, you need the following installed:

1. **Doxygen** (version 1.9.8 or later)
2. **Sphinx** and required Python packages

Install Python dependencies:
```bash
pip install -r docs/requirements.txt
```

Or install individually:
```bash
pip install sphinx breathe sphinx-rtd-theme sphinxcontrib-jquery sphinx-markdown-builder
```

### Docker Build

Alternatively, use the provided Docker container which includes all dependencies:

```bash
docker build .devcontainer/ci/ --tag note_c_docs
```

## Building Documentation

### HTML Documentation (Original)

To build the HTML documentation:

```bash
# Using the build script
./scripts/build_docs.sh

# Or using CMake directly
cmake -B build/ -DNOTE_C_BUILD_DOCS:BOOL=ON
cmake --build build/ -- docs
```

The HTML output will be in `build/docs/index.html`

### Markdown Documentation (New)

To build the Markdown documentation:

```bash
# Using the build script
./scripts/build_docs_markdown.sh

# Or using CMake directly
cmake -B build/ -DNOTE_C_BUILD_DOCS:BOOL=ON
cmake --build build/ -- docs-markdown
```

The Markdown output will be in `build/docs/markdown/`

### Using Docker

To build documentation using Docker:

```bash
# Build the Docker image (if not already built)
docker build .devcontainer/ci/ --tag note_c_docs

# Run the build inside Docker
docker run --rm --volume $(pwd):/note-c/ --workdir /note-c/ note_c_docs bash -c "./scripts/build_docs_markdown.sh"
```

## Output Structure

The Markdown documentation maintains the same structure as the HTML documentation:

- `index.md` - Main documentation index
- `api_reference.md` - API reference documentation
- `getting_started.md` - Getting started guide
- `calling_the_notecard_api.md` - API usage guide
- `library_initialization.md` - Library initialization guide
- `ports.md` - Porting guide

The API documentation generated from Doxygen comments will be integrated into the Markdown files via the Breathe extension.

## Configuration

The Markdown builder configuration is in `docs/conf.py`:

- `markdown_http_base` - Base URL for external links (set to the hosted docs URL)
- `markdown_uri_doc_suffix` - Suffix for document URIs (set to `.html` for compatibility)

You can modify these settings if you need to adjust how links are generated in the Markdown output.

## Troubleshooting

### "Could NOT find Doxygen"
Install Doxygen version 1.9.8 or later, or use the Docker build method.

### "sphinx-build: command not found"
Install Sphinx and related Python packages using `pip install -r docs/requirements.txt`.

### "No module named 'sphinx_markdown_builder'"
Install the sphinx-markdown-builder package: `pip install sphinx-markdown-builder`

## Notes

- The Markdown output is generated from the same source as the HTML documentation
- Doxygen generates XML, which Breathe converts to Sphinx directives, which are then rendered to Markdown
- Some complex HTML formatting may not translate perfectly to Markdown
- The Markdown files are suitable for inclusion in other documentation systems or for viewing on platforms that render Markdown
