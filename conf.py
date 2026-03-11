# -*- coding: utf-8 -*-
"""Configuration"""

import datetime
from typing import Any

from yanga_core.docs.sphinx import SphinxConfig

day = datetime.date.today()

# meta data #################################################################

project = "SPLed"
copyright = f"{day.year}, RMT and Friends"
release = f"{day}"

# file handling #############################################################
# @see https://www.sphinx-doc.org/en/master/usage/configuration.html

templates_path = [
    "doc/_tmpl",
]

exclude_patterns = [
    "README.md",
    "build/modules",
    "build/deps",
    ".venv",
    ".git",
    "**/test_results.rst",  # We renamed this file, but nobody deletes it.
]

html_context: dict[str, Any] = {
    "env": {
        # Add current execution time in UTC
        "timestamp": datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%d %H:%M:%S %Z")
    },
    # TODO: the feature configuration is not yet implemented
    "config": {},
}

include_patterns = ["index.md", "doc/**"]

report_config = SphinxConfig()
html_context.update(report_config.html_context)
include_patterns.extend(report_config.include_patterns)

# Print all include patterns
print("Include patterns:")
for pattern in include_patterns:
    print(f" - {pattern}")

project = report_config.project

# configuration of built-in stuff ###########################################
# @see https://www.sphinx-doc.org/en/master/usage/configuration.html

numfig = True

# html config ###############################################################
# @see https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

# Omit "documentation" in title
html_title = f"{project} {release}"

html_theme = "sphinx_rtd_theme"

# Show hyper link which leeds to the source of page displayed
html_show_sourcelink = True

html_theme_options = {
    "canonical_url": "",
    "analytics_id": "",  # Provided by Google in your dashboard
    "display_version": True,
    "prev_next_buttons_location": "bottom",
    "style_external_links": True,
    "logo_only": False,
    "style_nav_header_background": "white",
    # Toc options
    "collapse_navigation": True,
    "sticky_navigation": True,
    "navigation_depth": 6,
    "includehidden": True,
    "titles_only": False,
}


html_logo = "doc/_figures/SPLED_logo.png"

# extensions and their configuration #########################################
extensions = []

extensions.append("sphinx_rtd_size")
sphinx_rtd_size_width = "90%"

extensions.append("sphinxcontrib.mermaid")

# Parse markdown files
extensions.append("myst_parser")
myst_enable_extensions = [
    "colon_fence",
    "deflist",
    "html_admonition",
    "html_image",
]

# Traceability extension
extensions.append("sphinx_needs")

# needs_types - this option allows the setup of own need types like bugs, user_stories and more.
needs_types = [
    dict(directive="req", title="Requirement", prefix="R_", color="#BFD8D2", style="node"),
    dict(
        directive="spec",
        title="Specification",
        prefix="S_",
        color="#FEDCD2",
        style="node",
    ),
    dict(
        directive="impl",
        title="Implementation",
        prefix="I_",
        color="#DF744A",
        style="node",
    ),
    dict(directive="test", title="Test Case", prefix="T_", color="#DCB239", style="node"),
]

# Define own link types
needs_extra_links = [
    # SWE.3 BP.5: link from Implementation (Software unit) to Specification (Software detailed design)
    # AND
    # SWE.2 BP.7: link from Requirements (Software Requirement) to Architecture (Software Architecture)
    {"option": "implements", "incoming": "is implemented by", "outgoing": "implements"},
    # SWE.4 BP.5: link from Test Case (Unit test specification) to Specification (Software detailed design)
    {"option": "tests", "incoming": "is tested by", "outgoing": "tests"},
    # SWE.4 BP.5: link from Test Case (Unit test specification) to Test Result (Unit test result)
    {"option": "results", "incoming": "is resulted from", "outgoing": "results"},
]

# The suffix of source filenames.
source_suffix = [
    ".md",
    ".rst",
]


def setup(app):  # type: ignore
    app.connect("source-read", report_config.render_with_jinja)
