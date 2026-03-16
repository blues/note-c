set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package(Doxygen REQUIRED)

find_program(SPHINX_BUILD sphinx-build REQUIRED)

set(DOCS_CFG_DIR ${CMAKE_CURRENT_LIST_DIR})
set(DOCS_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(DOCS_SRC_DIR ${PROJECT_SOURCE_DIR}/docs)

set(SPHINX_OPTS "-j auto -W --keep-going -T -v")
separate_arguments(SPHINX_OPTS)

# Options for markdown build (without -W to allow extension warnings)
set(SPHINX_OPTS_MARKDOWN "-j auto --keep-going -T -v")
separate_arguments(SPHINX_OPTS_MARKDOWN)

add_custom_target(
    docs
    COMMAND ${CMAKE_COMMAND} -E env
        ${SPHINX_BUILD}
        -b html
        -c ${DOCS_CFG_DIR}
        -w ${DOCS_BUILD_DIR}/build.log
        ${SPHINX_OPTS}
        ${DOCS_SRC_DIR}
        ${DOCS_BUILD_DIR}
    USES_TERMINAL
    COMMENT "Running Sphinx HTML build..."
)

add_custom_target(
    docs-markdown
    COMMAND ${CMAKE_COMMAND} -E env
        ${SPHINX_BUILD}
        -b markdown
        -c ${DOCS_CFG_DIR}
        -w ${DOCS_BUILD_DIR}/markdown_build.log
        ${SPHINX_OPTS_MARKDOWN}
        ${DOCS_SRC_DIR}
        ${DOCS_BUILD_DIR}/markdown
    COMMAND python3 ${DOCS_CFG_DIR}/fix_markdown_anchors.py ${DOCS_BUILD_DIR}/markdown
    USES_TERMINAL
    COMMENT "Running Sphinx Markdown build and fixing anchors..."
)
